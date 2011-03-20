<?php
// $Id: uc_weightquote.admin.inc,v 1.1.2.4 2009/02/10 21:58:15 islandusurper Exp $

/**
 * @file
 * Shipping quote administration menu items.
 */

/**
 * Configure the store default shipping rate.
 */
function uc_weightquote_admin_settings() {
  $form['uc_weightquote_conditions'] = array(
    '#value' => '<p>'. l(t('Set the conditions that will return a weight-based quote.'), CA_UI_PATH .'/uc_weightquote_get_quote/edit/conditions') .'</p>',
  );

  $form['uc_weightquote_base_rate'] = array('#type' => 'textfield',
    '#title' => t('Base price'),
    '#description' => t('The starting price of weight based shipping costs.'),
    '#default_value' => variable_get('uc_weightquote_base_rate', 0),
    '#size' => 16,
    '#field_prefix' => variable_get('uc_sign_after_amount', FALSE) ? '' : variable_get('uc_currency_sign', '$'),
    '#field_suffix' => variable_get('uc_sign_after_amount', FALSE) ? variable_get('uc_currency_sign', '$') : '',
  );
  $form['uc_weightquote_product_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Default cost adjustment per !unit', array('!unit' => variable_get('uc_weight_unit', 'lb'))),
    '#description' => t('The amount per weight unit to add to the shipping cost for an item.<br />Example: to add $5 per pound, put 5 in here.'),
    '#default_value' => variable_get('uc_weightquote_product_default', 0),
    '#size' => 16,
    '#field_prefix' => variable_get('uc_sign_after_amount', FALSE) ? '' : variable_get('uc_currency_sign', '$'),
    '#field_suffix' => (variable_get('uc_sign_after_amount', FALSE) ? variable_get('uc_currency_sign', '$') : '') . t('/!unit', array('!unit' => variable_get('uc_weight_unit', 'lb'))),
  );

  return system_settings_form($form);
}

