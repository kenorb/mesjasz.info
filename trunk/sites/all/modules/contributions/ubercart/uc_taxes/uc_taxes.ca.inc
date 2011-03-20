<?php
// $Id: uc_taxes.ca.inc,v 1.1.2.9 2009/02/27 22:41:31 islandusurper Exp $

/**
 * @file
 * This file contains the Conditional Action hooks and functions necessary to
 * make the tax related entity, conditions, events, and actions work.
 */

/******************************************************************************
 * Conditional Action Hooks                                                   *
 ******************************************************************************/

/**
 * Implementation of hook_ca_entity().
 */
function uc_taxes_ca_entity() {
  $entities['tax'] = array(
    '#title' => t('Tax rule'),
    '#type' => 'object',
  );
  return $entities;
}

/**
 * Implementation of hook_ca_trigger().
 *
 * Register an event for each tax rule in {uc_taxes}.
 */
function uc_taxes_ca_trigger() {
  $triggers = array();

  $triggers['calculate_taxes'] = array(
    '#title' => t('Calculate taxes'),
    '#category' => t('Taxes'),
    '#arguments' => array(
      'order' => array('#entity' => 'uc_order', '#title' => t('Order')),
      'tax' => array('#entity' => 'tax', '#title' => t('Tax rule')),
      'user' => array('#entity' => 'user', '#title' => t('User account')),
    ),
  );

  return $triggers;
}

/**
 * Implementation of hook_action_info().
 */
function uc_taxes_ca_action() {
  $actions = array();

  // Loop through all the defined tax rates.
  foreach (uc_taxes_rate_load() as $rate) {
    $actions['uc_taxes_action_apply_tax_'. $rate->id] = array(
      '#title' => t('Charge a tax'),
      '#category' => t('Taxes'),
      '#callback' => 'uc_taxes_action_apply_tax',
      '#arguments' => array(
        'order' => array('#entity' => 'uc_order', '#title' => t('Order')),
        'tax' => array('#entity' => 'tax', '#title' => t('Tax')),
      ),
    );
  }

  return $actions;
}

/**
 * Workflow-ng action callback to calculate a tax.
 *
 * @param $order
 *   The order object being considered.
 * @param $tax
 *   The tax rule calculating the amount.
 * @return
 *   The line item array representing the amount of tax.
 */
function uc_taxes_action_apply_tax($order, $tax) {
  $amount = 0;
  $taxable_amount = 0;
  if (is_array($order->products)) {
    foreach ($order->products as $item) {
      $node = node_load($item->nid);

      // Special handling for manually added "Blank line" products.
      if (!$node) {
        $node = new stdClass();
        $node->type = 'blank-line';
        $node->shippable = $item->weight > 0;
      }

      // Tax products if they are of a taxed type and if it is shippable if
      // the tax only applies to shippable products.
      if (in_array($node->type, $tax->taxed_product_types) && ($tax->shippable == 0 || $node->shippable == 1)) {
        $taxable_amount += $item->price * $item->qty;
      }
    }
  }
  $taxed_line_items = $tax->taxed_line_items;
  if (is_array($order->line_items) && is_array($taxed_line_items)) {
    foreach ($order->line_items as $key => $line_item) {
      if ($line_item['type'] == 'tax' && $line_item['title'] == $tax->name) {
        // Don't tax yourself.
        continue;
      }
      if (in_array($line_item['type'], $taxed_line_items)) {
        $taxable_amount += $line_item['amount'];
      }
    }
  }
  if (isset($taxed_line_items['tax'])) {
    foreach ($order->taxes as $other_tax) {
      $taxable_amount += $other_tax->amount;
    }
  }
  $amount = $taxable_amount * $tax->rate;
  if ($amount) {
    $line_item = (object)array('id' => $tax->id, 'name' => $tax->name, 'amount' => $amount, 'weight' => $tax->weight);
    $line_item->data = array(
      'tax_rate' => $tax->rate,
      'taxable_amount' => $taxable_amount,
      'tax_jurisdiction' => $tax->name,
    );
    return $line_item;
  }
}

/**
 * Implementation of hook_ca_predicate().
 *
 * Create a predicate for each event corresponding to a tax rule.
 */
function uc_taxes_ca_predicate() {
  $predicates = array();

  // Loop through all the defined tax rates.
  foreach (uc_taxes_rate_load() as $rate) {
    $predicates['uc_taxes_'. $rate->id] = array(
      '#title' => $rate->name,
      '#class' => 'taxes',
      '#trigger' => 'calculate_taxes',
      '#status' => 1,
      '#actions' => array(
        array(
          '#name' => 'uc_taxes_action_apply_tax_'. $rate->id,
          '#title' => t('Apply !name', array('!name' => $rate->name)),
          '#argument_map' => array(
            'order' => 'uc_order',
            'tax' => 'tax',
          ),
        ),
      ),
    );
  }

  return $predicates;
}
