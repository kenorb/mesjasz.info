// $Id: jquerymenu_no_animation.js,v 1.1 2009/01/05 13:14:21 aaronhawkins Exp $
$(document).ready(function(){
  $("ul.jquerymenu li.parent span.parent").click(function(){
    momma = $(this).parent();
    if ($(momma).hasClass('closed')){
      $(momma).removeClass('closed').addClass('open');
      $(this).removeClass('closed').addClass('open');
    }
    else{
      $(momma).removeClass('open').addClass('closed');
      $(this).removeClass('open').addClass('closed');
    }
  });
});