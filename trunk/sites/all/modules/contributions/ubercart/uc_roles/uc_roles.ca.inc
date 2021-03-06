<?php
// $Id: uc_roles.ca.inc,v 1.1.2.5 2009/02/04 13:44:32 islandusurper Exp $

/**
 * @file
 * This file contains the Conditional Actions hooks and functions necessary to make the
 * roles-related entity, conditions, events, and actions work.
 */


/******************************************************************************
 * Conditional Actions Hooks                                                  *
 ******************************************************************************/

/**
 * Implementation of hook_ca_entity().
 *
 * An entity is defined in order to get a role expiration down
 * to token in the email.
 */
function uc_roles_ca_entity() {

  // CA entity for a role expiration object.
  $entities['uc_roles_expiration'] = array(
    '#title' => t('Ubercart role expiration'),
    '#type' => 'object',
  );

  return $entities;
}

/**
 * Implementation of hook_ca_predicate().
 */
function uc_roles_ca_predicate() {

  // Renew all the roles on an order when the status matches what's set in the roles admin settings.
  $configurations['uc_role_renewal'] = array(
    '#title' => t('Renew purchased roles'),
    '#description' => t('Renew purchased roles if the order status matches.'),
    '#class' => 'renewal',
    '#trigger' => 'uc_order_status_update',
    '#status' => 1,
    '#conditions' => array(
      '#operator' => 'AND',
      '#conditions' => array(
        array(
          '#name' => 'uc_order_status_condition',
          '#title' => t('If the order status is completed.'),
          '#argument_map' => array(
            'order' => 'updated_order',
          ),
          '#settings' => array(
            'order_status' => 'completed',
          ),
        ),
      ),
    ),
    '#actions' => array(
      array(
        '#name' => 'uc_roles_order_renew',
        '#title' => t('Update all role expirations for this order.'),
        '#argument_map' => array(
          'order' => 'order',
        ),
      ),
    ),
  );

  $order_args = array(
    'order' => 'order',
    'expiration' => 'expiration',
  );

  $user_args = array(
    'user' => 'user',
    'expiration' => 'expiration',
  );

  // Notify the user when a role is granted.
  $configurations['uc_role_notify_grant'] = array(
    '#title' => t('Notify customer when a role is granted'),
    '#description' => t('Notify the customer when they have had a role granted on their user.'),
    '#class' => 'notification',
    '#trigger' => 'uc_roles_notify_grant',
    '#status' => 1,
    '#actions' => array(
      array(
        '#name' => 'uc_roles_order_email',
        '#title' => t('Send an e-mail to the customer'),
        '#argument_map' => $order_args,
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[order-email]',
          'subject' => uc_get_message('uc_roles_grant_subject'),
          'message' => uc_get_message('uc_roles_grant_message'),
          'format' => 1,
        ),
      ),
    ),
  );

  // Notify the user when a role is revoked.
  $configurations['uc_role_notify_revoke'] = array(
    '#title' => t('Notify customer when a role is revoked'),
    '#description' => t('Notify the customer when they have had a role revoked from their user.'),
    '#class' => 'notification',
    '#trigger' => 'uc_roles_notify_revoke',
    '#status' => 1,
    '#actions' => array(
      array(
        '#name' => 'uc_roles_user_email',
        '#title' => t('Send an e-mail to the customer'),
        '#argument_map' => $user_args,
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[user-mail]',
          'subject' => uc_get_message('uc_roles_revoke_subject'),
          'message' => uc_get_message('uc_roles_revoke_message'),
          'format' => 1,
        ),
      ),
    ),
  );

  // Notify the user when a role is renewed.
  $configurations['uc_role_notify_renew'] = array(
    '#title' => t('Notify customer when a role is renewed'),
    '#description' => t('Notify the customer when they have had a role renewed on their user.'),
    '#class' => 'notification',
    '#trigger' => 'uc_roles_notify_renew',
    '#status' => 1,
    '#actions' => array(
      array(
        '#name' => 'uc_roles_order_email',
        '#title' => t('Send an e-mail to the customer'),
        '#argument_map' => $order_args,
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[order-email]',
          'subject' => uc_get_message('uc_roles_renew_subject'),
          'message' => uc_get_message('uc_roles_renew_message'),
          'format' => 1,
        ),
      ),
    ),
  );

  // Notify the user when a role is about to expire.
  $configurations['uc_role_notify_reminder'] = array(
    '#title' => t('Notify customer when a role is about to expire'),
    '#description' => t('Notify the customer when they have had a role that is about to expire.'),
    '#class' => 'notification',
    '#trigger' => 'uc_roles_notify_reminder',
    '#status' => 1,
    '#actions' => array(
      array(
        '#name' => 'uc_roles_user_email',
        '#title' => t('Send an e-mail to the customer'),
        '#argument_map' => $user_args,
        '#settings' => array(
          'from' => uc_store_email_from(),
          'addresses' => '[user-mail]',
          'subject' => uc_get_message('uc_roles_reminder_subject'),
          'message' => uc_get_message('uc_roles_reminder_message'),
          'format' => 1,
        ),
      ),
    ),
  );

  return $configurations;
}

