$Id: README.txt,v 1.5.2.5 2008/08/19 19:59:34 sutharsan Exp $

DESCRIPTION
-----------

Simplenews publishes and sends newsletters to lists of subscribers. Both
anonymous and authenticated users can opt-in to different mailing lists. 
HTML email can be send by adding Mime mail module.


REQUIREMENTS
------------

 * Taxonomy module
 * For large mailing lists, cron is required.
 * HTML-format newsletters and/or newsletters with file attachments require the
   mimemail module. 

INSTALLATION
------------

 1. CREATE DIRECTORY

    Create a new directory "simplenews" in the sites/all/modules directory and
    place the entire contents of this simplenews folder in it.

 2. ENABLE THE MODULE

    Enable the module on the Modules admin page:
      Administer > Site building > Modules

 3. ACCESS PERMISSION

    Grant the access at the Access control page:
      Administer > User management > Access control. 

 4. CONFIGURE SIMPLENEWS

    Configure Simplenews on the Simplenews admin pages:
      Administer > Site configuration > Simpelnews.

    Select the content type Simplenews uses for newsletters.
    Select the taxonomy term Simplenews uses to manage newsletter series.

 5. ENABLE SIMPLENEWS BLOCK

    With the Simpelenews block users can subscribe to a newsletter. For each
    newsletter one block is available.
    Enable the Simplenews block on the Administer blocks page:
      Administer > Site building > Blocks.

 6. CONFIGURE SIMPLENEWS BLOCK

    Configure the Simplenews block on the Block configuration page. You reach
    this page from Block admin page (Administer > Site building > Blocks). 
    Click the 'Configure' link of the appropriate simplenews block.
 
    Permission "subscribe to newsletters" is required to view the subscription
    form in the simplenews block or to view the link to the subscription form.
    Links in the simplenews block (to previous issues and RSS-feed) are only
    displayed to users who have "view links in block" privileges.

 7. SIMPLENEWS BLOCK THEMING

    More control over the content of simplenews blocks can be achieved using 
    the block theming. Theme your simplenews block by copying 
    simplenews-block.tpl.php into your theme directory and edit the content.
    The file is self documented listing all available variables.

 8. Multilingual support
 
    Simplenews supports multilingual newsletters for node translation,
    multilingual taxonomy and url path prefixes.

    When translated newsletter issues are avialable subscribers revieve the
    newsletter in their preferred language (according to account setting).
    Translation module is required for newsletter translation.

    Multilingual taxonomy of 'Localized terms' and 'per language terms' is
    supported. 'per language vocabulary' is not supported.
    I18ntaxonomy module is required.
    Use 'Localized terms' for a multilingual newsletter. Taxonomy terms are
    translated and translated newsletters are each taged with the same
    (translated) term. Subscribers reveive the newsletter in the preferred
    language set in their account settings or in the site default language.
    Use 'per language terms' for mailinglists each with a different language.
    Newsletters of different language each have their own tag and own list of
    subscribers.
    
    Path prefixes are added to footer message according to the subscribers
    preferred language.

    The preferred language of anonymous users is set based on the interface
    language of the page they visit for subscription. Anonymous users can NOT
    change their preferred language. Users with an account on the site will be
    subscribed with the preferred language as set in their account settings.

9. Send mailing lists

    Cron is required to send large mailing lists. Cron jobs can be triggered
    by Poormanscron or any other cron mechanisme such as crontab.
    If you have a medium or large size mailinglist (i.e. more than 500
    subscribers) always use cron to send the newsletters.
  
    To use cron:
     * Check the 'Use cron to send newsletters' checkbox.
     * Set the 'Cron throttle' to the number of newsletters send per cron run.
       Too high values will lead to the warning message 'Attempting to re-run
       cron while it is already running'
    
    Don't use cron:
     * Uncheck the 'Use cron to send newsletters' checkbox.
       All newsletters will be send immediately when saving the node.

    These settings are found on the Newsletter Settings page under
    'Send mail' options at:
      Administer > Site configuration > Simpelnews > Send mail.

 10. TIPS

    A subscription page is available at: /newsletter/subscriptions

DOCUMENTATION
-------------
More help can be found on the help pages: example.com/admin/help/simplenews
and in the drupal.org handbook: http://drupal.org/project/node/197057
