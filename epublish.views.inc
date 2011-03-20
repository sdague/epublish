<?php // -*- drupal -*-

  /**
   * @file
   *
   * Expose epublish data in views to make it easier to pull into other areas
   */

  /**
   * Implementation of hook_views_data().
   */
function epublish_views_data() {
  $data;
  _epublish_define_publication_table($data);
  _epublish_define_edition_table($data);
  _epublish_define_edition_node_table($data);

  return $data;

}

function _epublish_define_publication_table(&$data) {
  $data['epublish_publication']['table']['group'] = t('Epublish');

  $data['epublish_publication']['table']['base'] = array(
    'field' => 'pid',
    'title' => t('Epublish Publication'),
    'help' => t('An epublish publication')
  );

  $data['epublish_publication']['table']['join']['node'] = array(
    'left_table' => 'epublish_edition',
    'left_field' => 'pid',
    'field' => 'pid',
  );

  _epublish_define_view_number($data, 'epublish_publication', 'pid', 'Publication Id', 'Unique id associated with the publication');

  _epublish_define_view_text_field($data, 'epublish_publication', 'name', 'Name', 'Name of the publication');

  _epublish_define_view_text_field($data, 'epublish_publication', 'description', 'Description', 'Description of the publication');

  _epublish_define_view_number($data, 'epublish_publication', 'current_eid', 'Current Edition', 'Used to figure out the current edition of a publication');

  _epublish_define_view_text_field($data, 'epublish_publication', 'layout_list', 'Listing Layout', 'Layout used for the listing view');

  _epublish_define_view_text_field($data, 'epublish_publication', 'layout_node', 'Node Layout', 'Layout used for the node view');

}

function _epublish_define_edition_table(&$data) {
  $data['epublish_edition']['table']['group'] = t('Epublish');

  // Make it possible to create views that are editions, not just node based
  $data['epublish_edition']['table']['base'] = array(
    'field' => 'eid',
    'title' => t('Epublish Edition'),
    'help' => t('An edition of an epublish publication'),
  );

  _epublish_define_view_reference($data, 'epublish_edition', 'pid', 'Publication Id', 'Publication that includes the edition, for use in relationships', 'epublish_publication', 'pid');

  _epublish_define_view_number($data, 'epublish_edition', 'eid', 'Edition Id', 'Unique id associated with the edition');

  _epublish_define_view_number($data, 'epublish_edition', 'pubdate', 'Publication Date', 'YYYYMMDD when published');

  _epublish_define_view_number($data, 'epublish_edition', 'volume', 'Volume Number', 'User specified volume number');

  _epublish_define_view_number($data, 'epublish_edition', 'number', 'Edition Number', 'User specified edition number');

  _epublish_define_view_boolean_field($data, 'epublish_edition', 'published', 'Published', 'Whether or not the edition is published');

  _epublish_define_view_text_field($data, 'epublish_edition', 'dateline', 'Dateline', 'User specified dateline for the edition');

  _epublish_define_view_text_field($data, 'epublish_edition', 'description', 'Description', 'User specified description for the edition');


  $data['epublish_edition']['table']['join']['node'] = array(
    'left_table' => 'epublish_edition_node',
    'left_field' => 'eid',
    'field' => 'eid',
  );
  $data['epublish_edition']['table']['join']['epublish_publication'] = array(
    'left_field' => 'pid',
    'field' => 'pid',
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

  _epublish_define_view_reference($data, 'epublish_edition_node', 'nid', 'Node Id', 'Node Id included in an edition, used in relationships between nodes adn editions', 'node', 'nid');

  _epublish_define_view_reference($data, 'epublish_edition_node', 'eid', 'Edition Id', 'Id of the edition, using in relationships between nodes and editions', 'epublish_edition', 'eid');
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


function _epublish_define_view_boolean_field(&$data, $table, $column, $title, $help = '') {
  $data[$table][$column] = array(
    'title' => $title, // The item it appears as on the UI,
    'help' => $help, // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'field' => $column, // the real field
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
     ),
    'argument' => array('handler' => 'views_handler_argument_numeric'),
    'filter' => array('handler' => 'views_handler_filter_boolean_operator'),
    'sort' => array('handler' => 'views_handler_sort'),
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