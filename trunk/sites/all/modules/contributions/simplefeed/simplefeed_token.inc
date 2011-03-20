<?php
// $Id: simplefeed_token.inc,v 1.1.4.1 2008/02/19 17:30:21 csevb10 Exp $

/**
 * Implementation of hook_token_values().
 */
function simplefeed_token_values($type, $object = NULL) {
  if ($type == 'node') {
    $node = $object;

    $tokens = array();
    $tokens['feed-url'] = check_url($node->url);
    return $tokens;
  }
}

/**
 * Implementation of hook_token_list().
 */
function simplefeed_token_list($type) {
  if ($type == 'node' || $type == 'all') {
    $list = array(
      'simplefeed' => array(
        'feed-url' => t('The URL for the feed.'),
      ),
    );
    return $list;
  }
}
