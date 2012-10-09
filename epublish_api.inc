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


/**
 * epublish_get_publication: get a publication record from the database
 *
 * @param $pid
 *	 A publication ID number
 *
 * @return
 *	 An object containing all of the values for a single record in the
 *	 {epublish_publication} table.
 */
function epublish_get_publication($pid) {
  return db_fetch_object(db_query(db_rewrite_sql("SELECT * FROM {epublish_publication} WHERE pid=%d"), $pid));
}

/**
 * epublish_get_publication_by_name: get a publication record from the database, based on its name
 *
 * @param $name
 *	 A publication name
 *
 * @return
 *	 An object containing all of the values for a single record in the
 *	 {epublish_publication} table.
 */
function epublish_get_publication_by_name($name) {
  $name = preg_replace('/_/', ' ', $name);
  return db_fetch_object(db_query("SELECT * FROM {epublish_publication} WHERE LOWER('%s') LIKE LOWER(name)", trim($name)));
}


/* TODO: in order for these to work, we're going to have to cast to objects */
function epublish_add_publication(&$pub) {
  return db_write_record('epublish_publication', $pub, NULL);
}

function epublish_update_publication(&$pub) {
  return db_write_record('epublish_publication', $pub, $pub->pid);
}

function epublish_exists_publication($id) {
  (db_fetch_object(db_query(db_rewrite_sql("SELECT name FROM {epublish_publication} WHERE pid=%d"), $pid))) ? TRUE : FALSE;
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
/**
 * epublish_get_edition: get an epublish_edition record from the database
 *
 * @param $eid
 *	 An edition ID number
 *
 * @return
 *	 An object containing all of the values for a single record in the
 *	 {epublish_edition} table.
 */
function epublish_get_edition($eid) {
  return db_fetch_object(
                         db_query(
                                  db_rewrite_sql("SELECT * FROM {epublish_edition} WHERE eid=%d"),
                                  $eid)
                         );
}

/**
 * epublish_current_edition: return the current edition ID number for a publication. If no edition is specified, returns the edition with the latest publication date.
 *
 * @param $pid
 *	 A publication ID number
 *
 * @return
 *	 An edition ID number
 */
function epublish_get_current_edition_id($pid) {
  $pub = epublish_get_publication($pid);
  if ($pub->current_eid) {
    return $pub->current_eid;
  } else {
    // return most recent
    return db_fetch_object(
                           db_query(
                                    db_rewrite_sql("SELECT eid FROM {epublish_edition} WHERE pid='%d' ORDER BY volume DESC, number DESC, pubdate DESC, eid DESC LIMIT 1"),
                                    $pid)
                           );
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

/**
 * epublish_list_editions: get an edition list either by publication
 * or by all. Specify sort order
 *
 * @param $pid
 *	 publication id
 * @param $order
 *      order for sort, either 'DESC' or 'ASC'
 *
 * @return
 *	 A list of the edition objects sorted accordingly
 */

function epublish_list_editions($pid=0, $order='DESC') {
  $results = NULL;
  if ($pid != 0) {
    $results = db_query(db_rewrite_sql("SELECT * FROM {epublish_edition} WHERE pid=%d ORDER BY volume %s, number %s, pubdate %s, eid %s"), $pid, $order, $order, $order, $order);
  } else {
    $results = db_query(db_rewrite_sql("SELECT * FROM {epublish_edition} ORDER BY volume %s, number %s, pubdate %s, eid %s"), $order, $order, $order, $order);
  }
  $rows = array();
  while($obj = db_fetch_object($results)) {
    $rows[] = $obj;
  }
  return $rows;
}

function epublish_get_published_editions($pid=0, $vid=0, $num=0, $order='DESC') {
  $results = NULL;
  $sqlbase = "SELECT * FROM {epublish_edition} WHERE published=1 AND";
  $sqlorder = "ORDER BY volume %s, number %s, pubdate %s, eid %s";

  if ($pid and $vid and $num) {
    $results = db_query(db_rewrite_sql("$sqlbase pid=%d AND volume=%d AND number=%d $sqlorder"),
                        $pid, $vid, $num, $order, $order, $order, $order);
  } elseif ($pid and $vid) {
    $results = db_query(
                        db_rewrite_sql("$sqlbase pid=%d AND volume=%d $sqlorder"),
                        $pid, $vid, $order, $order, $order, $order);
  } elseif ($pid) {
    $results = db_query(
                        db_rewrite_sql("$sqlbase pid=%d $sqlorder"),
                        $pid, $order, $order, $order, $order);
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


/*********************************************************
 *
 *  Section Functions
 *
 ********************************************************/

/*
 * epublish_get_sections: get a list of sections as objects
 */
function epublish_list_sections() {
  $result = db_query("SELECT * FROM {epublish_section}");
  while ($section = db_fetch_object($result)) {
    $sections[$section->sid] = $section->title;
  }
  return $sections;
}

function epublish_num_sections() {
  return count(epublish_list_sections());
}

function epublish_create_default_sections() {

}

/**
 * epublish_get_section: get an epublish_section record from the database
 *
 * @param $sid
 *	 A publication section ID
 *
 * @return
 *	 An object containing all of the values for a single record in the
 *	 {epublish_section} table.
 */
function epublish_get_section($sid) {
  return db_fetch_object(db_query("SELECT * FROM {epublish_section} WHERE sid='%d'", $sid));
}

/**
 * epublish_get_section_by_title: get an epublish_section record from the database, based on its title
 *
 * @param $title
 *	 The title of a publication section
 *
 * @return
 *	 An object containing all of the values for a single epublish_section record in the
 *	 {epublish_section} table.
 */
function epublish_get_section_by_title($title) {
  return db_fetch_object(db_query("SELECT * FROM {epublish_section} WHERE title='%s'", $title));
}

function epublish_create_section($section) {
  return db_write_record('epublish_section', $section);
}

function epublish_update_section($section) {
  return db_write_record('epublish_section', $section, $section->sid);
}

function epublish_delete_section($sid) {
  return db_query("DELETE FROM {epublish_section} WHERE sid = '%d'", $sid);
}

/*********************************************************
 *
 * Epublish functions related to nodes
 *
 *********************************************************/

function epublish_get_editions_for_node($nid) {
  $results = db_query(db_rewrite_sql('SELECT * FROM {epublish_edition_node} een WHERE nid = %d'), $node->nid);
  $rows = array();
  while($obj = db_fetch_object($results)) {
    $rows[] = $obj;
  }
  return $rows;
}

function epublish_exists_node_in_edition($nid, $eid) {
  (db_fetch_object(db_query(db_rewrite_sql("SELECT name FROM {epublish_edition_node} WHERE nid=%d and eid=%d"), $nid, $eid))) ? TRUE : FALSE;
}

function epublish_remove_node_from_edition($nid, $eid) {
  db_query("DELETE FROM {epublish_edition_node} WHERE eid = '%d' AND nid = '%d'", $eid, $nid);
}

function epublish_remove_node_from_all_editions($nid) {
  return db_query("DELETE FROM {epublish_edition_node} WHERE nid = '%d'", $eid, $nid);
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
