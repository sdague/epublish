<?php
/***
 *
 * This is a set of functions for accessing the database. Every database
 * access function should be in this file, and not anywhere else. It helps
 * isolate the interfaces we need to make the module work.
 *
 */

function epublish_list_publications() {
  $results = db_rewrite_sql(db_query("SELECT * FROM {epublish_publication"));
  $rows = array();
  while($obj = db_fetch_object($results)) {
    $rows[] = $obj;
  }
  return $rows;
}

function epublish_get_publication($pid) {
  return db_fetch_object(db_rewrite_sql(db_query("SELECT * FROM {epublish_publication} WHERE pid=%d", $pid)));
}


/**
 *  Set the specified edition to be current for that listed publication.
 *
 * TODO: write test case before enabling in code
 */
function epublish_make_edition_current($pid, $eid) {
  $record = (object) array("current_eid" => $eid, "pid" => $pid);
  epublish_update_publication($record);
}

/* TODO: in order for these to work, we're going to have to cast to objects */
function epublish_add_publication(&$pub) {
  return db_write_record('epublish_publication', $pub, NULL);
}

function epublish_update_publication(&$pub) {
  return db_write_record('epublish_publication', $pub, $pub->pid);
}

function epublish_exists_publication($id) {
  (db_fetch_object(db_rewrite(db_query("SELECT name FROM {epublish_publication} WHERE pid=%d", $pid)))) ? TRUE : FALSE;
}

function epublish_delete_publication($id) {
  db_query("DELETE FROM {epublish_publication} WHERE pid=%d", $id);
  /* TODO: should cascade into the other types */
}

?>
