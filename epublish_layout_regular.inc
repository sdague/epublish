<?php
// $Id$

$description = 'regular';
$context = 'page';

/** 
 * theme_epublish_layout_regular: A themeable function to produce a list of section topics and their nodes.
 *
 * @param $topics
 *   An array of topics
 * @param $params
 *   An associative array of parameters. This exists so that the function
 *   can be themed to handle whatever value(s) the theme designer wants passed
 *   into the layout. For example, an image might be passed into the function
 *   using $params = ('image_path' => $url, 'image_width' => $width, 'image_height' => $height, 'caption' => $caption)
 *
 * @return
 *   An HTML-formatted list of topics and their nodes.
 */
function theme_epublish_layout_regular($topics, $params=NULL) {

  if ($params['description']) {
    $description = '<div class="description">' . $params['description'] . '</div>';
  }
  foreach ($topics as $topic) {
    if (count($topic->nodes)) {
      $subsection = epublish_get_section_by_title($topic->name);
      if ($subsection->sid) {
        $output .= '<div class="topic">' . l(t($topic->name), "headlines/" . $subsection->sid) . "</div>\n";
      } else {
        $output .= '<div class="topic">' . $topic->name . "</div>\n";
      }
      foreach ($topic->nodes as $nid) {
				if (module_exists('translation')) {
 					$lang = i18n_get_lang();
		 			$transnid .= translation_node_nid($nid, $lang);
					if ($transnid) { $nid = $transnid; $transnid = '';}
				}
        $node = node_load($nid);

        $output .= '<div class="title">' . l(t($node->title), "node/$nid") . "</div>\n";
        $output .= '<div class="user">' . t('by') . ' ' . l($node->name, "blog/" . $node->uid) . "</div>\n";
        if ($node->epublish_abstract) {
          $output .= '<div class="abstract">' . check_markup($node->epublish_abstract, $node->format) . "</div>\n";
        }
      }
    }
  }
  return '<div class="epublish"><div class="regular">' . $description . $output . '</div></div>';
}
?>
