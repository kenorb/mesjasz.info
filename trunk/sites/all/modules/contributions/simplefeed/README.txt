// $Id: README.txt,v 1.18.4.28 2008/10/15 01:38:00 m3avrck Exp $

###   ABOUT   #############################################################################

SimpleFeed, Version 3.1

Author:

 Ted Serbinski, aka, m3avrck
   hello@tedserbinski.com
   http://tedserbinski.com


Contributors:

 Scott Reynolds, aka, scott reynolds
   scott@scottreynolds.us
   http://scottreynolds.us

 Bill O'Connor, aka, csevb10
   billiamo@gmail.com
   http://achieveinternet.com

 Matt Farina, aka, mfer
   matt.farina@gmail.com
   http://www.mattfarina.com 

Requirements: Drupal 6.x



###   FEATURES   #############################################################################

- uses SimplePie <http://simplepie.org/> as the default feed parsing engine
- very simple and fast: everything is a node, use hook_nodeapi() to manipulate anything you want
- feed & feed items obey default node workflow + options (e.g., set if feed items should be published by default)
- auto-taxonomy, automatically add categories in feeds to Drupal's taxonomy system
- revisions support for both feeds & feed items
- automatically delete feed items after a certain amount of time
- automatically generate feed item titles if they don't exist in the feed
- optionally expire specific feed items by editing their expiration time
- edit any feed or feed item that comes in
- manually insert feed items into feeds
- customizable default input format for imported feeds (e.g., which tags to strip from feeds, if any)
- postgres support
- duplicate checking of both feeds & feed items
- granular feed permissions
- optional views support
- optional token support
- optionally verify feeds before adding to database
- optional stats collecting module for per-feed stats and global stats on feed usage


###   INSTALLATION   #############################################################################

1. Download and unzip the SimpleFeed module into your modules directory.

2. Download the SimplePie 1.1 library: http://simplepie.org/
   Place simplepie.inc in your SimpleFeed module directory.

3. Goto Administer > Site Building > Modules and enable both SimpleFeed and SimpleFeed Item.

4. Goto Administer > Site Configuration > Simplefeed and configure the options.

5. If you wish to use auto-taxonomy support:
   a. Enable taxonomy module
   b. Create a new vocabulary and assign it to both the "feed" and "feed item" node types.
   c. Check the "free tagging" box, since this feature only works with free tagging enabled taxonomies.
   d. Revisit the settings page to have this vocabulary shown.

6. Goto User Management > Access control and set the permissions for both feeds and feed items.

7. Goto Create content > Feed to create a new feed.

8. Setup cron to run and download need items for the newly added feed.

9. Optionally install Views module that provides various views for seeing all feeds & feed items on your site.

10. Optionally enable the default block provided (requires Views).



###   NOTES   #############################################################################

- Even though aggressive says this caching won't work, it will -- it's only needed for non-cached pages and hence no affect.

- If some feeds aren't parsed, likely this is an issue with SimplePie <http://simplepie.org/> and not the module, which only
  implements this library (e.g., it doesn't actually parse anything). Please submit those bugs to: http://simplepie.org/support/

- If you want your feeds to use a different default input format than "filtered html", make sure you give
  anonymous users the right to use that input filter under Admin > Site Configuration > Input Formats

- If cron.php does not return because of a script timeout (e.g., trying to parse too many feeds at once),
  wget calls by default the site up to 20 times total until it gets a valid response. Change your wget to -t 1 (try once)
  0   *   *   *   *   wget -O - -q -t 1 http://www.example.com/cron.php

- Any registered Drupal user that creates a feed node will also own all subsequent feed item children. If you want
  different behavior or don't want this at all, consider overriding this in a node template for this node type.

- If you want to change or remove the links at the bottom of each feed node or feed item node, simply use
  Drupal's hook_link_alter <http://api.drupal.org/api/function/hook_link_alter/6> to alter them.
  
- Additional docs: http://drupal.org/node/310470



###   CHANGELOG   #############################################################################

3.x, xxxx-xx-xx
----------------------
- reset error on feed update, assuming the update addresses the error so it can be rechecked again on cron

3.1, 2008-10-07
----------------------
- #316086 - don't reimport expired feed items (bug fix, performance)
- improved messages for feed stats
- default feed items to expire "never" instead of 1 day (usability)
- improve stats for expiring feed items
- ignore feed items that will expire right away so they aren't added / deleted (performance)
- delete stats if feed is deleted

3.0, 2008-09-30
----------------------
- #273934 - new option to add rel="nofollow" to source links
- more sensible options for processing # of feeds per cron run
- #272482 - fix links to feed items that have entities in them
- new option to automatically download feed items when feed is created
- #314087 - new feed stats
- #297991 - move refresh/empty links to local tasks instead of links
- #301290 - improved performance with feeds throwing errors, new option to verify feeds at input
- #307686 - fix Feedburner feeds and improve performance of feed checking
- fix feeds that are not updated on cron runs because of sharing the same $feed object
- #269029 - fix taxonomy sometimes not assigned to feed items
- #282265 - fix feeds not refreshing new content on the expected interval

2.1, 2008-06-11
----------------------
- #240697 - prevent updated feed bodies from causing duplicates, uses more robust unique
            identifier for feed item, patch by mfer
- change URLs to be stored as TEXT so they can have unlimited length
  set max length in URL textfield to 1024 since Drupal defaults to 128
- #219834 - fix expiration of feed items

2.0, 2008-03-24
----------------------
- #193977 - fix views namespace conflicts with FeedAPI
- check_url() before sending to token module
- simplify feed refreshing, using $nid instead of $vid, and improving performance slightly
- #218084 - fix cache to work with both public and private file systems
- much more efficient routine for processing feeds
- add a feed hash to determine when a feed has been updated, improves performance significantly
- improve unique feed item id to simply be a hash of title & body, which is more robust and much
  faster to compute, removing costly serialize()
- #136395 - improve taxonomy support, making it faster and supporting regular taxonomies along with
  free tagging

1.0, 2007-11-16
----------------------
- Initial release
