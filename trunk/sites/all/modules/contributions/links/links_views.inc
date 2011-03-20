<?php
/*
 * $Id: links_views.inc,v 1.2 2006/06/22 03:01:14 syscrusher Exp $
 *
 * Author:  Eaton (Drupal.org user #16496)
 *
 * RELEASE STATUS: This is BETA code; do not use in production sites.
 */

function links_views_tables() {
  $tables['links_node'] = array(
    'name' => 'links_node',
    "provider" => "links",
    'join' => array(
      'left' => array(
        'table' => 'node',
        'field' => 'nid'
      ),
      'right' => array(
        'field' => 'nid'
      ),
    ),
    'fields' => array(
      'link_title' => array(
        'name' => t('Links: Link title'),
        'help' => t('The title of the associated link.'),
        'query_handler' => 'links_qhandler_title',
        'handler' => array(
           'links_handler_field_title_clickable' => 'Show as link',
           'links_handler_field_title' => 'Show as text',
         ),
      ),
      'clicks' => array(
        'name' => t('Links: Clickthroughs'),
        'help' => t('The number of clickthroughs for the associated link.'),
      ),
    ),
    'sorts' => array(
      'clicks' => array(
        'name' => t('Links: Clickthroughs'),
        'help' => t('This allows you to sort by the number of link clickthroughs.'),
      )
    ),
  );
  $tables['links'] = array(
    'name' => 'links',
    "provider" => "links",
    'join' => array(
      'left' => array(
        'table' => 'links_node',
        'field' => 'lid'
      ),
      'right' => array(
        'field' => 'lid'
      ),
    ),
    'fields' => array(
      'url' => array(
        'name' => t('Links: Link URL'),
        'query_handler' => 'links_qhandler_url',
        'handler' => 'links_handler_field_url',
        'help' => t('The clickable URL of the link.'),
      ),
    ),
  );
  return $tables;
}

function links_handler_field_url($fieldinfo, $fielddata, $value, $data) {
  return l($value, $value);
}

function links_handler_field_title_clickable($fieldinfo, $fielddata, $value, $data) {
  $title = $data->links_node_link_title;
  if ($title == '') {
    $title = $data->global_link_title;   
  }
  return l($title, $data->link_title_url);
}

function links_handler_field_title($fieldinfo, $fielddata, $value, $data) {
  $title = $data->links_node_link_title;
  if ($title == '') {
    $title = $data->global_link_title;   
  }
  return $title;
}

function links_qhandler_url($field, $fieldinfo, &$query) {
  $query->ensure_table('links');
  $query->add_field('link_title', 'links', 'global_link_title');
}

function links_qhandler_title($field, $fieldinfo, &$query) {
  $query->ensure_table('links');
  $query->add_field('link_title', 'links', 'global_link_title');
  $query->add_field('url', 'links', 'link_title_url');
}
