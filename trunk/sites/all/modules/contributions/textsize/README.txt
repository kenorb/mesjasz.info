$Id: README.txt,v 1.1.1.1.2.2 2008/04/10 13:22:09 christianzwahlen Exp $

Drupal text size module
----------------------
Author - Christian Zwahlen info@zwahlendesign.ch
Requires - Drupal 5
License - GPL (see LICENSE.txt)

Description
-----------
This module display a text resizer in a block for better Web Accessibility.
Uniform menu name, based on the open source browser Firefox http://www.mozilla.com/en-US/firefox/
Firefox 2:
Text Size
* Increase Ctrl++
* Decrease Ctrl+-
* Normal Ctrl+0

Features
--------
* jQuery support.
* No Javascript required.
* Javascript (jQuery) support.
* No CSS required.
* Tested themes: Bluemarine, Garland.
* No images in the source code, only one link text (not the ALT text of images and the text of the links also).

Installation
------------
1. Copy the textsize modul to the Drupal directory "sites/all/modules/".
2. Go to "administer" -> "modules" and enable the module.
3. Check the "administer" -> "access control" page to enable use of
   this module to different roles.
4. Make sure the menu item is enabled in "administer" -> "menus".

Setup
-----
1. Go to "admin/settings/textsize"
2. Change the default normal text size (100%).
3. Go to "admin/settings/locale/language/import" and import your language file (for german "/modules/pagestyle/po/de.po").
4. Create your own icons, the source file is in the file "textsize_source.tar.gz" directory "/images/source",
  name it:
    "decrease.png",
    "increase.png",
    "normal.png",
  and copy in the directory "modules/textsize/images".
  The GIF images are for the Internet Explorer 6 or older.

Translations
------------
German
Improtieren sie die Datei "/modules/textsize/po/de.po". unter "admin/settings/locale/language/import" in die Deutsche Sprache

Other languages
Open the file "/modules/textsize/po/textsize.pot". in poEdit (http://www.poedit.net) or KBabel (http://kbabel.kde.org), translait the modul in your language and save the file in ""/modules/textsize/po/" as example "fr.po" for french language.

History
--------
1.x - For Drupal 5.x and Drupal 6.x
0.1.5 - 15.11.2007 New: Show the current text size, normal text size is first active, works on IE 6/7.
0.1.4 - 15.09.2007 New: More settings, language indipendend icon design, german translation,.
0.1.3.1 - 22.08.2007 New: Help page, fixed hover bug.
0.1.3 - 18.08.2007 New: New code, more settings, icons as background.
0.1.2 - 01.08.2007 New: Gnome 2.18 icons.
0.1.1 - 28.07.2007 New: Display the current page, Settings in Drupal: "Text Size Normal".
0.1.0 - 25.07.2007