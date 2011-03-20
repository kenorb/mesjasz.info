
var form_ids;
var configure_autosave = false;
var position = 0;
var autosaved_forms;

Drupal.attachAutosave = function() {
  setTimeout('Drupal.saveForm()', Drupal.settings['period'] * 1000);
}

Drupal.saveForm = function() {
    if (typeof tinyMCE == 'object') {
      editorBookmark = tinyMCE.selectedInstance.selection.getBookmark();
      editorIsDirty=true;
      tinyMCE.triggerSave();
    }
    
  //for (var i = 0; i < form_ids.length; i++) {
    form_id = Drupal.settings['form_id'].replace(/_/g, '-');
    //else form_id = Drupal.settings['form_id'][i].replace(/_/g, '-');
    serialized = $('#' + form_id).formHash();
    //serialized['q'] =  Drupal.settings['q'][0];
    serialized['q'] =  Drupal.settings['q'];
    $.ajax({
      url: Drupal.settings['autosave_url'],
      type: "POST",
      dataType: "xml/html/script/json",
      data: serialized,
      complete: function() {
        $('#autosave-status #status').html('Form autosaved.');
        $('#autosave-status #operations').css('display', 'none').css('visibility', 'hidden');
        $('#autosave-status').slideDown();
        setTimeout("$('#autosave-status').fadeOut('slow')", 3000);
      }
    });
  //}
    if (typeof tinyMCE == 'object') {
      tinyMCE.selectedInstance.selection.moveToBookmark(editorBookmark);
    }
    setTimeout('Drupal.saveForm()', Drupal.settings['period'] * 1000);
}

Drupal.configureAutosave = function() {
  $('form').each(function(index) {
    id = $(this).attr('id');
    fieldset = document.createElement('fieldset');
    fieldset.id = 'autosave-' + id;
    elem_id = '#autosave-' + id;
    legend = document.createElement('legend');
    $(this).wrap(fieldset);
    $(legend).html('<span style="color: green; font-weight: bold">Autosave enabled</span>');
    $(elem_id).prepend(legend);
    $(elem_id).attr('autosave', 'true');
    
    $(elem_id).click(function() {
      id = $('form', this).attr('id');
      if ($(this).attr('autosave') == 'true') {
        $('form', this).css('opacity', '0.2');
        $('legend', this).html('<span style="color: #FFAE00; font-weight: bold">Autosave disabled</span>');
        $(this).attr('autosave', 'false');
      }
      else if ($(this).attr('autosave') == 'false') {
        $('form', this).css('opacity', '1');
        $('form', this).css('opacity', '');
        $('legend', this).html('<span style="color: green; font-weight: bold">Autosave enabled</span>');
        $(this).attr('autosave', 'true');
      }
    });
  });
}

Drupal.checkArrowsStatus = function(length, pos) {
  pos += 1;
  if (pos == 1) {
    $('#left-arrow').removeAttr('class').attr('class', 'arrow disabled').unbind('click');
    $('#right-arrow').removeAttr('class').attr('class', 'arrow enabled').unbind('click').bind('click', function() { Drupal.rightArrowCallback(length, pos) });
  }
  else if (pos < length) {
    $('#left-arrow').removeAttr('class').attr('class', 'arrow enabled').unbind('click').bind('click', function() { Drupal.leftArrowCallback(length, pos)});
    $('#right-arrow').removeAttr('class').attr('class', 'arrow enabled').unbind('click').bind('click', function() { Drupal.rightArrowCallback(length) });
  }
  else if (length == pos) {
    $('#left-arrow').removeAttr('class').attr('class', 'arrow enabled').unbind('click').bind('click', function() { Drupal.leftArrowCallback(length) });
    $('#right-arrow').removeAttr('class').attr('class', 'arrow disabled').unbind('click');
  }
}

Drupal.leftArrowCallback = function(length) {
  position -= 1;
  //$('#autosave-status #status').html('This form (' + autosaved_forms[position]['form_id'] + ') was autosaved on ' + autosaved_forms[position]['saved_date']);
  $('#autosave-status #status').html('This form was autosaved on ' + autosaved_forms[position]['saved_date']);
  $('#autosave-status').fadeIn();
  $('#autosave-status #view a').html('View');
  Drupal.checkArrowsStatus(length, position);
}

