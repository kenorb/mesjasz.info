<?php
// $Id: uc_recurring.admin.inc,v 1.1.2.2 2008/11/07 21:13:25 islandusurper Exp $

/**
 * @file
 * Recurring payments administration menu items.
 *
 */

// Displays a table for the administration of recurring fees.
function uc_recurring_admin() {
  $output = drupal_get_form('uc_recurring_admin_filter_form');

  $header = array(
    array('data' => t('ID'), 'field' => 'ru.rfid', 'sort' => 'desc'),
    array('data' => t('Order'), 'field' => 'ru.order_id'),
    array('data' => t('Amount'), 'field' => 'ru.fee_amount'),
    array('data' => t('Next'), 'field' => 'ru.next_charge'),
    array('data' => t('Interval'), 'field' => 'ru.regular_interval'),
    array('data' => t('Left'), 'field' => 'ru.remaining_intervals'),
    array('data' => t('Total')),
    array('data' => t('Operations')),
  );

  if (arg(4) == 'view' && intval(arg(6)) > 0) {
    if (arg(5) == 'fee') {
      $result = db_query("SELECT * FROM {uc_recurring_users} AS ru WHERE ru.rfid = %d", arg(6));
    }
    elseif (arg(5) == 'order') {
      $result = db_query("SELECT * FROM {uc_recurring_users} AS ru WHERE ru.order_id = %d", arg(6));
    }
  }
  else {
    $result = pager_query("SELECT * FROM {uc_recurring_users} AS ru" . tablesort_sql($header), 30);
  }

  while ($fee = db_fetch_array($result)) {
    $ops = array();

    // Get the $ops from the module implementing the handler.
    $callback = $fee['fee_handler'] .'_recurring_fee_ops';
    if (function_exists($callback)) {
      $ops = $callback('fee_admin', $fee);
    }

    $rows[] = array(
      l($fee['rfid'], 'admin/store/orders/recurring/view/fee/'. $fee['rfid']),
      l($fee['order_id'], 'admin/store/orders/'. $fee['order_id']),
      uc_currency_format($fee['fee_amount']),
      $fee['remaining_intervals'] == 0 ? '-' : format_date($fee['next_charge'], 'small'),
      array('data' => check_plain($fee['regular_interval']), 'nowrap' => 'nowrap'),
      $fee['remaining_intervals'],
      $fee['remaining_intervals'] + $fee['charged_intervals'],
      array('data' => implode(' ', $ops), 'nowrap' => 'nowrap'),
    );
  }

  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, 30, 0);

  if (arg(4) == 'view') {
    $output .= l(t('Back to the full list.'), 'admin/store/orders/recurring');
  }

  return $output;
}

// Filter by a specific order ID.
function uc_recurring_admin_filter_form() {
  $form['type'] = array(
    '#type' => 'select',
    '#options' => array(
      'order' => t('Order ID'),
      'fee' => t('Fee ID'),
    ),
    '#default_value' => arg(5) == 'fee' ? 'fee' : 'order',
    '#prefix' => '<div style="float: left; margin-right: 1em;">',
    '#suffix' => '</div>',
  );
  $form['id'] = array(
    '#type' => 'textfield',
    '#default_value' => arg(6),
    '#size' => 10,
    '#prefix' => '<div style="float: left; margin-right: 1em;">',
    '#suffix' => '</div>',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#attributes' => array('style' => 'margin: .85em 0em;'),
  );

  return $form;
}

function uc_recurring_admin_filter_form_submit($form, &$form_state) {
  if (intval($form_state['values']['id']) > 0) {
    $form_state['redirect'] = 'admin/store/orders/recurring/view/'. $form_state['values']['type'] .'/'. $form_state['values']['id'];
  }
}

// Confirm a recurring fee charge.
function uc_recurring_admin_charge_form() {
  $fee = uc_recurring_fee_load('user', arg(4));
  $form['message'] = array(
    '#value' => '<div>'. t('Are you sure you want to charge the customer !amount at this time?', array('!amount' => uc_currency_format($fee['fee_amount']))) .'</div>',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Charge'),
    '#suffix' => l(t('Cancel'), uc_referer_uri()),
  );
  return $form;
}

