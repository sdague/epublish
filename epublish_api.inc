<?php
/***
 *
 * This is a set of functions for accessing the database. Every database
 * access function should be in this file, and not anywhere else. It helps
 * isolate the interfaces we need to make the module work.
 *
 */

/*************************************************
 *
 * Publication Database functions
 *
 *************************************************/

function epublish_list_publications() {
  $results = db_query(db_rewrite_sql("SELECT * FROM {epublish_publication"));
  $rows = array();
  while($obj = db_fetch_object($results)) {
    $rows[] = $obj;
  }
  return $rows;
}

function epublish_get_publication($pid) {
  return db_fetch_object(db_query(db_rewrite_sql("SELECT * FROM {epublish_publication} WHERE pid=%d", $pid)));
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

/*************************************************
 *
 * Edition Database functions
 *
 *************************************************/

function epublish_get_edition($eid) {
  return db_fetch_object(db_query(db_rewrite_sql("SELECT * FROM {epublish_edition} WHERE eid=%d", $eid)));
}

function epublish_get_current_edition($pid) {
  $pub = epublish_get_publication($pid);
  if ($pub->current_eid) {
    return epublish_get_edition($pub->current_eid);
  } else {
    return db_fetch_object(db_query(db_rewrite_sql("SELECT eid FROM {epublish_edition} WHERE pid='%d' ORDER BY volume DESC, number DESC, pubdate DESC, eid DESC LIMIT 1", $pid)));
  }
}

function epublish_update_edition(&$ed) {
  return db_write_record('epublish_edition', $ed, $ed->eid);
}

function epublish_add_edition(&$ed) {
  return db_write_record('epublish_edition', $ed);
}

function epublish_delete_edition($eid) {
  db_query("DELETE FROM {epublish_edition} WHERE eid=%d", $eid);
  db_query("DELETE FROM {epublish_edition_node} WHERE eid = %d", $eid);
}

function epublish_list_editions($pid=0) {
  $order = variable_get('epublish_edition_order', 'DESC');
  $results = NULL;
  if ($pid != 0) {
    $results = db_query(db_rewrite_sql("SELECT * FROM {epublish_edition} WHERE pid=%d ORDER BY volume %s, number %s, pubdate %s, eid %s", $publication->pid, $order, $order, $order, $order));
  } else {
    $results = db_query(db_rewrite_sql("SELECT * FROM {epublish_edition} ORDER BY volume %s, number %s, pubdate %s, eid %s", $order, $order, $order, $order));
  }
  $rows = array();
  while($obj = db_fetch_object($results)) {
    $rows[] = $obj;
  }
  return $rows;
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

/***
 *
 * Epublish functions related to nodes
 *
 *********************************************************/

function epublish_get_editions_for_node($nid) {
  $results = db_query(db_rewrite_sql('SELECT * FROM {epublish_edition_node} een WHERE nid = %d', $node->nid));
  $rows = array();
  while($obj = db_fetch_object($results)) {
    $rows[] = $obj;
  }
  return $rows;
}

function epublish_exists_node_in_edition($nid, $eid) {
  (db_fetch_object(db_rewrite(db_query("SELECT name FROM {epublish_edition_node} WHERE nid=%d and eid=%d", $nid, $eid)))) ? TRUE : FALSE;
}

function epublish_remove_node_from_edition($nid, $eid) {
  db_query("DELETE FROM {epublish_edition_node} WHERE eid = '%d' AND nid = '%d'", $eid, $nid);
}

function epublish_add_node_to_edition($eid, $nid, $weight=0, $tid=0) {
  $data = (object) array("eid" => $eid, "nid" => $nid, "weight" => $weight, "tid" => $tid);
  epublish_remove_node_from_edition($eid, $nid);
  return db_write_record("epublish_edition_node", $data);
}


/**
 * This is slightly more complicated because we only want published nodes
 */
function epublish_get_nodes_for_edition($eid) {
  $result = db_query(db_rewrite_sql("SELECT n.nid FROM {node} n, {epublish_edition_node} een WHERE een.nid = n.nid AND een.eid='%d' AND n.status='1' ORDER BY n.sticky DESC, n.created DESC"), $eid);
  while ($node = db_fetch_object($result)) {
    $nodes[] = $node->nid;
  }
  return $nodes;
}




?>
