<?php

module_load_include('inc', 'mymodule_add_profiles', 'inc/env');

  /**
   * This function allows for querying the Shared database, which is not on the GHDx db server.
   */
function _open_shared_conn() {
    global $dbs;
    Database::addConnectionInfo('shared', 'default', $dbs['shared']);
    db_set_active('shared');
}

  /**
   * This function retrieves all subnational locations from the requested version in the shared.location_hierarchy_history table for the specified countries.
   * Mexico location_id = 65.  Japan => 75.  UK => 85. US => 95. Sweden => 105. The subnational locations are all coded as level 4. 
   * The default location version is v250, the one for the paper public release, but it can be changed if necessary.
   * Additional countries can be added by altering the query.
   */
 function _get_shared_loc($version_id = 250) {

  //Open the connection
  _open_shared_conn();

  //Retrieve all the new locations
  $query = "SELECT
    l.location_id AS id,
    l.location_name AS name
    FROM shared.location_hierarchy_history l
    WHERE l.location_set_version_id = :version_id
    AND l.level = 4
    AND l.parent_id IN (65,75,85,95,105)
    ORDER BY l.sort_order";
  
  $result = db_query($query, array(':version_id' => $version_id))->fetchAll(PDO::FETCH_KEY_PAIR);
  
  //Reset the database back to healthdata
  db_set_active();

  // $result will look like this:
  // Array
  // (
  //   [243] => Japan - Aichi
  //   [276] => Mexico - Aguascalientes
  //   [397] => United Kingdom - Wales
  // )

  return $result;
}

/**
   * This function retrieves all parent location ids from the requested version in the location_hierarchy_history table for the subnational children.
   * Japan, Mexico, Sweden, the US, and the UK.
   */
 function get_shared_parents($version_id = 300) {

  open_shared_conn();

  $query = "SELECT
    l.location_name AS name,
    l.parent_id AS parent_id
    FROM shared.location_hierarchy_history l
    WHERE l.location_set_version_id = :version_id
    AND l.level = 4 
    AND l.parent_id in (65,75,85,95,105)
    ORDER BY l.sort_order";
  
  $result = db_query($query, array(':version_id' => $version_id))->fetchAll(PDO::FETCH_KEY_PAIR);

  db_set_active();

  // $result will look like this. The id could not go first because of the key pair functionality which is a little annoying.
  // Array
  // (
  //   [Aichi] => 75
  //   [Zacatecas] => 65
  //   [Stockholm] => 105
  // )

  return $result;
}

/**
   * This function returns all the parent location ids for the subnationals being added. Made the array by hand instead of querying the shared db.
   */
function get_parent_ids() {
 
  return array(75 => 'Japan', 105 => 'Sweden', 85 => 'United Kingdom', 95 => 'United States', 65 => 'Mexico');

}
