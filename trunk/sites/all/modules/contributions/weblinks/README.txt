 $Id: README.txt,v 1.1.8.2 2008/09/06 00:17:28 nancyw Exp $
 @file
 README.txt file for Weblinks.


README.txt file
===============
Web Links provides a comprehensive way to post links on your site. All links are nodes, which can be put into taxonomies/categories and administered. The most common options are set by default so the module should work well right out of the box. However, there is a great deal of customization that may also be done to make the installation fit your needs. Additionally, a variety of blocks may be enabled (see below).

The Link Validity Checker feature will check links during a Cron run to see if they are still valid. You may have the module update any links that are marked as moved and unpublish links that have returned an error twice in a row.

A separate filter module (distributed with the package) may be used to easily insert links into other content.


Dependencies
============

The core taxonomy module must be enabled.

Incompatibilities
=================

Category module appears to be incompatible with this module.

The Links module's database is incompatible and will not work with Weblinks.

Installation and Settings
=========================

The most up-to-date information on the module can be found at:
http://drupal.org/node/273907

Recent Changes
==============

It was brought to our attention that there really was no way to control who has access to the main
links page. We have added a new permission "access web links" to address this oversight. You will
need to set this for any roles you wish to access Web Links.