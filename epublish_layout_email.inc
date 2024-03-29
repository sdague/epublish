<?php

/**
 * @file
 * epublish_layout_email.inc
 */
$description = 'email template simple list of headlines only';
$context = 'email';

/**
 * theme_epublish_layout_email: A themeable function to show a simple
 * list of headlines.  This layout lists headlines only, and does not
 * show topic names. The list layout is designed for use by
 * epublish.module, which can use it to control the order in which
 * headlines display in each edition of a publication.
 *
 * @param $topics
 *   An array of topics returned from epublish_build_section3.
 * @param $params
 *   An associative array of parameters. This exists so that the function
 *   can be themed to handle whatever value(s) the theme designer wants passed
 *   into the layout. For example, an image might be passed into the function
 *   using $params = ('image_path' => $url, 'image_width' => $width, 'image_height' => $height, 'caption' => $caption)
 *
 * @return
 *   An HTML-formatted list of headlines.
 */
function theme_epublish_layout_email($topics, $params=NULL) {
  if ($params['description']) {
    $description = '<div class="description">'. $params['description'] .'</div>';
  }
  $output = '<div class="pub_list_email">'. $description .'<ul>';
  foreach ($topics as $topic) {
    foreach ($topic->nodes as $nid) {
      if (module_exists('i18n')) {
        $lang = i18n_get_lang();
          $transnids = translation_node_get_translations($nid);
          if ($transnids[$lang]) {
            $nid = $transnids[$lang]->nid;
        }
      }
      $node = node_load($nid);
      $output .= "<li>" . l(t($node->title), "node/$nid");
      if ($node->epublish_abstract) {
        $output .= ': '. check_markup($node->epublish_abstract, $node->format, FALSE);
      }
      $output .= "</li>\n";
    }
  }
  $output .= "</ul></div>\n";
  return $output;
}
