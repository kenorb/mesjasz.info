<?php
// $Id: simplenews-block.tpl.php,v 1.1.2.2 2008/09/21 13:52:03 sutharsan Exp $

/**
 * @file simplenews-block.tpl.php
 * Default theme implementation to display the simplenews block.
 * The form applies to one newsletter (series) identified by $tid
 *
 * Available variables:
 * - $subscribed: the current user is subscribed to the $tid newsletter
 * - $user: the current user is authenticated
 * - $tid: tid of the newsletter
 * - $message: announcement message (Default: 'Stay informed on our latest news!')
 * - $form: newsletter subscription form *1
 * - $subscription_link: link to subscription form at 'newsletter/subscriptions' *1
 * - $newsletter_link: link to taxonomy list of the newsletter issue *2
 * - $issuelist: list of newsletters (of the $tid newsletter series) *2
 * - $rssfeed: RSS feed of newsletter (series) *2
 * Note 1: requires 'subscribe to newsletters' permission
 * Note 2: requires 'view links in block' or 'administer newsletters' permission
 *
 * Simplenews module controls the display of the block content. The following
 * variables are available for this purpose:
 *  - $use_form       : TRUE = display the form; FALSE = display link to example.com/newsletter/subscriptions
 *  - $use_issue_link : TRUE = display link to newsletter issue list
 *  - $use_issue_list : TRUE = display list of the newsletter issue
 *  - $use_rss        : TRUE = display RSS feed
 *
 * @see template_preprocess_simplenews_block()
 */
?>
  <?php if ($message): ?>
    <p><?php print $message; ?></p>
  <?php endif; ?>

  <?php if ($use_form): ?>
    <?php print $form; ?>
  <?php elseif ($subscription_link): ?>
    <p><?php print $subscription_link; ?></p>
  <?php endif; ?>

  <?php if ($use_issue_link && $newsletter_link): ?>
    <div class="issues-link"><?php print $newsletter_link; ?></div>
  <?php endif; ?>

  <?php if ($use_issue_list && $issue_list): ?>
    <div class="issues-list"><?php print $issuelist; ?></div>
  <?php endif; ?>

  <?php if ($use_rss): ?>
    <?php print $rssfeed; ?>
  <?php endif; ?>
