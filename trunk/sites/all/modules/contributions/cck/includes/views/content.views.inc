<?php
// $Id: content.views.inc,v 1.1.2.9 2008/10/04 13:14:21 karens Exp $

/**
 * @file
 * Interface between content.module and views.module.
 */

// Include the files defining the classes we extend.
// This is needed in case the /cck folder lives in the main
// /modules folder (views_module_include() will then load
// content.views.inc before node.views.inc)
module_load_include('inc', 'views', 'modules/node.views');

/**
 * Implementation of hook_views_handlers().
 */
function content_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'content') . '/includes/views/handlers',
    ),
    'handlers' => array(

      // argument handlers
      'content_handler_argument' => array(
        'parent' => 'views_handler_argument',
      ),
      'content_handler_argument_string' => array(
        'parent' => 'views_handler_argument_string',
      ),
      'content_handler_argument_numeric' => array(
        'parent' => 'views_handler_argument_numeric',
      ),
      'content_handler_argument_many_to_one' => array(
        'parent' => 'views_handler_argument_many_to_one',
      ),

      // field handlers
      'content_handler_field' => array(
        'parent' => 'views_handler_field_node',
      ),
      'content_handler_field_multiple' => array(
        'parent' => 'content_handler_field',
      ),

      // filter handlers
      'content_handler_filter_string' => array(
        'parent' => 'views_handler_filter_string',
      ),
      'content_handler_filter_numeric' => array(
        'parent' => 'views_handler_filter_numeric',
      ),
      'content_handler_filter_float' => array(
        'parent' => 'views_handler_filter_float',
      ),
      'content_handler_filter_many_to_one' => array(
        'parent' => 'views_handler_filter_many_to_one',
      ),

      // relationship handlers
      'content_handler_relationship' => array(
        'parent' => 'views_handler_relationship',
      ),

      // sort handlers
      'content_handler_sort' => array(
        'parent' => 'views_handler_sort',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_plugins.
 *
 * Defines some plugins used by the Views modes for
 * nodereference and userreference.
 */
function content_views_plugins() {
  $plugins = array(
    'module' => 'content', // This just tells our themes are elsewhere.
    'display' => array(
      'content_simple' => array(
        'path' => drupal_get_path('module', 'content') . '/includes/views/handlers',
        // Those strings are not translated for now.
        // We'll need to change that if / when we remove 'no ui'
        'title' => 'Simple', // TODO: better name ? (currently not displayed anyway)
        'help' => 'Destination-agnostic display. Mostly useful for programmatic views.',
        'handler' => 'content_plugin_display_simple',
        'no ui' => TRUE, // Programmatic use only.
        'uses hook menu' => FALSE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'accept attachments' => FALSE,
      ),
    ),
    'style' => array(
      'content_php_array_autocomplete' => array(
        'path' => drupal_get_path('module', 'content') . '/includes/views/handlers',
        // Those strings are not translated for now.
        // We'll need to change that if / when we remove 'no ui'
        'title' => 'Results array (with title)',
        'help' => 'Returns the view as a PHP array of names + rendered rows.',
        'handler' => 'content_plugin_style_php_array_ac',
        'no ui' => TRUE, // Programmatic use only.
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'type' => 'content_simple',
        'even empty' => TRUE,
      ),
    ),
  );
  return $plugins;
}

/**
 * Implementation of hook_views_data().
 *
 * Exposes all fields to the views system.
 */
function content_views_data() {
  $data = array();
  foreach (content_fields() as $field) {
    $module = $field['module'];
    $result = module_invoke($module, 'field_settings', 'views data', $field);
    if (empty($result)) {
      $result = content_views_field_views_data($field);
    }
    if (is_array($result)) {
      $data = array_merge($data, $result);
    }
  }
  return $data;
}


function content_views_field_views_data($field) {
  $field_types = _content_field_types();

  // Check the field module is available.
  // TODO: is this really how we should do it ?
  if (isset($field_types[$field['type']])) {
    $db_info = content_database_info($field);

    // Field modules that do not store data in the database
    // should not create views data tables.
    if (empty($db_info['columns'])) {
      return;
    }

    $table_alias = content_views_tablename($field);

    $types = array();
    foreach (content_types() as $type) {
      if (isset($type['fields'][$field['field_name']])) {
        $types[] = $type['name'];
      }
    }

    $data = array();
    $data['table']['group']  = t('Content');
    $data['table']['join']['node'] = array(
      'table' => $db_info['table'],
      'left_field' => 'vid',
      'field' => 'vid',
    );

    $columns = array();
    $arguments = array();
    $filters = array();
    foreach ($db_info['columns'] as $column => $attributes) {
      $columns[] = $attributes['column'];
      $sorts[] = !empty($attributes['sortable']) ? TRUE : FALSE;

      // Identify likely filters and arguments for each column based on field type.
      switch ($attributes['type']) {
        case 'int':
        case 'mediumint':
        case 'tinyint':
        case 'bigint':
        case 'serial':
          $filters[] = 'content_handler_filter_numeric';
          $arguments[] = 'content_handler_argument_numeric';
          break;
        case 'numeric':
        case 'float':
          $filters[] = class_exists('views_handler_filter_float') ? 'content_handler_filter_float' : 'content_handler_filter_numeric';
          $arguments[] = 'content_handler_argument_numeric';
          break;

        case 'text':
        case 'blob':
          // TODO add markup handlers for these types
        default:
          $filters[] = 'content_handler_filter_string';
          $arguments[] = 'content_handler_argument_string';
          break;
      }
    }

    // Ensure all columns are retrieved,
    $additional_fields = drupal_map_assoc($columns);

    $data[$columns[0]] = array(
      'group' => t('Content'),
      'title' => t($field_types[$field['type']]['label']) .': '. t($field['widget']['label']) . ' ('. $field['field_name'] .')',
      'help' =>  t($field_types[$field['type']]['label']) .' - '. t('Appears in: @types', array('@types' => implode(', ', $types))),
      'field' => array(
        'field' => $columns[0],
        'table' => $db_info['table'],
        'handler' => 'content_handler_field_multiple',
        'click sortable' => $sorts[0],
        'additional fields' => $additional_fields,
        'content_field_name' => $field['field_name'],
        'allow empty' => TRUE,
        // Access control modules should implement content_views_access_callback().
        'access callback' => 'content_views_access_callback',
        'access arguments' => array($field),
      ),
      'argument' => array(
        'field' => $columns[0],
        'table' => $db_info['table'],
        'handler' => $arguments[0],
        'click sortable' => $sorts[0], // TODO used in once place in node.views.inc, should we use it here?
        'name field' => '', // TODO
        'additional fields' => $additional_fields,
        'content_field_name' => $field['field_name'],
        'empty field name' => t('<No value>'),
        'allow empty' => TRUE,
      ),
      'filter' => array(
        'field' => $columns[0],
        'title' => t($field['widget']['label']),
        'table' => $db_info['table'],
        'handler' => $filters[0],
        'additional fields' => $additional_fields,
        'content_field_name' => $field['field_name'],
        'allow empty' => TRUE,
      ),
    );
    // TODO do we need different handling for sorts with Views 2,
    // especially when relationships are involved?
    if (!empty($sorts[0])) {
      $data[$columns[0]]['sort'] = array(
        'field' => $columns[0],
        'table' => $db_info['table'],
        'handler' => 'content_handler_sort',
        'additional fields' => $additional_fields,
        'content_field_name' => $field['field_name'],
        'allow empty' => TRUE,
      );
    }
    // TODO: provide automatic filters, sorts, and arguments for each column, not just the first?
    return array($table_alias => $data);
  }
}

/**
 * Helper function so it is possible to change the Views tablename
 * in the future without re-writing code.
 */
function content_views_tablename($field) {
  return 'node_data_'. $field['field_name'];
}

function theme_content_view_multiple_field($items, $field, $values) {
  $output = '';
  foreach ($items as $item) {
    if (!empty($item) || $item == '0') {
      $output .= '<div class="field-item">'. $item .'</div>';
    }
  }
  return $output;
}