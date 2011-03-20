// $Id: thickbox_login.js,v 1.2.2.3 2008/09/23 07:49:20 frjo Exp $
// Contributed by user jmiccolis.
Drupal.behaviors.initThickboxLogin = function(context) {
  $("a[@href*='/user/login']").addClass('thickbox').each(function() { this.href = this.href.replace(/user\/login(%3F|\?)?/,"user/login/thickbox?height=230&width=250&") });
  $("a[@href*='?q=user/login']").addClass('thickbox').each(function() { this.href = this.href.replace(/user\/login/,"user/login/thickbox&height=230&width=250") });
}