# phpMyAdmin MySQL-Dump
# version 2.3.3pl1
# http://www.phpmyadmin.net/ (download page)
#
# Host: localhost
# Generation Time: Mar 19, 2005 at 01:50 AM
# Server version: 4.00.17
# PHP Version: 4.3.10
# --------------------------------------------------------

#
# Table structure for table `epublish_publication`
#

DROP TABLE IF EXISTS epublish_publication;
CREATE TABLE epublish_publication (
  pid int(10) unsigned NOT NULL default '0',
  name varchar(128) NOT NULL default '',
  description longtext NOT NULL,
  schedule varchar(128) NOT NULL default '',
  current_eid int(10) unsigned NOT NULL default '0',
  layout_list varchar(128) NOT NULL default '',
  layout_page varchar(128) NOT NULL default '',
  sid int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (pid)
) TYPE=MyISAM;
# --------------------------------------------------------

#
# Table structure for table `epublish_abstract`
#

DROP TABLE IF EXISTS epublish_abstract;
CREATE TABLE epublish_abstract (
  nid int(10) NOT NULL default '0',
  epublish_abstract longtext NOT NULL,
  use_as_teaser int(2) NOT NULL default '0',
  UNIQUE KEY nid (nid)
) TYPE=MyISAM;
# --------------------------------------------------------

#
# Table structure for table `epublish_edition`
#

DROP TABLE IF EXISTS epublish_edition;
CREATE TABLE epublish_edition (
  eid int(10) unsigned NOT NULL default '0',
  pid int(10) unsigned NOT NULL default '0',
  dateline varchar(128) NOT NULL default '',
  description longtext NOT NULL,
  volume int(10) NOT NULL default '0',
  number int(10) NOT NULL default '0',
  pubdate int(8) NOT NULL default '0',
  layout_list varchar(128) NOT NULL default '',
  layout_page varchar(128) NOT NULL default '',
  sid int(10) unsigned NOT NULL default '0',
  published int(4) NOT NULL default '0',
  PRIMARY KEY  (eid),
  KEY pid (pid)
) TYPE=MyISAM;
# --------------------------------------------------------

#
# Table structure for table `epublish_edition_node`
#

DROP TABLE IF EXISTS epublish_edition_node;
CREATE TABLE epublish_edition_node (
  eid int(10) unsigned NOT NULL default '0',
  nid int(10) unsigned NOT NULL default '0',
  weight tinyint(4) NOT NULL default '0',
  tid int(10) NOT NULL default '0',
  KEY eid (eid,nid),
  KEY tid (tid),
  KEY nid (nid)
) TYPE=MyISAM;
# --------------------------------------------------------

#
# Table structure for table `epublish_section`
#

DROP TABLE IF EXISTS epublish_section;
CREATE TABLE epublish_section (
  sid int(10) unsigned NOT NULL default '0',
  title varchar(128) NOT NULL default '',
  vid int(10) unsigned NOT NULL default '0',
  node_types longtext,
  timeframe varchar(16) NOT NULL default '',
  layout_list varchar(128) NOT NULL default '',
  layout_page varchar(128) NOT NULL default '',
  weight tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (sid)
) TYPE=MyISAM;
# --------------------------------------------------------

#
# Table structure for table `epublish_topic`
#

DROP TABLE IF EXISTS epublish_topic;
CREATE TABLE epublish_topic (
  sid int(10) unsigned NOT NULL default '0',
  tid int(10) NOT NULL default '0',
  count int(2) unsigned NOT NULL default '1',
  weight tinyint(4) NOT NULL default '0',
  node_types longtext,
  KEY tid (tid),
  KEY htid (sid)
) TYPE=MyISAM;
# --------------------------------------------------------

#
# Table structure for table `epublish_volume`
#

DROP TABLE IF EXISTS epublish_volume;
CREATE TABLE epublish_volume (
  pid int(10) unsigned NOT NULL default '0',
  volume int(10) unsigned NOT NULL default '0',
  dateline varchar(128) NOT NULL default '',
  KEY volume (volume),
  KEY pid (pid)
) TYPE=MyISAM;

