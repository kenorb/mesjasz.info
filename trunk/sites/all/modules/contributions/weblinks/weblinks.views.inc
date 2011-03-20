<?php
// $Id: weblinks.views.inc,v 1.1.2.3 2008/09/14 00:31:22 nancyw Exp $
/**
 * @file
 * Provide views data and handlers for weblinks.module
 */

/**
 * @defgroup views_weblinks_module weblinks.module handlers
 */

/**
 * Implementation of hook_views_data()
 */
function weblinks_views_data() {
  // Basic table information.

  // ----------------------------------------------------------------
  // weblinks table

  $data['weblinks']['table']['group']  = t('Web Links');

  $data['weblinks']['table']['join'] = array(
    // ...to the node table
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid AND node.vid = weblinks.vid',
    ),
  );

  // url
  $data['weblinks']['url'] = array(
    'title' => t('URL'),
    'help' => t('The web address for the link.'),

    'field' => array(
      'handler' => 'views_handler_field_weblinks',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // click_count
  $data['weblinks']['click_count'] = array(
    'title' => t('Click count'),
    'help' => t('The total number of times the link has been clicked.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // last_click
  $data['weblinks']['last_click'] = array(
    'title' => t('Last click'),
    'help' => t('The most recent time the link was clicked.'),

    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // weight
  $data['weblinks']['weight'] = array(
    'title' => t('Weight'),
    'help' => t("The node weight for the link's node."),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // last_status
  $data['weblinks']['last_status'] = array(
    'title' => t('Last status'),
    'help' => t('The status returned the last time the link was checked for validity.'),

    'field' => array(
//      'handler' => 'views_handler_field_string',
      'click sortable' => TRUE,
     ),
    'filter' => array(
//      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // last_checked
  $data['weblinks']['last_checked'] = array(
    'title' => t('Last checked'),
    'help' => t('The most recent time the link was checked to see if it works.'),

    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
