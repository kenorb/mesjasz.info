<?php
// $Id: weblinks_node_view.tpl.php,v 1.5.2.7 2008/10/22 17:41:11 nancyw Exp $

/**
 * @file
 * weblinks_node_view.tpl.php
 * Theme implementation to display a list of related content.
 *
 * Available variables:
 * - $node: node object that contains the URL.
 * - $options: Options for the URL.
 * - $status: Status of the link.
 */
if ($node->url != NULL) {
  switch (variable_get('weblinks_view_as', 'url')) {
    case 'url':
      if (variable_get('weblinks_strip', FALSE)) {
        $parts = parse_url($node->url);
        $url = str_replace('www.', '', $parts['host']) . $parts['path'];
      }
      else {
        $url = $node->url;
      }
      $link = l(_weblinks_trim($url, variable_get('weblinks_trim', 0)), $node->url, $options);
      break;

    case 'visit':
      $title_text = variable_get('weblinks_visit_text', t('Visit [title]'));
      if (module_exists('token')) {
        $title = token_replace($title_text, 'node', $node);
      }
      else {
        $title = str_replace('[title]', check_plain($node->title), $title_text);
      }
      $link = decode_entities(l($title, $node->url, $options));
      break;
  }
  echo '<div class="weblinks-linkview">'. $link .'</div>'. $status;
}
