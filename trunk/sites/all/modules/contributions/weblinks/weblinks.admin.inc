<?php
// $Id: weblinks.admin.inc,v 1.34.2.37 2008/12/29 00:59:08 nancyw Exp $

/**
 * @file
 * Settings form.
 */

/**
 * Module settings page.
 */
function weblinks_settings_page() {
  return drupal_get_form('weblinks_admin_settings');
}
 
function weblinks_admin_settings() {
  drupal_add_js(drupal_get_path('module', 'weblinks') .'/weblinks.admin.js');

/**
 * Overall settings.
 */

  $vocabs = taxonomy_get_vocabularies('weblinks');
  if (count($vocabs) > 1) {
    $avail_vocs = array();
    foreach ($vocabs as $vid => $vocabulary) {
      $avail_vocs[$vid] = check_plain($vocabulary->name);
    }
    ksort($avail_vocs);
    $form['weblinks_nav_vocabulary'] = array(
      '#type' => 'radios',
      '#title' => t('Vocabulary to use for Weblinks'),
      '#default_value' => variable_get('weblinks_nav_vocabulary', ''),
      '#options' => $avail_vocs,
      '#required' => TRUE,
      '#prefix' => '<div class="weblinks-radios">',
      '#suffix' => '</div><div class="clear-block"></div>',
      );
  }
  else {
    if (count($vocabs) < 1) {
      variable_del('weblinks_nav_vocabulary');
      _weblinks_get_vocid();
      drupal_set_message(t('No vocabulary for the weblinks content type was found. A new vocabulary was created.'));
    }
  }

  $len_opts = drupal_map_assoc(array(0, 10, 15, 20, 30, 40, 50, 60, 70, 80, 128, 256));

  $form['weblinks_trim_block'] = array(
    '#type' => 'select',
    '#options' => $len_opts,
    '#title' => t('Maximum block title length'),
    '#default_value' => variable_get('weblinks_trim_block', 0),
    '#description' => t('This controls how much of a node title will be shown in a block to conserve space. Zero (0) means unlimited.'),
    );

    $form['page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links page settings'),
    '#description' => t('These settings control the main links page display.'),
    '#attributes' => array('id' => 'weblinks-page-settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );

/**
 * Settings for the links page.
 */
  $form['page']['weblinks_collapsible'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make groups collapsible'),
    '#default_value' => variable_get('weblinks_collapsible', TRUE),
    '#description' => t('Enable this if you want the groups to be collapsible on the main links page.'),
    '#required' => FALSE,
    );

  $form['page']['weblinks_collapsed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapse all groups'),
    '#default_value' => variable_get('weblinks_collapsed', FALSE),
    '#description' => t('Enable this if you want all the groups to be collapsed on the main links page.'),
    '#required' => FALSE,
    '#prefix' => '<div class="weblinks_collapse_hide">',
    '#suffix' => '</div>',
    );

  $sort_options = array(
    'standard' => t('Normal Drupal - by weight, sticky, created date.'),
    'title' => t('Legacy - by weight and title.'),
    );
  $form['page']['weblinks_page_sort'] = array(
    '#type' => 'radios',
    '#options' => $sort_options,
    '#title' => t('Page Sort order'),
    '#default_value' => variable_get('weblinks_page_sort', 'title'),
    '#description' => t('This determines how the links page will sort the links.'),
    '#required' => FALSE,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div><div class="clear-block"></div>',
    );

  $form['page']['weblinks_pagedesc'] = array(
    '#type' => 'textarea',
    '#title' => t('Page information'),
    '#rows' => 3,
    '#default_value' => variable_get('weblinks_pagedesc', NULL),
    '#description' => t('This description will appear at the top of the links page.  It will be filtered by your default input format.'),
    '#required' => FALSE,
    );

  // Disallow blank setting.
  $mfd_default = variable_get('weblinks_maxfrontdepth', 1);
  if (!is_numeric($mfd_default)) {
    $mfd_default = 1;
  }
  $form['page']['weblinks_maxfrontdepth'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum group depth (main links page)'),
    '#default_value' => $mfd_default,
    '#description' => t('This controls how many subgroups, if any, will display on the main links page. Blank, zero (0), and one (1) will all show only the top level.'),
    '#required' => FALSE,
    '#size' => 2,
    );

/**
 * Settings for group display.
 */
  $form['group'] = array(
    '#type' => 'fieldset',
    '#title' => t('Group settings'),
    '#description' => t('These settings control the way the groups display on the main links page.'),
    '#attributes' => array('id' => 'weblinks-group-settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );

  $form['group']['weblinks_catdesc'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display group descriptions'),
    '#default_value' => variable_get('weblinks_catdesc', TRUE),
    '#description' => t('Generally recommended.'),
    );

  $form['group']['weblinks_empty_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Empty container text'),
    '#rows' => 2,
    '#default_value' => variable_get('weblinks_empty_text', NULL),
    '#description' => t('This text will appear when a container is empty. Leave it blank to not show the group at all. It will be filtered by your default input format.'),
    );

  $form['group']['weblinks_unclassified_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title for unclassified links'),
    '#default_value' => variable_get('weblinks_unclassified_title', t('Unclassified')),
    '#description' => t('This will be used to describe the unclassified links. You may leave it blank for no title. It will not be translated.'),
    );

  $form['group']['weblinks_unclassified_desc'] = array(
    '#type' => 'textfield',
    '#title' => t('Description for unclassified links'),
    '#default_value' => variable_get('weblinks_unclassified_desc', t('These links have not been assigned a group.')),
    '#description' => t('This will be used as the description for the unclassified links group. If this group has no title, this will not be shown either. It will not be translated.'),
    '#prefix' => '<div class="weblinks_unclassified_hide">',
    '#suffix' => '</div>',
    );

/**
 * Settings for link display.
 */
  $form['link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link settings'),
    '#description' => t('These settings control the way individual links display on the main links page.'),
    '#attributes' => array('id' => 'weblinks-link-settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );

  $form['link']['weblinks_urlnode'] = array(
    '#type' => 'radios',
    '#title' => t('Links lead to URL or Web Link node'),
    '#default_value' => variable_get('weblinks_urlnode', 'url'),
    '#description' => t('Most people will want to leave this as URL. Specifying "node" makes it easier to edit the links, but requires an additional click to follow the link.'),
    '#options' => array('url' => t('URL'), 'node' => t('node')),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['link']['weblinks_external'] = array(
    '#type' => 'checkbox',
    '#title' => t('Open links in a new window'),
    '#default_value' => variable_get('weblinks_external', TRUE),
    '#description' => t('Recommended to keep your vistors on your site. Should not be used when the "Links lead..." option is "node."'),
    '#prefix' => '<div class="weblinks_external_hide">',
    '#suffix' => '</div>',
    );

  $desc_opts = array(
    'none' => t('No description'),
    'teaser' => t('Teaser length'),
    'body' => t('Full length'),
    );
  $form['link']['weblinks_linkdesc'] = array(
    '#type' => 'radios',
    '#options' => $desc_opts,
    '#title' => t('Link description display'),
    '#default_value' => variable_get('weblinks_linkdesc', 'teaser'),
    '#description' => t('This sets how the description will be displayed on the main links page.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $form['link']['weblinks_show_url'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display URL?'),
    '#default_value' => variable_get('weblinks_show_url', FALSE),
    '#description' => t('Do you want to include the actual URL on the links page?'),
    );

  $form['link']['weblinks_linkinfo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display link information?'),
    '#default_value' => variable_get('weblinks_linkinfo', TRUE),
    '#description' => t('This displays link author, publish date, and revision date'),
    );

  $form['link']['weblinks_user_links'] = array(
    '#type' => 'checkbox',
    '#title' => t("Display a link to other users' links?"),
    '#default_value' => variable_get('weblinks_user_links', FALSE),
    '#description' => t("Shows a link to another user's links."),
    );

  $form['link']['weblinks_comment_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display comment information?'),
    '#default_value' => variable_get('weblinks_comment_links', FALSE),
    '#description' => t('Shows links for adding and viewing comments.'),
    );

  $form['link']['weblinks_nofollow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Spam link deterrent?'),
    '#default_value' => variable_get('weblinks_nofollow', FALSE),
    '#description' => t('If enabled, Drupal will add rel="nofollow" to all links, as a measure to reduce the effectiveness of spam links. Note: this will also prevent valid links from being followed by search engines, therefore it is likely most effective when enabled for anonymous users.'),
    );

