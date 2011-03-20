<?php
// $Id: simplefeed.views.inc,v 1.1.2.1 2008/03/01 02:08:58 csevb10 Exp $

/**
 * Implementation of hook_views_data().
 */
function simplefeed_views_data() {
  $data = array();
  $data['table']['group'] = t('Simplefeed Feed');
  // Advertise this table as a possible base table
  $data['table']['join']['node'] = array(
    'handler' => 'views_join',
    'arguments' => array('simplefeed_feed', 'node', 'vid', 'vid')
  );

  // expires
  $data['expires'] = array(
    'title' => t('Expires'),
    'help' => t('The default expiration date for a feed.'),
    'field' => array(
      'field' => 'expires',
      'group' => t('Simplefeed Feed'),
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by the default expiration date for a feed.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'help' => t('Filter by the default expiration date for a feed. '),
    ),
  );
  
  // refresh
  $data['refresh'] = array(
    'title' => t('Refresh'),
    'help' => t('How often a feed should be refreshed.'),
    'field' => array(
      'field' => 'expires',
      'group' => t('Simplefeed Feed'),
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by the default refresh time period for a feed.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'help' => t('Filter by the default refresh time period for a feed. '),
    ),
  );
  
  // url
  $data['url'] = array(
    'title' => t('URL'),
    'help' => t('The URL to the RSS feed.'),
    'field' => array(
      'field' => 'url',
      'group' => t('Simplefeed Feed'),
      'handler' => 'views_handler_field_markup',
      'format' => 1,
    ),
  );
  
  return array('simplefeed' => $data);
}