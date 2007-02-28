<?php
// $Id$

$description = 'regular custom';
$context = 'page';

/** 
 * theme_epublish_layout_regular_custom: A themeable function to produce a list of section topics and their nodes.
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
function theme_epublish_layout_regular_custom($topics, $params=NULL) {
  if ($params['description']) {
    //$description = '<div class="description">' . $params['description'] . '</div>';
  }

  $o_index = "" ;
  $o_detail = "" ;
  $o_header = "" ;
  $o_footer = "" ;
  $o_advertisement = "" ;
  $counter = 1 ;
  
  // define class variables
  // $css_title = "title" ;
  $css_title = "epub-title"; 
  $css_linktitle = "link-title" ;
  $css_abstract = "abstract" ;

  foreach ($topics as $topic) { // begin foreach (two) 
    if (count($topic->nodes)) {
      $subsection = epublish_get_section_by_title($topic->name); 

      // Add topic to output string, ignore if topic is top or bottom one
      if ( ($topic->weight != 99) && ($topic->weight != (-99)) && ($topic->weight != (-98)) ){

	// HTML Documentation comments 
	$o_index .= "<!-- BEGIN : " . t($topic->name) . " Links Section -->\n" ;
	$o_detail .= "<!-- BEGIN : " . t($topic->name) . " Section -->\n" ;

	if ($subsection->sid) {
	   $o_index .= '<div class="topic">  ' . l( $topic->name, '#T'.$sid. '_'.$counter ). "</div>\n";
	   $o_detail .= '<div class="topic">' . l(t($topic->name), "headlines/" . $subsection->sid ,array(),NULL,NULL,true ) . '<a name="T'.$sid.'_'.$counter.'">&nbsp;</a>' . "</div>\n";
        } else {
	   $o_index .= '<div class="topic"> ' . l( $topic->name, '#T'.$sid. '_'.$counter)."</div>\n";
	   $o_detail .= '<div class="topic"><a name="T'.$sid.'_'.$counter.'">&nbsp;</a>' . $topic->name . "</div>\n";
       }
      } 
      
	// for translation
      foreach ($topic->nodes as $nid) { // beigin of foreach (one)
				if (module_exist('translation')) {
 					$lang = i18n_get_lang();
		 			$transnid .= translation_node_nid($nid, $lang);
					if ($transnid) { $nid = $transnid; $transnid = '';}
				}
        $node = node_load((array('nid' => $nid)));
		if (strtolower(preg_replace('|[^a-zA-Z0-9_]+|','-',$topic->name) ) == "classified-ads" || strtolower(preg_replace('|[^a-zA-Z0-9_]+|','-',$topic->name) ) == "classifieds-ads" ) //"top-stories" ) 
		{	
			$css_title = "ad-title" ;
			$css_linktitle = "ad-link-title" ;
			$css_abstract = "ad-abstract" ;
		}

	// if it is not top or bottom topic add node title
       if ( ($topic->weight != 99) && ($topic->weight != (-99)) && ($topic->weight != (-98)) ){
	   			// hide bullet for classified-ads
		  		if (strtolower(preg_replace('|[^a-zA-Z0-9_]+|','-',$topic->name) ) == "classified-ads" || strtolower(preg_replace('|[^a-zA-Z0-9_]+|','-',$topic->name) ) == "classifieds-ads" ) //"top-stories" ) 
				{
				if(arg(2)) {
     				$o_index .= '<div class="' . $css_title . '"> '. l( $node->title,'#N'.$sid. '_'.$counter).'</div> ';
			        }else{
				$o_index .= '<div class="' . $css_title . '"> '. l( $node->title, '#N'.$sid. '_'.$counter).'</div> ';
				}
				$o_detail .= '<div class="' . $css_linktitle . ' ' . strtolower(preg_replace('|[^a-zA-Z0-9_]+|','-',$topic->name) ) . '-' .   $css_title . '">' . l(t($node->title), "node/$nid",array(),NULL,NULL,true) . '<a name="N'.$sid.'_'.$counter.'">&nbsp;</a>' . "</div>\n" ;
				} else {
				if(arg(2)) {
     				$o_index .= '<div class="' . $css_title . '"><font color="#990000"><b>o</b></font> '. l( $node->title, '#N'.$sid. '_'.$counter).'</div> ';
			        }else{
				 $o_index .= '<div class="' . $css_title . '"><font color="#990000"><b>o</b></font> '. l( $node->title, '#N'.$sid. '_'.$counter).'</div> ';
				}
				  $o_detail .= '<div class="' . $css_linktitle . ' ' . strtolower(preg_replace('|[^a-zA-Z0-9_]+|','-',$topic->name) ) . '-' .   $css_title . '"><font color="#990000"><b>o</b></font> ' . l(t($node->title), "node/$nid",array(),NULL,NULL,true) . '<a name="N'.$sid.'_'.$counter.'">&nbsp;</a>' . "</div>\n" ;
				}
	}
	
	// Add commented section 
        //$o_index .= '<div class="user">' . t('by') . ' ' . l($node->name, "blog/" . $node->uid) . "</div>\n";
        //$o_detail .= '<div class="user">' . t('by') . ' ' . l($node->name, "blog/" . $node->uid) . "</div>\n";


	// Now detail has got additional information
	// Add Abstract
        if ($node->epublish_abstract) {
	      if ( ($topic->weight != 99) && ($topic->weight != (-99)) && ($topic->weight != (-98)) ){
		$o_detail .= '<div class="' . $css_abstract . '">' . check_output($node->epublish_abstract . ' <span class="morelink"> ' . l("[more..]", "node/$nid",array(),NULL,NULL,true)  . ' </span>', $node->format) . " </div>\n";
	      } else {
	        // Footer
		if ( ($topic->weight == 99) ){
		   $o_footer .= '<div class="' . $css_abstract . '">' . check_output($node->epublish_abstract, $node->format) . "</div>\n";
		} 
		// Header
		if ( $topic->weight == (-99)) {
		   $o_header .= '<div class="' . $css_abstract . '">' . check_output($node->epublish_abstract, $node->format) . "</div>\n";
		}
		// Advertisement
		if ( ($topic->weight == (-98)) ){ 
		$o_advertisement .= ''. check_output($node->epublish_abstract, $node->format );
		}
	      }
        } // end of if for abstract

	$counter++ ;
      } // end of foreach statement (one)

    }
	if ( ($topic->weight != 99) && ($topic->weight != (-99)) && ($topic->weight != (-98)) ) {
		$o_index .= "\n<!-- END : " . t($topic->name) . " Links Section -->\n" ;
		$o_detail .= "\n<!-- END : " . t($topic->name) . " Section -->\n" ;
	   $o_detail .= '<div><a href="#">Top</a></div>' ;
	}

  } //end of foreach (two)


  if ( $o_advertisement ) {
	  $o_advertisement = '<div class="advertisement" >' . $o_advertisement . "</div>\n";
  }
  // Add a line after header if exists
  if ( $o_header ) {
	  $o_header = $o_header . '<hr/>' ;
  }
  // Add a line before footer if exists
  if ( $o_footer ) {
	  $o_footer = '<hr/>' . $o_footer ;
  }

  $output .= $o_header . $o_advertisement  . $o_index . $o_detail . $o_footer  ;

  $output = '<div class="epublish">' . '<div class="regular">' . $description . $output . '</div>' .'</div>';
  return $output ;
}
?>