/**
 * Implementation of hook_ca_action().
 */
function uc_roles_ca_action() {

  // Renew a role expiration.
  $actions['uc_roles_order_renew'] = array(
    '#title' => t('Renew the roles on an order.'),
    '#category' => t('renewal'),
    '#callback' => 'uc_roles_action_order_renew',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
    ),
  );

  // Send an email to an order with a role expiration
  $actions['uc_roles_order_email'] = array(
    '#title' => t('Send an order email regarding roles.'),
    '#category' => t('Notification'),
    '#callback' => 'uc_roles_action_order_email',
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Order'),
      ),
      'expiration' => array(
        '#entity' => 'uc_roles_expiration',
        '#title' => t('Role expiration'),
      ),
    ),
  );

  // Send an email to a user with a role expiration
  $actions['uc_roles_user_email'] = array(
    '#title' => t('Send an order email regarding roles.'),
    '#category' => t('Notification'),
    '#callback' => 'uc_roles_action_user_email',
    '#arguments' => array(
      'user' => array(
        '#entity' => 'user',
        '#title' => t('User'),
      ),
      'expiration' => array(
        '#entity' => 'uc_roles_expiration',
        '#title' => t('Role expiration'),
      ),
    ),
  );

  return $actions;
}

/**
 * Implementation of hook_ca_trigger().
 */
function uc_roles_ca_trigger() {

  $order = array(
    '#entity' => 'uc_order',
    '#title' => t('Order'),
  );
  $account = array(
    '#entity' => 'user',
    '#title' => t('User'),
  );
  $expiration = array(
    '#entity' => 'uc_roles_expiration',
    '#title' => t('Role expiration'),
  );

  $triggers['uc_roles_notify_grant'] = array(
    '#title' => t('E-mail for granted roles'),
    '#category' => t('Notification'),
    '#arguments' => array(
      'order' => $order,
      'expiration' => $expiration,
    ),
  );

  $triggers['uc_roles_notify_revoke'] = array(
    '#title' => t('E-mail for revoked roles'),
    '#category' => t('Notification'),
    '#arguments' => array(
      'user' => $account,
      'expiration' => $expiration,
    ),
  );

  $triggers['uc_roles_notify_renew'] = array(
    '#title' => t('E-mail for renewed roles'),
    '#category' => t('Notification'),
    '#arguments' => array(
      'order' => $order,
      'expiration' => $expiration,
    ),
  );

  $triggers['uc_roles_notify_reminder'] = array(
    '#title' => t('E-mail for role expiration reminders'),
    '#category' => t('Notification'),
    '#arguments' => array(
      'user' => $account,
      'expiration' => $expiration,
    ),
  );

  return $triggers;
}

/**
 * Send an email with order and role replacement tokens.
 *
 * The recipients, subject, and message fields take order token replacements.
 */
