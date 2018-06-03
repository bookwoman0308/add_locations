<?php

global $dbs;
$dbs = isset($dbs) ? $dbs : array();

$dbs['shared'] = array (
  'database' => 'shared',
  'username' => 'specialuser',
  'password' => 'specialpassword',
  'host' => 'geo-db-h02',
  'driver' => 'mysql',
);
