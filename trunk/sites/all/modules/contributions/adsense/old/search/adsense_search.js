// $Id: adsense_search.js,v 1.3.2.2 2008/09/11 22:52:00 jcnventura Exp $

if (Drupal.jsEnabled) {
  $(document).ready(function () {
    // initiate farbtastic colorpicker
    var farb = $.farbtastic("#colorpicker");
    var firstField = "";

    $("input.form-text").each( function() {
      if (this.name.substring(0, 20) == "adsense_search_color") {
        if (firstField == "") {
          firstField = this;
        };

        farb.linkTo(this);
        $(this).click(function () {
          farb.linkTo(this);
        });
      };
    });

    farb.linkTo(firstField);
  });
}
