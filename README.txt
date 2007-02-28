This module helps organize a group of postings into a publication, such as
a newspaper, magazine or newsletter. Each publication can have multiple
editions, ordered by publication date, volume and edition number, e.g.,
"Daily News, March 3, 2005, volume 4, number 3."

== CREATING AND ADMINISTERING PUBLICATIONS ==

The epublish module deals with three objects, which can be 
created or edited using admin/epublish:

* A "publication" could be a newsletter or magazine -- something that
  appears in multiple editions, either on a regular or sporadic basis.

* "Editions" consist of a single edition of a publication, typically
  labeled with a dateline, volume and number. A monthly magazine, for
  example, might be labeled "November 2004, volume 3, number 11." (The
  volume and number fields are optional.) Each publication can have a 
  designated "current" edition. The current edition will display in 
  all places where a publication is specified but no specific edition 
  number has otherwise been indicated. Each edition can be associated 
  with a list of the postings for that edition. A path can also be 
  entered for each edition, giving the URL to a downloadable non-HTML 
  version of the edition, such as a PDF document.

* Within each edition, content can be organized using a headlines 
  "section." Headlines sections organize postings by topic using 
  taxonomy terms, similar to the way that news websites such as the 
  New York Times categorize their content using topics such as 
  "international," "national," "sports," etc. In addition to topics 
  based on taxonomy terms, two special, non-taxonomy-based topic 
  groupings exist: "top stories" and "miscellaneous."

Users with permission to administer publications can use 
"admin/epublish" to create new editions of a publication and to
add or organize the list of nodes associated with each edition. (Each
node appears as a separate headline in edition listings.) Weight can
be used to control the order in which headlines appear.

== VIEWING PUBLICATIONS ==

A list of publications can be viewed at path "epublish". Each publication
has a unique numeric publication ID (pid). An individual publication can be 
using the path "epublish/{pid}" where {pid} is the publication's ID number, 
or at "epublish\{name}" where {name} is the publication name. An 
individual edition can be viewed using the path "epublish/{pid}/{eid}" or 
"epublish/{name}/{eid}" where {eid} is the edition ID number. As an 
alternative to {eid}, the path can also specify editions by their volume 
and number using the syntax "v{volume}n{number}". For example, the path
"epublish/Monthly%20News/v3n14" would show volume 3, number 14 of the 
publication named "Monthly News." Or, the path "epublish/4/v3" would 
list all editions of volume 3 of the publication with {pid} 4.

The path "epublish/{pid}/current" specifies a publication's current 
edition.

== VIEWING LATEST HEADLINES ==

In addition to displaying postings that have been collected into specific 
editions of a publication, you can also view a list of the most recent 
postings in each topic section at "epublish/headlines/{sid}" where 
{sid} is a numeric section ID. "Topics" use taxonomy terms to organize 
content according to the following rules:

* When editing the topics in each section, you can set an "automatically-
  included headlines limit" specifying the maximum number of headlines that 
  should be included for that topic and also set a time frame that limits 
  the search. For example, a time frame of "30 days" means that postings 
  older than that will be ignored. You can also specify which node types 
  are eligible for inclusion.

* Most topics refer to a taxonomy term and all of its descendants. For 
  example, a taxonomy might include the term "music," with child terms 
  "rock," "classical," "jazz." Nodes that have been tagged with one or 
  more of these terms would be included in the topic "music."

* In addition, there are two special topics, named "top stories" and 
  "miscellaneous," which do not filter for taxonomy terms at all. "Top 
  stories" are taken from stories that have been marked as "sticky at 
  top of lists". The "miscellaneous" topic appears at the bottom of 
  each section. (If, however, you set its "automatically-included 
  headlines limit" to zero, no miscellanous headlines will be 
  displayed.)

== PUBLICATIONS AND HEADLINES BLOCKS ==

The epublish module creates a sidebar block for each section, as 
well as a "epublish" block that lists all publications. To make 
blocks visible on your pages, use "admin/blocks".

== CUSTOMIZING LAYOUTS ==

The epublish module comes with several standard "layouts" that 
make it possible to customize the layout of publication pages. The 
"node views," "one- and two-column" and "regular" layouts are 
different page layouts for displaying nodes from a single edition. 
The "simple list of headlines only" layout is used on pages that 
list multiple editions of a publication.

The epublish directory includes a file "epublish.css" with
some sample CSS style tags written to work with the module's
standard layouts. You can add those tags to your existing Drupal
theme, or write your own CSS tags to produce a different style.

You can use "admin/settings/epublish" to select a headlines 
*layout* and *section* that apply to all publications and their 
editions. If you check the "custom layouts" or "custom sections" 
boxes, individual publications and editions can also be assigned 
their own separate layouts and sections that override the global 
settings, according to the following order of precedence:

=== Section selection ===

* If the _edition_ specifies a custom section, it takes precedence.

* Otherwise, if the _publication_ specifies a custom section, it 
  takes precedence.

* Otherwise, the globally-specified section is used.

=== Layout selection ===

* If the _section_ specifies a custom layout, it takes precedence.

* Otherwise, if the _edition_ specifies a custom layout, it 
  takes precedence.

* Otherwise, if the _publication_ specifies a custom layout, it 
  takes precedence.

* Otherwise, the globally-specified layout is used.

The ability to customize layouts and sections provides considerable 
flexibility for design purposes, but it can can also make 
administration of publications more complicated. Most users will probably
want to leave the "custom layouts" option turned off.

== CREATING ADDITIONAL LAYOUTS ==

Existing layouts are also themeable, and additional layouts can be added 
as needed.

To create a new layout, add a file to the "modules/epublish" directory 
with the filename layout_{name}.inc, where {name} is the name of the new 
layout. The file should begin by setting two variables:

* $description = a short description of the layout. (This description will 
  appear in pulldown menus generated by form_select).
  
* $context = the context in which the layout will be used. Currently two
  contexts are in use: "page" and "list". "Page" layouts are used to
  produce HTML that is intended to appear by itself in the main content
  column of a Drupal page. "List" layouts are used to produce HTML that
  is intended to appear as a simple listing of node titles within a
  broader listing of multiple editions of a publication.

The layout file also needs to define a function
 
   theme_epublish_layout{name}($topics, params=NULL) {
     ...some code...
	 return $output;
   }
   
where {name} is the name of the layout, $topics is an array of "topic"
objects, and $params is an optional associative array of additional 
parameters. (This exists so the function can be themed to handle 
whatever values a theme designer wants passed into the layout.)

Each "topic" object in the $topics array contains the following 
attributes:

  $topic->tid    =  a numeric topic ID. The values 0 and -1 correspond to 
                    "miscellaneous" and "top stories," respectively. All 
                    other values correspond to a taxonomy term with the 
                    same numeric term ID.

  $topic->name   =  the topic name

  $topic->nodes  =  an array of node IDs for each article associated with
                    the topic

Typically, the "theme_epublish_layout{name}" function iterates
through the list of topics, using logic such as the following:

   foreach ($topics as $topic) {
     $output .= (do something with $topic->name);
     foreach ($topic->nodes as $nid) {
       $node = node_load(array('nid' => $nid));
       $output .= (do something with $node);
     }
   }

Questions? Contact Sheldon Rampton (sheldon@prwatch.org).