function uc_recurring_admin_charge_form_submit($form, &$form_state) {
  $fee = uc_recurring_fee_load('user', arg(4));

  // Attempt to process the charge.
  if (uc_recurring_charge($fee)) {
    // Update the fee in the database.
    $next_charge = strtotime('+'. $fee['regular_interval']);
    db_query("UPDATE {uc_recurring_users} SET next_charge = %d, remaining_intervals = remaining_intervals - 1, charged_intervals = charged_intervals + 1 WHERE rfid = %d", $next_charge, $fee['rfid']);
    drupal_set_message(t('Recurring fee @fee charged successfully.', array('@fee' => arg(4))));
  }
  else {
    drupal_set_message(t('Attempt to charge recurring fee @fee failed.', array('@fee' => arg(4))), 'error');
  }

  $form_state['redirect'] = 'admin/store/orders/recurring/view/fee/'. arg(4);
}

// Let an admin edit a recurring fee.
function uc_recurring_admin_edit_form() {
  drupal_add_css(drupal_get_path('module', 'uc_recurring') .'/uc_recurring.css');

  $fee = uc_recurring_fee_load('user', arg(4));
  list($fee['regular_interval_value'], $fee['regular_interval_unit']) = explode(' ', $fee['regular_interval']);

  $form['fee_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Recurring fee amount'),
    '#description' => t('Charge this amount each billing period.'),
    '#default_value' => $fee['fee_amount'],
    '#size' => 16,
    '#field_prefix' => variable_get('uc_sign_after_amount', FALSE) ? '' : variable_get('uc_currency_sign', '$'),
    '#field_suffix' => variable_get('uc_sign_after_amount', FALSE) ? variable_get('uc_currency_sign', '$') : '',
  );

  $form['remaining_intervals'] = array(
    '#type' => 'textfield',
    '#title' => t('Remaining billing periods'),
    '#description' => t('Specify how many more times to charge the fee.'),
    '#size' => 16,
    '#default_value' => $fee['remaining_intervals'],
  );

  $form['regular'] = array(
    '#type' => 'fieldset',
    '#title' => t('Regular interval'),
    '#collapsible' => FALSE,
    '#description' => t('Modify the length of the billing period for this fee. Changing this value will reset the timer for the next charge. You can also charge the fee manually to collect payment ahead of time and reset the interval.'),
    '#attributes' => array('class' => 'interval-fieldset'),
  );
  $form['regular']['regular_interval_value'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc(uc_range(1, 52)),
    '#default_value' => $fee['regular_interval_value'],
  );
  $form['regular']['regular_interval_unit'] = array(
    '#type' => 'select',
    '#options' => array(
      'days' => t('day(s)'),
      'weeks' => t('week(s)'),
      'months' => t('month(s)'),
      'years' => t('year(s)'),
    ),
    '#default_value' => $fee['regular_interval_unit'],
  );

  $form['reset_next_charge'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reset the next charge timer upon form submission using the specified interval.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#suffix' => l(t('Cancel'), uc_referer_uri()),
  );

  return $form;
}

function uc_recurring_admin_edit_form_submit($form, &$form_state) {
  $interval = $form_state['values']['regular_interval_value'] .' '. $form_state['values']['regular_interval_unit'];

  db_query("UPDATE {uc_recurring_users} SET fee_amount = %f, regular_interval = '%s', "
          ."remaining_intervals = %d WHERE rfid = %d", $form_state['values']['fee_amount'],
           $interval, $form_state['values']['remaining_intervals'], arg(4));

  if ($form_state['values']['reset_next_charge']) {
    $next_charge = strtotime('+'. $interval);
    db_query("UPDATE {uc_recurring_users} SET next_charge = %d WHERE rfid = %d", $next_charge, arg(4));
  }

  drupal_set_message(t('The changes to the fee have been saved.'));

  $form_state['redirect'] = 'admin/store/orders/recurring/view/fee/'. arg(4);
}

// Confirm a recurring fee deletion.
function uc_recurring_admin_delete_form() {
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#suffix' => l(t('Cancel'), uc_referer_uri()),
  );
  return $form;
}

function uc_recurring_admin_delete_form_submit($form, &$form_state) {
  uc_recurring_fee_delete(array('pfid' => arg(4)), 'user');

  drupal_set_message(t('Recurring fee @fee deleted.', array('@fee' => arg(4))));

  $form_state['redirect'] = 'admin/store/orders/recurring';
}

