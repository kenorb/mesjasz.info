/* $Id: weblinks.admin.js,v 1.1.4.2 2008/08/31 18:20:55 rmiddle Exp $ */

function weblinks_urlnode_handler(event) {
  if ($("input[@name=weblinks_urlnode]:checked").val() == 'url') {
    $("div.weblinks_external_hide").show();
  }
  else {
    $("div.weblinks_external_hide").hide();
  }
}

function weblinks_collapse_handler(event) {
  if ($("input[@name=weblinks_collapsible]:checked").val() == 1) {
    $("div.weblinks_collapse_hide").show();
  }
  else {
    $("div.weblinks_collapse_hide").hide();
  }
}

function weblinks_checker_handler(event) {
  if ($("input[@name=weblinks_checker_enabled]:checked").val() == 1) {
    $("div.weblinks_checker_hide").show();
    $("#edit-weblinks-checker-enabled-wrapper label").css({fontWeight:"normal" });
  }
  else {
    $("div.weblinks_checker_hide").hide();
    $("#edit-weblinks-checker-enabled-wrapper label").css({fontWeight:"bold" });
  }
}

function weblinks_unclassified_handler(event) {
  if ($("input[@name=weblinks_unclassified_title]").val() != '') {
    $("div.weblinks_unclassified_hide").show();
  }
  else {
    $("div.weblinks_unclassified_hide").hide();
  }
}

function weblinks_link_display_handler(event) {
  // Node view "url" or "visit".
  if ($("input[@name=weblinks_view_as]:checked").val() == 'url') {
    $("div.weblinks_trim_hide").show();
    $("div.weblinks_visit_hide").hide();
  }
  else {
    $("div.weblinks_trim_hide").hide();
    $("div.weblinks_visit_hide").show();
  }
}

function weblinks_setup() {
  // Collapse the fieldsets, bottom to top.
  Drupal.toggleFieldset('#weblinks-view-settings');
  Drupal.toggleFieldset('#weblinks-link-settings');
  Drupal.toggleFieldset('#weblinks-group-settings');
  Drupal.toggleFieldset('#weblinks-page-settings');
}

// Run the javascript on page load.
if (Drupal.jsEnabled) {
  $(document).ready(function () {
  // On page load, determine the default settings.
  weblinks_urlnode_handler();
  weblinks_collapse_handler();
  weblinks_checker_handler();
  weblinks_unclassified_handler();
  weblinks_link_display_handler();

  // Bind the functions to click events.
  $("input[@name=weblinks_urlnode]").bind("click", weblinks_urlnode_handler);
  $("input[@name=weblinks_collapsible]").bind("click", weblinks_collapse_handler);
  $("input[@name=weblinks_checker_enabled]").bind("click", weblinks_checker_handler);
  $("input[@name=weblinks_unclassified_title]").bind("click", weblinks_unclassified_handler);
  $("input[@name=weblinks_view_as]").bind("click", weblinks_link_display_handler);
  
  weblinks_setup();
  });
}