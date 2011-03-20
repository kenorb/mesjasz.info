// $Id: uc_country_select.js,v 1.6.2.1 2008/10/15 14:47:39 islandusurper Exp $

/**
 * Set the select box change behavior for the country selector
 */
Drupal.behaviors.ucCountrySelect = function(context) {
  $('select[@id$=-country]:not(.ucCountrySelect-processed)', context).addClass('ucCountrySelect-processed').change(
    function() {
      uc_update_zone_select(this.id, '');
    }
  );
}

function uc_update_zone_select(country_select, default_zone) {
  var zone_select = country_select.substr(0, country_select.length - 8) + '-zone';

  var options = { 'country_id' : $('#' + country_select).val() };

  $('#' + zone_select).parent().siblings('.zone-throbber').attr('style', 'background-image: url(' + Drupal.settings.basePath + 'misc/throbber.gif); background-repeat: no-repeat; background-position: 100% -20px;').html('&nbsp;&nbsp;&nbsp;&nbsp;');

  $.post(Drupal.settings.basePath + '?q=uc_js_util/zone_select', options,
         function (contents) {
           if (contents.match('value="-1"') != null) {
             $('#' + zone_select).attr('disabled', 'disabled');
           }
           else {
             $('#' + zone_select).removeAttr('disabled');
           }
           $('#' + zone_select).empty().append(contents).val(default_zone).change();
           $('#' + zone_select).parent().siblings('.zone-throbber').removeAttr('style').empty();
         }
  );
}
