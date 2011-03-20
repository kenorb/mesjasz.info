// $Id: jquerymenu.js,v 1.4 2008/12/28 19:26:22 aaronhawkins Exp $
$(document).ready(function(){
  $("ul.jquerymenu li.parent span.parent").click(function(){
    momma = $(this).parent();
    if ($(momma).hasClass('closed')){
      $($(this).siblings('ul').children()).hide().fadeIn('3000');
      $(momma).children('ul').slideDown('700');
      $(momma).removeClass('closed').addClass('open');
      $(this).removeClass('closed').addClass('open');
    }
    else{
      $(momma).children('ul').slideUp('700');
      $($(this).siblings('ul').children()).fadeOut('3000');
      $(momma).removeClass('open').addClass('closed');
      $(this).removeClass('open').addClass('closed');
    }
  });
});