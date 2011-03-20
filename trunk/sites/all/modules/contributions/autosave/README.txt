DESCRIPTION
===========
The Autosave module automatically saves a node after a period of time. Content types that can be autosaved as well as the period of time a node is autosaved, is configurable.

Autosaved nodes are are saved as a snap shot of the form.

NOTE: this version of autosave only works for single form (of selected node type) on a page.

This module is designed to work with Drupal 5.x.

DEPENDENCIES
============
Inlcudes the jQuery Form Plugin which was downloaded from here: http://dev.jquery.com/browser/trunk/plugins/form/jquery.form.js?format=txt

INSTALLATION
============
1. Place the "autosave" folder in your "modules" directory (i.e. modules/autosave).
2. Enable the Autosave module under Administer >> Site building >> Modules.
3. Under config for a node type select it to use Autosave.
5. Under Admin -> Site Config -> Autosave enter the period of time before each autosave (in milliseconds).

AUTHOR
======
Edmund Kwok (edmund.kwok [at] insyghtful.com)

CHANGE LOG
==========
Major fixes/modifications done by Peter Lindstrom sponsored by About.com

- fix issues with operation in IE7
- provide support for TinyMCE
- clean up autosave table after successful submit of node
- fix autosave validate bug on submit
- add support to enable autosave for specific node types
- fix issue of wrong form_id when VIEW->IGNORE