<?php // -*- drupal -*-

  /**
   * @file
   *
   * Expose epublish data in views to make it easier to pull into other areas
   */

function epublish_views_data() {
  $data;
  _epublish_define_edition_table($data);
  _epublish_define_edition_node_table($data);

  return $data;

}

function _epublish_define_edition_table(&$data) {
  $data['epublish_edition']['table']['group'] = t('Epublish');
  _epublish_define_view_reference($data, 'epublish_edition', 'pid', 'Publication Id', 'Publication that includes the edition', 'epublish_publication', 'pid');

  _epublish_define_view_number($data, 'epublish_edition', 'eid', 'Edition Id', 'Unique id associated with the edition');

  _epublish_define_view_number($data, 'epublish_edition', 'pubdate', 'Publication Date', 'YYYYMMDD when published');

  _epublish_define_view_text_field($data, 'epublish_edition', 'dateline', 'Dateline');

  $data['epublish_edition']['table']['join']['node'] = array(
    'left_table' => 'epublish_edition_node',
    'left_field' => 'eid',
    'field' => 'eid',
  );
}

function _epublish_define_edition_node_table(&$data) {
  $data['epublish_edition_node']['table']['group'] = t('Epublish');

  $data['epublish_edition_node']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid'
    ),
    'epublish_edition' => array(
      'left_field' => 'eid',
      'field' => 'eid'
    )
  );

  _epublish_define_view_reference($data, 'epublish_edition_node', 'nid', 'Node Id', 'Node Id included in an edition', 'node', 'nid');

  _epublish_define_view_reference($data, 'epublish_edition_node', 'eid', 'Edition Id', 'Id of the edition', 'epublish_edition', 'eid');
}


function _epublish_define_view_reference(&$data, $table, $column, $title, $help, $base, $field) {
  $data[$table][$column] = array(
    'title' => $title, // The item it appears as on the UI,
    'help' => $help, // The help that appears on the UI,
    'field' => array(
      'field' => $column, // the real field
      'handler' => 'views_handler_field_numeric',
    ),
    // Information for displaying a title as a field

    'relationship' => array(
      'base' => $base,
      'field' => $field,
      'handler' => 'views_handler_relationship',
      'label' => $title,
    ),
  );
}


function _epublish_define_view_number(&$data, $table, $column, $title, $help) {
  $data[$table][$column] = array(
    'title' => $title, // The item it appears as on the UI,
    'help' => $help, // The help that appears on the UI,
    // Information for displaying a title as a field
    'field' => array(
      'field' => $column, // the real field
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array('handler' => 'views_handler_filter_numeric'),
    'sort' => array('handler' => 'views_handler_sort'),
  );
}


function _epublish_define_view_text_field(&$data, $table, $column, $title, $help = '') {
  $data[$table][$column] = array(
    'title' => $title, // The item it appears as on the UI,
    'help' => $help, // The help that appears on the UI,
    // Information for displaying a title as a field
    'field' => array(
      'field' => $column, // the real field
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array('handler' => 'views_handler_argument_string'),
    'filter' => array('handler' => 'views_handler_filter_string'),
    'sort' => array('handler' => 'views_handler_sort'),
  );
}