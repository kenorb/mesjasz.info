<?php
// $Id: weblinks.taxonomy.inc,v 1.3.2.2 2008/08/01 02:59:22 rmiddle Exp $

/**
 * @file
 * Taxonomy Settings form.
 */

/**
 * Returns a form for adding a container to the weblink vocabulary.
 *
 * @param $edit Associative array containing a container term to be added or edited.
 */
function weblinks_form_container($form_state, $tid = 0) {
  $edit = array('weight' => 0);
  if ((is_numeric($tid)) && ($tid != 0)) {
    $edit = (array)taxonomy_get_term($tid);
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Group Name'),
    '#default_value' => $edit['name'],
    '#description' => t('This Groups the links and allows you to sort them.'),
    '#required' => TRUE,
    );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Group Description'),
    '#default_value' => $edit['description'],
    '#cols' => 60,
    '#rows' => 5,
    '#description' => t('The description can provide additional information about the link grouping.'),
    );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $edit['weight'],
    '#delta' => 10,
    '#description' => t('In listings, the heavier terms (with a larger weight) will sink and the lighter terms will be positioned nearer the top.'),
  );
  $vocid =  _weblinks_get_vocid();
  $form['vid'] = array('#type' => 'hidden', '#value' => $vocid);
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  if ($tid) {
    $form['delete'] = array('#type' => 'submit', '#value' => t('Delete'),     '#submit' => array('weblinks_form_container_submit_delete'));
    $form['tid'] = array('#type' => 'hidden', '#value' => $tid);
  }
  return $form;
}

/**
 * Process forum form and container form submissions.
 */
function weblinks_form_container_submit($form, &$form_state) {
  $container = TRUE;
  $type = t('weblinks container');
  $status = taxonomy_save_term($form_state['values']);
  switch ($status) {
    case SAVED_NEW:
      $containers = variable_get('weblinks_containers', array());
      $containers[] = $form_state['values']['tid'];
      variable_set('weblinks_containers', $containers);
      drupal_set_message(t('Created new %type %term.', array('%term' => $form_state['values']['name'], '%type' => $type)));
      break;
    case SAVED_UPDATED:
      drupal_set_message(t('The %type %term has been updated.', array('%term' => $form_state['values']['name'], '%type' => $type)));
      break;
  }
  $form_state['redirect'] = 'admin/content/weblinks';
  return;
}

/**
 * Process forum form and container form submissions.
 */
function weblinks_form_container_submit_delete($form, &$form_state) {
  $container = TRUE;
  $type = t('weblinks container');
  $status = taxonomy_del_term($form_state['values']['tid']);
  switch ($status) {
    case SAVED_DELETED:
      drupal_set_message(t('The %type %term has been deleted.', array('%term' => $form_state['values']['name'], '%type' => $type)));
      break;
  }
  $form_state['redirect'] = 'admin/content/weblinks';
  return;
}
