<?php
/***
 *
 * This is a set of functions for accessing the database. Every database
 * access function should be in this file, and not anywhere else. It helps
 * isolate the interfaces we need to make the module work.
 *
 */

function epublish_get_all_publications() {
  $results = db_rewrite(db_query("SELECT * FROM {epublish_publication"));
  $rows = array();
  while($row = db_fetch_array($results)) {
    $rows[] = $row;
  }
  return $rows;
}

function epublish_get_publication($pid) {
  return db_fetch_object(db_rewrite(db_query("SELECT * FROM {epublish_publication} WHERE pid=%d", $pid)));
}

?>