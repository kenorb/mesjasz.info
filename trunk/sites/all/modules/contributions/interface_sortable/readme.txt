*******************************************************
  README.txt for interface_sortable.module for Drupal
*******************************************************

The Interface Sortable module makes it easy for developers to add JavaScript
drag-and-drop form elements to forms and drag-and-drop widgets to non-form
pages.  It depends on the JQuery Interface module which in turn relies on
the JQuery Update module.

The Interface Sortable Demos module demonstrates the use of the Interface
Sortable module.  It can be turned off for production site that use the
Interface Sortable module.

INSTALLATION:

1. Put the interface_sortable directory in the modules directory that makes
   the most sense for your site.  See http://drupal.org/node/70151 for tips
   on where to install contributed modules.
2. Enable interface_sortable via admin/build/modules.
3. Optionally enable interface_sortable_demos via admin/build/modules.

TO DO:

1. Figure out why dragged items are shifted to the right in IE.
2. Clean up the Javascript.
3. Do we want to use hook_footer for sending out the Javascript, the way
   it's done in the messagefx module?

THANKS:

Thanks to jjeff for providing jquery_update and jquery_interface, upon
which this module depends.  This module is basically a Scriptaculous to
Interface port of jjeff's sortable work in Drupal 4.7.