function uc_roles_action_order_email($order, $role_expiration, $settings) {
  $account = user_load(array('uid' => $order->uid));
  $language = language_default();

  // Token replacements for the subject and body
  $settings['replacements'] = array(
    'global' => NULL,
    'order' => $order,
    'user' => $account,
    'uc_roles' => $role_expiration,
  );

  // Replace tokens and parse recipients.
  $recipients = array();
  $addresses = token_replace_multiple($settings['addresses'], $settings['replacements']);
  foreach (explode("\n", $addresses) as $address) {
    $recipients[] = trim($address);
  }

  // Send to each recipient.
  foreach ($recipients as $email) {
    $sent = drupal_mail('uc_order', 'action-mail', $email, $language, $settings, $settings['from']);

    if (!$sent['result']) {
      watchdog('ca', 'Attempt to e-mail @email concerning order @order_id failed.', array('@email' => $email, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    }
  }
}

// Email settings form
function uc_roles_action_order_email_form($form_state, $settings = array()) {
  return ca_build_email_form($form_state, $settings, array('global', 'uc_roles'));
}

/**
 * Send an email with order and role replacement tokens.
 *
 * The recipients, subject, and message fields take order token replacements.
 */
function uc_roles_action_user_email($account, $role_expiration, $settings) {
  $language = language_default();

  // Token replacements for the subject and body
  $settings['replacements'] = array(
    'global' => NULL,
    'user' => $account,
    'uc_roles' => $role_expiration,
  );

  // Replace tokens and parse recipients.
  $recipients = array();
  $addresses = token_replace_multiple($settings['addresses'], $settings['replacements']);
  foreach (explode("\n", $addresses) as $address) {
    $recipients[] = trim($address);
  }

  // Send to each recipient.
  foreach ($recipients as $email) {
    $sent = drupal_mail('uc_order', 'action-mail', $email, $language, $settings, $settings['from']);

    if (!$sent['result']) {
      watchdog('ca', 'Attempt to e-mail @email concerning order @order_id failed.', array('@email' => $email, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    }
  }
}

// Email settings form
function uc_roles_action_user_email_form($form_state, $settings = array()) {
  return ca_build_email_form($form_state, $settings, array('global', 'uc_roles'));
}

/**
 * Builds an email settings form.
 */
function ca_build_email_form($form_state, $settings, $token_filters) {
  $form = array();

  $form['from'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender'),
    '#default_value' => $settings['from'],
    '#description' => t('The "From" address.'),
    '#required' => TRUE,
  );
  $form['addresses'] = array(
    '#type' => 'textarea',
    '#title' => t('Recipients'),
    '#default_value' => $settings['addresses'],
    '#description' => t('Enter the email addresses to receive the notifications, one on each line. You may use order tokens for dynamic email addresses.'),
    '#required' => TRUE,
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $settings['subject'],
    '#required' => TRUE,
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => $settings['message'],
  );
  $form['format'] = filter_form($settings['format']);

  $form['token_help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#description' => t('You can make use of the replacement patterns in the recipients field, the subject, and the message body.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  foreach ($token_filters as $name) {
    $form['token_help'][$name] = array(
      '#type' => 'fieldset',
      '#title' => t('@name replacement patterns', array('@name' => drupal_ucfirst($name))),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['token_help'][$name]['content'] = array(
      '#value' => theme('token_help', $name),
    );
  }

  return $form;
}

/**
 * Renew an orders product roles.
 *
 * @param $order
 *   An Ubercart order object.
 *
 * This function updates expiration time on all roles found on all products
 * on a given order. First, the order user is loaded, then the order's products
 * are scanned for role product features. If any are found, the expiration time
 * of the role is set using the feature settings to determine the new length of
 * time the new expiration will last. An order comment is saved, and the user is
 * notified in Drupal, as well as through the email address associated with the
 * order.
 */
function uc_roles_action_order_renew($order) {

  global $user;

  // Load the order's user.
  if (!($order_user = user_load($order->uid))) {
    return;
  }

  foreach ($order->products as $product) {

    $roles = db_query("SELECT * FROM {uc_roles_products} WHERE nid = %d", $product->nid);
    while ($role = db_fetch_object($roles)) {

      // Product model matches, or was 'any'.
      if (!empty($role->model) && $role->model != $product->model) {
        continue;
      }

      // Leave an order comment.
      $existing_role = db_fetch_object(db_query("SELECT * FROM {uc_roles_expirations} WHERE uid = %d AND rid = %d", $order_user->uid, $role->rid));
      if (!is_null($existing_role->expiration)) {
        $op = 'renew';
        $comment = t('Customer user role %role renewed.', array('%role' => _uc_roles_get_name($role->rid)));
      }
      else {
        $op = 'grant';
        $comment = t('Customer granted user role %role.', array('%role' => _uc_roles_get_name($role->rid)));
      }
      uc_order_comment_save($order->order_id, $user->uid, $comment);

      // Grant the role to the user.
      $expiration = _uc_roles_product_get_expiration($role, $product->qty, $existing_role->expiration);

      // Grant the role to the user.
      uc_roles_grant($order_user, $role->rid, $expiration);

      // Get the new expiration (if applicable)
      $new_expiration = db_fetch_object(db_query("SELECT * FROM {uc_roles_expirations} WHERE uid = %d AND rid = %d", $order_user->uid, $role->rid));

      // Trigger role email.
      ca_pull_trigger('uc_roles_notify_'. $op, $order, $new_expiration);
    }
  }
}

// Borrow the order form for role renewal order state configuration.
function uc_roles_action_order_renew_form($form_state, $settings = array()) {
  return uc_order_action_update_status_form($form_state, $settings);
}
