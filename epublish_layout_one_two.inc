<?php // -*- drupal -*-

/**
 * @file
 * layout_one_two.inc.
 */
$description = 'one- and two-column';
$context = 'page';

/**
 * theme_epublish_layout_one_two: A themeable function to produce a list of section topics and their headlines.
 * This layout lists the first topic and its headlines in larger type, along with author bylines for each
 * headline.  Subsequent topics appear in smaller type in a two-column layout that shows headlines only.
 *
 * @param $section
 *   An array of topics, each of which is an object containing an attribute named "nodes" that is an array of node IDs
 * @param $params
 *   An associative array of parameters. This exists so that the function
 *   can be themed to handle whatever value(s) the theme designer wants passed
 *   into the layout. For example, an image might be passed into the function
 *   using $params = ('image_path' => $url, 'image_width' => $width, 'image_height' => $height, 'caption' => $caption)
 *
 * @return
 *   An HTML-formatted list of headlines.
 */
function theme_epublish_layout_one_two($topics, $params=NULL) {
  if ($params['description']) {
    $description = '<div class="description">'. $params['description'] .'</div>';
  }
  $output = '	<div class="epublish"><div class="one_two">'."$description\n";
  if ($topics) {
    $topic = array_shift($topics);
    foreach ($topic->nodes as $nid) {
      if (module_exists('i18n')) {
        $lang = i18n_get_lang();
          $transnids = translation_node_get_translations($nid);
          if ($transnids[$lang]) {
            $nid = $transnids[$lang]->nid;
        }
      }
      $node = node_load($nid);
      $output .= '<div class="title">'. l(t($node->title), "node/$nid") ."</div>\n";
      $output .= '<div class="user">'. t('By') .' '.'<span class="all_caps">'. $node->name ."</span></div>\n";
      if ($node->epublish_abstract) {
        $output .= '<div class="abstract">'. check_markup($node->epublish_abstract, $node->format, FALSE) ."</div>\n";
      }
    }
    $output .= "\n";
    $output .= '      <div class="other_heads">
	      <table width="100%">
	        <tr>
		      <td width="50%" valign="top">'."\n";
    $count = count($topics);
    $position = 1;
    foreach ($topics as $topic) {
      if ($position * 2 > $count) {
        $position = 0;
        $output .= '            </td>
			<td width="12"><img width="12" height="1"></td>
			<td width="50%" valign="top">'."\n";
      }
      $position++;
      $output .= '    		  <div class="topic">'. $topic->name ."</div>\n";
      foreach ($topic->nodes as $nid) {
        if (module_exists('i18n')) {
          $lang = i18n_get_lang();
          $transnids = translation_node_get_translations($nid);
          if ($transnids[$lang]) {
            $nid = $transnids[$lang]->nid;
          }
        }
        $node = node_load($nid);
        $output .= '<div class="topic_title">'. l(t($node->title), "node/$nid") ."</div>\n";
      }
    }
    $output .= '            </td>
		  </tr>
	    </table>
	  </div>';
  }
  $output .= "</div></div>\n";
  return $output;
}
