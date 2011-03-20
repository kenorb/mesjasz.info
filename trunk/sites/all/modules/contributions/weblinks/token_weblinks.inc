<?php
// $Id: token_weblinks.inc,v 1.1.2.6 2008/10/03 15:22:44 nancyw Exp $ */
/**
 * @file
 *   Token module support for the Weblinks module.
 */

/**
 * Implementation of hook_token_list().
 */
function weblinks_token_list($type = 'all') {
  $tokens = array();
  if ($type == 'node' || $type == 'all') {
    $tokens['node']['weight'] = t('The weight of the node.');
    $tokens['node']['weblinks-url'] = t('The URL of the Web Link.');
    $tokens['node']['weblinks-url-raw'] = t('Unfiltered URL of the Web Link.  WARNING - raw user input.');
    $tokens['node']['weblinks-title'] = t('The title of the Web Link.');
    $tokens['node']['weblinks-title-raw'] = t('Unfiltered title of the Web Link.  WARNING - raw user input.');
    $tokens['node']['weblinks-click-count'] = t('How many times the Web Link has been clicked.');
    $tokens['node']['weblinks-last-click'] = t('When the Web Link was last clicked on.');
    $tokens['node']['weblinks-last-status'] = t('The status of the Web Link when it was last checked.');
    $tokens['node']['weblinks-last-checked'] = t('The date/time when the Web Link was last checked.');
    return $tokens;
  }
}

/**
 * Implementation of hook_token_values().
 */
function weblinks_token_values($type, $object = NULL, $options = array()) {
  $tokens = array();
  if ($type == 'node' && $object->type == 'weblinks') {
    $tokens['weight'] = $object->weight;
    $tokens['weblinks-url'] = decode_entities(check_plain($object->url));
    $tokens['weblinks-url-raw'] = $object->url;
    // Panels deletes the title (http://drupal.org/node/213751).
    if (!isset($object->title)) {
      $object->title = db_result(db_query("SELECT n.title FROM {node} n WHERE n.nid=%d", $object->nid));
    }
    $tokens['weblinks-title'] = decode_entities(check_plain($object->title));
    $tokens['weblinks-title-raw'] = $object->title;
    $tokens['weblinks-click-count'] = $object->click_count;
    $tokens['weblinks-last-click'] = $object->last_click;
    $tokens['weblinks-last-status'] = $object->last_status;
    $tokens['weblinks-last-checked'] = $object->last_checked;

    if (isset($object->last_click)) {
      $tokens += _weblinks_token_date($object->last_click, 'clicked');
    }

    if (isset($object->last_checked)) {
      $tokens += _weblinks_token_date($object->last_checked, 'checked');
    }

    return $tokens;
  }
}

function _weblinks_token_date($date, $prefix = NULL) {
  if ($prefix) {
    $prefix .= '-';
  }
  $values = array(
    $prefix .'small'  => format_date($date, 'small'),
    $prefix .'medium' => format_date($date, 'medium'),
    $prefix .'large'  => format_date($date, 'large'),
    $prefix .'since'  => $date ? format_interval(time() - $date) : t('Never'),
    );
  $date = (int)$date;
  $values += array(
    $prefix .'yyyy'  => date('Y', $date),
    $prefix .'yy'    => date('y', $date),
    $prefix .'month' => date('F', $date),
    $prefix .'mon'   => date('M', $date),
    $prefix .'mm'    => date('m', $date),
    $prefix .'m'     => date('n', $date),
    $prefix .'ww'    => date('W', $date),
    $prefix .'date'  => date('N', $date),
    $prefix .'day'   => date('l', $date),
    $prefix .'ddd'   => date('D', $date),
    $prefix .'dd'    => date('d', $date),
    $prefix .'d'     => date('j', $date),
    $prefix .'hh'    => date('h', $date),
    $prefix .'mm'    => date('i', $date),
    $prefix .'ss'    => date('d', $date),
    );

  return $values;
}