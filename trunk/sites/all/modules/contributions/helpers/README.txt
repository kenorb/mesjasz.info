README for helpers module.

h2. AUTHORS
 Bèr Kessels | www.webschuur.com
 Nancy Wichmann | nanwich.info
 (please add yourself if you contributed)


h2. ABOUT
"Helpers?" I hear you ask...
 A helper would, for example be: number_to_currency(), or distance_of_time_in_words(). Or off course something like select_year, or even select_country.
Basically all kinds of interesting and extremly usefull functions that are too specific to make it into core, but still very handy to have at hand. Stuff that you have wondered that "would be very usefull in addition to the default PHP",
 How would it be to call a $form['country'] = select_country() and get a "select your country" select, prefilled with all the known countries in the world? Or to have a ready made function to render any number as a currency? Or even to render any percentage in a star-rating piece of HTML (render_rate_as_stars()) etc.
This is part of my abovementioned plan to "bring more Ruby on Rails delight to Drupal". And most of all to make Drupal a developer friendly environment (where developing no longer requires you to think about the folks that might put the currency symbol behind the value... But where that is simply a case of calling a proper function). More at: http://drupal.org/node/61397#comment-117091

h2. HOW TO CONTRIBUTE
Some notes on contributing.
We don't allow just anyone to write into this module, because that would undermine the goal "creating a good and helpfull toolbox". So please upload patches for this module.
Once we have seen your contributions, and we like them, we will agree on you writing directly to the module. Obviously you'd need write access first!

h2. CODING STANDARDS
All applications must folliow the Drupal coding standards. But we want something more:
* Name your functions very clear. theme_box is not as good as theme_general_block_element. convert_currency is far less descriptive then convert_currency_to_decimal. Be creative, and look at the other functions for inspiration.
* No need to include the work "helper" in your functions. Allthough Drupal has a guideline to name all the functions "module_name_foo_bar" we break this. Helpers must be usefull, not finger-breaking-complex to type. Hence we prefer theme_read_more above theme_helpers_read_more.
* Remove functions that went into core. I beleive that a lot of functions from the helpers might make it into core. Once they are in, please remove them from this module.
* Link to issues. If a patch in the queue tries to insert a function from this module into core, provide a link to that issue-thread.
* Doxygen. Each function MUST have documentation. A change to a function, REQUIRES you to change the docs too.