/**
 * Settings for node_view.
 */
  $form['view'] = array(
    '#type' => 'fieldset',
    '#title' => t('View settings'),
    '#description' => t('These settings control the way individual links display on the node view page.'),
    '#attributes' => array('id' => 'weblinks-view-settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );

  $form['view']['weblinks_view_as'] = array(
    '#type' => 'radios',
    '#title' => t('Link displays as'),
    '#options' => array('url' => t('URL'), 'visit' => t('Visit')),
    '#default_value' => variable_get('weblinks_view_as', 'url'),
    '#description' => t('This option sets whether the URL itself will be displayed or will hide behind a "Visit <em>title</em>" link.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  $token = module_exists('token');
  $form['view']['weblinks_visit_text'] = array(
    '#type' => 'textfield',
    '#title' => t('"Visit" text'),
    '#default_value' => variable_get('weblinks_visit_text', t('Visit [title]')),
    '#description' => t('This is the text that will be used with the node title (indicated by "[title]") to invite a click.'),
    '#prefix' => '<div class="weblinks_visit_hide">',
//    '#suffix' => '</div>',
//    '#disabled' => !$token,
    );

  if ($token) {
    $form['view']['token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
//      '#description' => t('Prefer raw-text replacements for text to avoid problems with HTML entities!'),
      );

    $form['view']['token_help']['help'] = array(
      '#value' => theme('token_help', 'node'),
      );
  }

  // The end div here ends the "weblinks_visit_hide" above.
  $form['view']['weblinks_trim'] = array(
    '#type' => 'select',
    '#options' => $len_opts,
    '#title' => t('Maximum link title length'),
    '#default_value' => variable_get('weblinks_trim', 0),
    '#description' => t('This controls how much of long URLs will be shown in a link in full node view. Zero (0) means unlimited.'),
    '#prefix' => '</div><div class="weblinks_trim_hide">',
    );

  $form['view']['weblinks_strip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Strip protocol and "www"?'),
    '#default_value' => variable_get('weblinks_strip', FALSE),
    '#description' => t('If enabled, the protocol (e.g. "http://") and "www" (if present) will be removed before displaying the URL.'),
    '#suffix' => '</div>',
    );

  $form['view']['weblinks_view_position'] = array(
    '#type' => 'radios',
    '#title' => t('Link position on view'),
    '#options' => array(-1 => t('above'), 1 => t('below')),
    '#default_value' => variable_get('weblinks_view_position', -1),
    '#description' => t('This option sets whether the link itself will be displayed above or below the description when viewing the node.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

/**
 * Settings for field defaults.
 */
  $form['field'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field defaults'),
    '#description' => t("These settings provide various defaults for fields that may be entered. Be careful when changing these as they can interfere with a user's input."),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    );

  $form['field']['weblinks_url_stub'] = array(
    '#type' => 'textfield',
    '#title' => t('Default for URL'),
    '#default_value' => variable_get('weblinks_url_stub', 'http://'),
    '#description' => t('This will be used as a stub in the URL field.'),
    );

  $form['field']['weblinks_body_stub'] = array(
    '#type' => 'textfield',
    '#title' => t('Default for description'),
    '#default_value' => variable_get('weblinks_body_stub', ''),
    '#description' => t('This will be used as a stub in the "Link description" field.'),
    );

  $format_list = filter_formats();
  $formats = array(FILTER_FORMAT_DEFAULT => '-default-');
  foreach ($format_list as $number => $filter) {
    $formats[$number] = $filter->name;
  }
  $form['field']['weblinks_format'] = array(
    '#type' => 'radios',
    '#options' => $formats,
    '#title' => t('Default input format'),
    '#default_value' => variable_get('weblinks_format', FILTER_FORMAT_DEFAULT),
    '#description' => t('This will be used as the default input format.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div>',
    );

  return system_settings_form($form);
}

function weblinks_group_settings($form_state) {
  $tree = weblinks_get_tree(0);
  $rows = $form = array();
  $form = array('#tree' => TRUE);

  $sort_options = array(
    '' => t('Default.'),
    'standard' => t('Normal Drupal - by weight, sticky, created date.'),
    'title' => t('Legacy - by weight and title.'),
    );

  foreach ($tree as $term) {
    $form[$term->tid]['tid'] = array(      
      '#type' => 'value',
      '#value' => $term->tid,
      );

    $form[$term->tid]['enable'] = array(      
      '#type' => 'checkbox',
      '#title' => '<strong>'. str_repeat('&emsp;&emsp;', $term->depth) . t('Show %name', array('%name' => check_plain($term->name))) .'</strong>',
      '#default_value' => variable_get('weblinks_page_'. $term->tid, TRUE),
      '#description' => t('Check to display this group on the main links page.'),
      );

    $form[$term->tid]['collapse'] = array(      
      '#type' => 'checkbox',
      '#title' => str_repeat('&emsp;&emsp;', $term->depth) . t('Collapse by default'),
      '#default_value' => variable_get('weblinks_collapse_'. $term->tid, FALSE),
      '#description' => t('Check to collapse this group on the main links page by default.'),
      '#prefix' => '<div class="weblinks-indented">',
      '#suffix' => '</div>',
      );

    // Set the maximum number of links to display for each group
    $form[$term->tid]['maxdisp'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum links to display'),
      '#size' => 10,
      '#default_value' => variable_get('weblinks_maxdisp_'. $term->tid, ''),
      '#description' => t('Leave blank if you would not like to limit the number of links displayed'),
      );

    $form[$term->tid]['block'] = array(      
      '#type' => 'checkbox',
      '#title' => t('Create block'),
      '#default_value' => variable_get('weblinks_create_block_'. $term->tid, TRUE),
      '#description' => t('Check to allow a block for this group to be created.'),
      );

    if ($term->depth == 0 && $term->tid != 0) {
      $form[$term->tid]['allow_groups'] = array(      
        '#type' => 'checkbox',
        '#title' => t('Block by groups'),
        '#default_value' => variable_get('weblinks_block_allow_groups_'. $term->tid, FALSE),
        '#description' => t('Allow Random, Recent, and Popular block by groups. Requires "Create block."'),
        );
    }

    $form[$term->tid]['sort'] = array(
      '#type' => 'radios',
      '#options' => $sort_options,
      '#title' => t('Group Sort order'),
      '#default_value' => variable_get('weblinks_group_sort_'. $term->tid, ''),
      '#description' => t('This determines how the links page will sort the links.'),
      '#required' => FALSE,
      '#prefix' => '<div class="weblinks-radios">',
      '#suffix' => '</div>',
      );
  }
      
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}

function theme_weblinks_group_settings($form) {
  $rows = array();
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['tid'])) {
      $tid = $form[$key]['tid']['#value'];

      $row = array();
      $row[] = drupal_render($form[$key]['enable']) .'<br />'. drupal_render($form[$key]['collapse']);
      $row[] = drupal_render($form[$key]['maxdisp']) .'<br />'. drupal_render($form[$key]['block']) .'<br />'. drupal_render($form[$key]['allow_groups']);
      $row[] = drupal_render($form[$key]['sort']);
      $rows[] = array('data' => $row, 'class' => 'weblinks-group', 'valign' => 'top');
    }
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No groups available.'), 'colspan' => '5'));
  }

  return theme('table', array(), $rows, array('id' => 'weblinks-groups')) . drupal_render($form);
}

/**
 * Group settings.
 */
function weblinks_group_settings_submit($form, &$form_state) {
  foreach ($form_state['values'] as $row => $values) {
    if (is_array($values) && array_key_exists('tid', $values)) {
      // Save the values that are set, and delete those that are default.
      // This helps prevent a potentially huge number of variables.
      if ($values['enable']) {
        variable_del('weblinks_page_'. $values['tid']);
      }
      else {
        variable_set('weblinks_page_'. $values['tid'], $values['enable']);
      }

      if ($values['collapse']) {
        variable_set('weblinks_collapse_'. $values['tid'], $values['collapse']);
      }
      else {
        variable_del('weblinks_collapse_'. $values['tid']);
      }

      if ($values['maxdisp']) {
        variable_set('weblinks_maxdisp_'. $values['tid'], $values['maxdisp']);
      }
      else {
        variable_del('weblinks_maxdisp_'. $values['tid']);
      }

      if (!$values['block']) {
        variable_set('weblinks_create_block_'. $values['tid'], $values['block']);
      }
      else {
        variable_del('weblinks_create_block_'. $values['tid']);
      }

      if ($values['allow_groups']) {
        variable_set('weblinks_block_allow_groups_'. $values['tid'], $values['allow_groups']);
      }
      else {
        variable_del('weblinks_block_allow_groups_'. $values['tid']);
      }

      if ($values['sort']) {
        variable_set('weblinks_group_sort_'. $values['tid'], $values['sort']);
      }
      else {
        variable_del('weblinks_group_sort_'. $values['tid']);
      }
    }
  }
}

/**
 * Validity check settings.
 */
function weblinks_checker_settings() {
  drupal_add_js(drupal_get_path('module', 'weblinks') .'/weblinks.admin.js');
  $form = array();
  include(drupal_get_path('module', 'weblinks') .'/weblinks_errors.inc');

  $form['weblinks_allow_dupes'] = array(
    '#type' => 'radios',
    '#title' => t('Allow duplicate URLs'),
    '#default_value' => variable_get('weblinks_allow_dupes', 0),
    '#options' => array(1 => t('yes'), 0 => t('no'), 2 => t('warn')),
    '#description' => t('This check is performed at link creation time. Most users will not want to allow duplicate URLs.'),
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div><div class="clear-block"></div>',
    );

  $form['weblinks_validate_check'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check if link is valid when entered'),
    '#default_value' => variable_get('weblinks_validate_check', FALSE),
    '#description' => t('If enabled, the module will attempt to use the URL to contact the site when the link is created or updated. If the link returns any error, the add/update will fail.'),
    );

  $form['weblinks_checker_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable link validity checker?'),
    '#description' => t('Do you want to check the links on a schedule?'),
    '#default_value' => variable_get('weblinks_checker_enabled', FALSE),
    );

  $form['weblinks_checker_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Links to include'),
    '#description' => t('This sets the number of links to be checked on each Cron run. If there are too many, the Cron run may time out.'),
    '#size' => 2,
    '#default_value' => variable_get('weblinks_checker_limit', 5),
    '#prefix' => '<div class="weblinks_checker_hide">',
    );

  $form['weblinks_checker_unpublish'] = array(
    '#type' => 'checkbox',
    '#title' => t('Unpublish link on second error?'),
    '#description' => t('If a previous error has been detected, do you want the link unpublished?'),
    '#default_value' => variable_get('weblinks_checker_unpublish', FALSE),
    );

  $form['weblinks_checker_redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update link on permanent redirect?'),
    '#description' => t('If a link is marked as a permanent redirect ("301"), do you want the link updated?'),
    '#default_value' => variable_get('weblinks_checker_redirect', FALSE),
    );

  $interval = array(0, 15*60, 30*60, 60*60, 2*60*60, 3*60*60, 4*60*60, 6*60*60, 12*60*60, 24*60*60, 2*24*60*60, 3*24*60*60, 7*24*60*60, 14*24*60*60, 28*24*60*60);
  $form['weblinks_checker_interval'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc($interval, 'format_interval'),
    '#title' => t('Cron interval'),
    '#description' => t("This sets the frequency of checking. If you run Cron frequently, you may wish to set this higher than your Cron frequency so that you don't overload the linked site. The check will be run at the next scheduled Cron run after this interval expires. An interval of 0 means every cron run."),
    '#size' => count($interval) / 2,
    '#default_value' => variable_get('weblinks_checker_interval', 24*60*60),
    );

  $ignore_deflt = variable_get('weblinks_checker_ignore', array());
  if (!$ignore_deflt) {
    $ignore_deflt = weblinks_error_codes();
    foreach ($ignore_deflt as $code => $text) {
      if (substr($code, 0, 1) == '2' || substr($code, 0, 1) == '1') {
        $ignore_deflt[$code] = $code;
      }
      else {
        $ignore_deflt[$code] = FALSE;
      }
    }
  }
  $form['weblinks_checker_ignore'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Ignore error codes'),
    '#options' => weblinks_error_codes(),
    '#description' => t('The validity checker will treat all the selected codes as OK.'),
    '#default_value' => $ignore_deflt,
    '#prefix' => '<div class="weblinks-radios">',
    '#suffix' => '</div></div>',
    );

  return system_settings_form($form);
}
