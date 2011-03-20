$Id: README.txt,v 1.4 2006/08/01 14:56:19 syscrusher Exp $

The Links Package is a set of modules that facilitate use of web links
(URLs) in various content types. Each link is carefully examined to put
it into a common format and then cataloged into a master database table
when it is first used. Later, references to the same link by other content
on the site are automatically connected to the same master catalog entry,
so that any particular unique URL appears in the database only once, no
matter how many times it is referred to by site content.



                         MODULE DESCRIPTIONS

LINKS

  links.module provides an application program interface (API) for managing
  internal and external links as part of a Drupal node. The module by itself
  does very little; mainly, it provides a user interface for controlling the
  way in which links are handled and how they behave within all nodes -- in
  other words, the global settings for the API library.

  Author:  Scott Courtney (drupal.org user "syscrusher")

  RELEASE STATUS: This module is at an initial release state, ready for
  early-adopter use in production.

LINKS_RELATED

  links_related.module allows nodes of arbitrary types (as designated by the
  site administrator) to have one or more associated links to related
  content.

  Author:  Scott Courtney (drupal.org user "syscrusher")

  RELEASE STATUS: This module is at an initial release state, ready for
  early-adopter use in production.

LINKS_WEBLINK

  links_weblink.module is a modified version of the existing weblink.module,
  but uses the new links API and database tables. links_weblink.module is
  also integrated with views.module, which allows very flexible retrieval
  of the links catalog. links_weblink.module provides its own simple query
  mechanism for sites with modest needs, but views.module offers a much
  more robust mechanism.

  RELEASE STATUS: This module is at an initial release state, ready for
  early-adopter use in production.

LINKS_ADMIN

  links_admin.module is for administering the site-wide database of links,
  independent of the module(s) using each link.

  RELEASE STATUS: This module is EXPERIMENTAL. Do not use in production
  sites. links_admin.module is not yet part of the standard download and
  is available only from CVS.


                           UPDATE SCRIPTS

To run the update scripts, you must first use the regular Drupal ugrade
procedure to convert your {node} table (and related tables) to version 4.6
The update scripts will NOT work with Drupal 4.5; neither will the links
package.

THE UPDATE SCRIPTS ARE EXPERIMENTAL -- Use at your own risk.

update-weblinks-old.php

  If you have weblink nodes that were created before Drupal 4.6, this is the
  update script that you need. It non-destructively copies data from the old
  {weblink} table into the new combination of {links} and {links_node}
  using the API calls to correctly normalize each URL. (It is *not* correct
  to simply use SQL commands to directly copy the URLs from the old table to
  the new ones.)

update-webstory.php

  The module "webstory" was developed for use on a few specific sites. Chances
  are about 99% that you are not using webstory.module and therefore do not
  need this script. It's provided as an example, however, in case it might
  be useful to other developers.