Drupal.rightArrowCallback = function(length) {
  position += 1;
  //$('#autosave-status #status').html('This form (' + autosaved_forms[position]['form_id'] + ') was autosaved on ' + autosaved_forms[position]['saved_date']);
  $('#autosave-status #status').html('This form was autosaved on ' + autosaved_forms[position]['saved_date']);
  $('#autosave-status').fadeIn();
  $('#autosave-status #view a').html('View');
  Drupal.checkArrowsStatus(length, position);
}

if (Drupal.jsEnabled) {
  $(document).ready(function() {
    if (configure_autosave) {
      Drupal.configureAutosave();
    }
    else {
      form_ids = Drupal.settings['form_id'];
      $('body').append('<div id="autosave-status"><span id="status"></span><span id="operations"> \
      <span id="view"><a href="#">View</a></span> \
      <span id="ignore"><a href="#" title="Ignore/Delete Saved Form">Ignore</a></span> \
      <span id="keep"><a href="#" title="Keep Saved Form - Revert to Saved">Keep</a></span></span></div>');
      autosaved_forms = Drupal.settings['autosaved_form'];
      if (autosaved_forms) {
        // $('#autosave-status #keep a').html(''); 
        $('#autosave-status #keep').css('display', 'none').css('visibility', 'hidden');
	      // Check if there are more than one autosaved form in the page.
        if (autosaved_forms.length > 1) {
          // More than one autosaved form in the page, add navigation arrows.
          $('#autosave-status').append('<div id="left-arrow" class="arrow disabled"></div>');
          $('#autosave-status').append('<div id="right-arrow" class="arrow enabled"></div>');
          
          Drupal.checkArrowsStatus(autosaved_forms.length, position);
        }
        $('#autosave-status #view a').click(function() {
          autosaved_form_id = autosaved_forms[position]['form_id'].replace(/_/g, '-');
          if ($(this).html() == 'View') {
            $('#' + autosaved_form_id).formHash(autosaved_forms[position]['serialized']);
            $('#' + autosaved_form_id).focus();
            $(this).html('Reset');
            $('#autosave-status #keep').css('display', 'inline').css('visibility', 'visible');
            $('#autosave-status #keep a').html('Keep'); 
          }
          else if ($(this).html() == 'Reset') {
            form = document.getElementById(autosaved_form_id);
            form.reset();
            if (typeof tinyMCE == 'object') $('#' + autosaved_form_id).formHash($('#' + autosaved_form_id).formHash());  
            //$('#autosave-status #keep a').html('');
            $('#autosave-status #keep').css('display', 'none').css('visibility', 'hidden');
            $(this).html('View');
          }    
          return false;
        });
        $('#autosave-status #ignore a').click(function() {
          autosaved_form_id = autosaved_forms[position]['form_id'].replace(/_/g, '-');
          $('#autosave-status').fadeOut('slow');
          form = document.getElementById(autosaved_form_id);
          form.reset();
          $('#autosave-status #operations').css('display', 'none').css('visibility', 'hidden');
          $('#autosave-status #left-arrow').css('display', 'none').css('visibility', 'hidden');
          $('#autosave-status #right-arrow').css('display', 'none').css('visibility', 'hidden');
          Drupal.attachAutosave();
          return false;
        });
        $('#autosave-status #keep a').click(function() {
          autosaved_form_id = autosaved_forms[position]['form_id'].replace(/_/g, '-');
          $('#autosave-status').fadeOut('slow');
          form = document.getElementById(autosaved_form_id);
          $('#autosave-status #operations').css('display', 'none').css('visibility', 'hidden');
          $('#autosave-status #left-arrow').css('display', 'none').css('visibility', 'hidden');
          $('#autosave-status #right-arrow').css('display', 'none').css('visibility', 'hidden');
          Drupal.attachAutosave();
          return false;
        });
        //$('#autosave-status #status').html('This form (' + autosaved_forms[0]['form_id'] + ') was autosaved on ' + autosaved_forms[0]['saved_date']);
        $('#autosave-status #status').html('This form was autosaved on ' + autosaved_forms[0]['saved_date']);
        $('#autosave-status').slideDown();
      }
      // There are no autosaved forms, continue with autosave.
      else {
        Drupal.attachAutosave();
      }
    }
  });
} 
