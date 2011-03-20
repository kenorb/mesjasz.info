<?php
// $Id: weblinks.user.inc,v 1.2.2.11 2008/08/13 23:18:58 nancyw Exp $
/**
 * @file
 * Displays a Drupal page containing weblinks submitted by a given user.
 */
function weblinks_user_page($account) {
  $output = drupal_get_form('weblinks_user_form', $account);
  return $output;
}

function _weblinks_user_form_access($user, $account) {
  if (user_access('edit own weblinks', $user) && ($account->uid == $user->uid)) {
    return TRUE;
  }
  return user_access('administer weblinks', $user);
}

function weblinks_user_form($form_state, $account) {
  global $user;
  $vocid = _weblinks_get_vocid();
  $unclassed = _weblinks_unclassed();

  $result = db_query("SELECT n.* FROM {node} n WHERE  n.uid=%d AND n.type='weblinks' ORDER BY n.changed DESC", $account->uid);

  $destination = drupal_get_destination();
  $options = array();
  $node_ops = module_invoke_all('node_operations');
  while ($node = db_fetch_object($result)) {
    $form['title'][$node->nid] = array('#value' => l($node->title, 'node/'. $node->nid) .' '. theme('mark', node_mark($node->nid, $node->changed)));
    $form['status'][$node->nid] =  array('#value' =>  ($node->status ? t('published') : t('not published')));

    $tid_list = array();
    $terms = db_query("SELECT tn.tid, td.name FROM {term_node} tn LEFT JOIN {term_data} td USING (tid) WHERE tn.nid=%d AND tn.vid=%d", $node->nid, $node->vid);
    while ($row = db_fetch_array($terms)) {
      $tid_list[] = check_plain($row['name'] ? $row['name'] : $unclassed->name);
    }
    $form['group'][$node->nid] =  array('#value' => implode(', ', $tid_list));

    if (_weblinks_user_form_access($user, $account)) {
      $form['operationse'][$node->nid] = array('#value' => l(t('edit'), 'node/'. $node->nid .'/edit', array('query' => $destination)));
      $form['operationsd'][$node->nid] = array('#value' => l(t('delete'), 'node/'. $node->nid .'/delete', array('query' => $destination)));
    }
  }
  $form['pager'] = array('#value' => theme('pager', NULL, 50, 0));

  return $form;
}

/**
 * Theme node administration overview.
 */
function theme_weblinks_user_form($form) {
  // Overview table:
  $header = array(t('Title'), t('Status'), t('Group'), array('data' => t('Operations'), 'colspan' => '2'));
  $rows =array();

  if (isset($form['title']) && is_array($form['title'])) {
    foreach (element_children($form['title']) as $key) {
      $rows[] = array(
        drupal_render($form['title'][$key]),
        drupal_render($form['status'][$key]),
        drupal_render($form['group'][$key]),
        drupal_render($form['operationse'][$key]),
        drupal_render($form['operationsd'][$key]),
        );
    }
  }
  else  {
    $rows[] = array(array('data' => t('No posts available.'), 'colspan' => '6'));
  }
  $output .= theme('table', $header, $rows);
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }
  $output .= drupal_render($form);
  return $output;
}

