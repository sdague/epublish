<?php
// $Id$
/**
 * @file
 * epublish_layout_indexed.inc
 */

$description = 'node views';
$context = 'page';

/** 
 * theme_epublish_layout_nodes: A themeable function to produce a list of section topics and their nodes.
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
 *   An HTML-formatted list of nodes.
 */
function theme_epublish_layout_nodes($topics, $params=NULL) {
  if ($params['description']) {
    $description = '<div class="description">'. $params['description'] .'</div>';
  }
  // The default assumption for this layout is that topics are NOT shown
  if ($params['show_topics']) {
    foreach ($topics as $topic) {
      if (count($topic->nodes)) {
        $subsection = epublish_get_section_by_title($topic->name);
        if ($subsection->sid) {
          $output .= '<div class="topic">'. l(t($topic->name), 'headlines/'. $subsection->sid) ."</div>\n";
        } 
        else {
          $output .= '<div class="topic">'. $topic->name ."</div>\n";
        }
        foreach ($topic->nodes as $nid) {
          if (module_exists('i18n')) {
            $lang = i18n_get_lang();
            $transnids = translation_node_get_translations($nid);
            if ($transnids[$lang]) {
              $nid = $transnids[$lang]->nid;
            }
          }
          $output .= node_view(node_load($nid), 1);
        }
      }
    }
  }
  else {
    foreach ($topics as $topic) {
      foreach ($topic->nodes as $nid) {
        if (module_exists('i18n')) {
          $lang = i18n_get_lang();
            $transnids = translation_node_get_translations($nid);
            if ($transnids[$lang]) {
              $nid = $transnids[$lang]->nid;
            }
        }
        $output .= node_view(node_load($nid), 1);
      }
    }
  }
  return '<div class="epublish"><div class="nodes">'. $description . $output ."</div></div>";
}
