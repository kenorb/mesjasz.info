<?php
// $Id: simplefeed.admin.inc,v 1.1.2.8 2008/10/07 06:25:56 m3avrck Exp $
/**
 * Administrative settings.
 */
function simplefeed_admin_settings() {
  $form['feed_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default feed settings'),
    '#description' => t('Default settings for each feed that is added. Note, that some of these settings can be overriden by editing any specific feed node.'),
    '#collapsible' => TRUE,
    '#access' => user_access('administer feeds'),
  );

  $period = array(0 => t('Never')) + drupal_map_assoc(array(3600, 10800, 21600, 32400, 43200, 86400, 172800, 259200, 604800, 1209600, 2419200, 4838400, 9676800), 'format_interval');
  $form['feed_settings']['simplefeed_expires'] = array(
    '#type' => 'select',
    '#title' => t('Discard feed items older than'),
    '#default_value' => variable_get('simplefeed_expires', 0),
    '#options' => $period,
    '#description' => t('Older feed items will be automatically discarded.  Requires !cron to be running.', array('!cron' => l('cron', 'admin/logs/status'))),
  );

  $period = array(0 => t('Never'), 1 => t('Manual')) + drupal_map_assoc(array(900, 1800, 3600, 7200, 10800, 21600, 32400, 43200, 64800, 86400, 172800, 259200, 604800), 'format_interval');
  $form['feed_settings']['simplefeed_refresh'] = array(
    '#type' => 'select',
    '#title' => t('Check feeds every'),
    '#description' => t('How often should a feed be checked for new content? If you select "manual", you can update a feed by clicking "refresh this feed" below the feed node. If you select a time, !cron must be running.', array('!cron' => l('cron', 'admin/logs/status'))),
    '#options' => $period,
    '#default_value' => variable_get('simplefeed_refresh', 3600),
  );

  foreach (filter_formats() as $id => $format) {
    $formats[$id] = $format->name;
  }
  $form['simplefeed_format'] = array(
    '#type' => 'select',
    '#title' => t('Default input format'),
    '#description' => t('The default input format for feeds (e.g., which HTML tags to not strip out).'),
    '#options' => $formats,
    '#default_value' => variable_get('simplefeed_format', 1),
  );

  // only if taxonomy is enabled will these options work
  if (module_exists('taxonomy')) {
    $vocabularies = taxonomy_get_vocabularies();
    $vocabularies_list = array(0 => 'None');
    foreach ($vocabularies as $vocabulary) {
      $vocabularies_list[$vocabulary->vid] = check_plain($vocabulary->name);
    }

    $form['simplefeed_vocab'] = array(
      '#type' => 'select',
      '#title' => t('Vocabulary'),
      '#description' => t("Select a vocabulary that both feeds and feed items share to have feed items automatically inherit their feed parent's terms when created. Select none to disable this feature."),
      '#options' => $vocabularies_list,
      '#default_value' => variable_get('simplefeed_vocab', 0),
    );
    $form['#validate'][] = 'simplefeed_settings_validate';

    $form['simplefeed_categories'] = array(
      '#type' => 'checkbox',
      '#title' => t('Automatically add categories set by external feeds to the vocabulary above.'),
      '#description' => t('This is useful when external feeds provide their own categories. These categories can then be merged into the main vocabulary defined above (If not using free-tagging, this will only associate with existing terms, it will not create new terms).'),
      '#default_value' => variable_get('simplefeed_categories', 0),
    );
  }

  $throttle = drupal_map_assoc(array(25, 50, 100, 250, 375, 500));
  $form['simplefeed_cron_throttle'] = array(
    '#type' => 'select',
    '#title' => t('Cron throttle'),
    '#description' => t('The number of feeds to parse each cron run.'),
    '#options' => $throttle,
    '#default_value' => variable_get('simplefeed_cron_throttle', 50),
  );

  $form['simplefeed_check_url'] = array(
   '#type' => 'select',
   '#title' => t('Verify feed before adding to database'),
   '#description' => t('This setting determines whether or not SimpleFeed should check to see if a feed exists at the URL provided. This can slow down adding feeds slightly since a feed has to be fetched and verified before the node can be created.'),
   '#options' => array(
     0 => t('No'),
     1 => t('Yes'),
   ),
   '#default_value' => variable_get('simplefeed_check_url', 1),
  );

  $form['simplefeed_download'] = array(
    '#type' => 'select',
    '#title' => t('Download feed items when feed is added'),
    '#description' => t('This setting determines whether feed items should be downloaded when a feed node is created. If set to "no", feed items will be downloaded on cron.'),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
    '#default_value' => variable_get('simplefeed_download', 0),
  );
  
  $form['simplefeed_nofollow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Spam link deterrent'),
    '#default_value' => variable_get('simplefeed_nofollow', 1),
    '#description' => t('If enabled, rel="nofollow" will be added to outgoing feed source links, as a measure to reduce the effectiveness of spam links. Note: this will also prevent valid links from being followed by search engines, therefore it is likely most effective when any user can add feeds.'),
  );   

  return system_settings_form($form);
}

/**
 * Validate the SimpleFeed settings.
 */
function simplefeed_settings_validate($form, $form_state) {
  // ensure that the vocab selected has been selected to be used by both feed and feed item node types
  if ($form_state['values']['simplefeed_vocab']) {
    $check = array();
    $types = db_query('SELECT type FROM {vocabulary_node_types} WHERE vid = %d', $form_state['values']['simplefeed_vocab']);
    while ($type = db_fetch_object($types)) {
      $check[] = $type->type;
    }

    if (!in_array('feed', $check) || !in_array('feed_item', $check)) {
      form_set_error('simplefeed_vocab', t('If you want to use this vocabulary, you need to associate it with both the <em>feed</em> and <em>feed item</em> node types.'));
    }
  }

  // you can't add categories to a non-existent vocab
  if ($form_state['values']['simplefeed_categories'] && !$form_state['values']['simplefeed_vocab']) {
    form_set_error('simplefeed_categories', t('You must specify a valid vocabulary if you want to automatically add categories.'));
  }
}
