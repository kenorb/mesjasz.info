<?php
// $Id: calendar_plugin_style_ical.inc,v 1.1.2.1 2008/09/05 11:03:42 karens Exp $
/**
 * Default style plugin to render an iCal feed.
 */
class calendar_plugin_style_ical extends views_plugin_style_rss {
  function init(&$view, &$display) {
    parent::init($view, $display);
    $fields = $display->handler->default_display->options['fields'];
    $this->options['fields'] = $fields;
  }
  
  function query() {
    // We need these values for the iCal feed.
    $this->view->query->add_field('node', 'title');
    $this->view->query->add_field('node', 'type');
    parent::query();
  }
  
  function attach_to($display_id, $path, $title) {
    $display = $this->view->display[$display_id]->handler;
    $url_options = array();
    if (!empty($view->exposed_raw_input)) {
      $url_options['query'] = $view->exposed_raw_input;
    }

    $url = url($this->view->get_url(NULL, $path), $url_options);
    if (!$display->has_path()) {
      if (empty($this->preview)) {
        calendar_ical_add_feed($url, $title);
      }
    }
    else {
      if (empty($this->view->feed_icon)) {
        $this->view->feed_icon = '';
      }
      
      $this->view->feed_icon .= theme('calendar_ical_icon', $url, $title);
    }
  }

  /**
   * Set default options
   */
  function options(&$options) {
    parent::options($options);
    $options['summary_field'] = 'node_title';
    $options['description_field'] = '';
    $options['location_field'] = '';
    $options['fields'] = array();
  }
  
  function option_definition() {
    $options = parent::option_definition();

    $options['summary_field'] = array('default' => '', 'translatable' => TRUE);
    $options['description_field'] = array('default' => '', 'translatable' => TRUE);
    $options['location_field'] = array('default' => '', 'translatable' => TRUE);
    
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $options = array('' => '');
    foreach ($this->options['fields'] as $field) {
      $handler = views_get_handler($field['table'], $field['field'], 'field');
      $alias = $field['table'] .'_'. $field['field'];
      $options[$alias] = $handler->ui_name();
    }
    $form['#prefix'] = t('<div class="form-item">Map the View fields to the values they should represent in the iCal feed.</div>');
    
    $form['summary_field'] = array(
      '#type' => 'select',
      '#title' => t('Title'),
      '#default_value' => !empty($this->options['summary_field']) ? $this->options['summary_field'] : 'node_title',
      '#options' => $options,
      );
    $form['description_field'] = array(
      '#type' => 'select',
      '#title' => t('Description'),
      '#default_value' => $this->options['description_field'],
      '#options' => $options,
      );
    $form['location_field'] = array(
      '#type' => 'select',
      '#title' => t('Location'),
      '#default_value' => $this->options['location_field'],
      '#options' => $options,
      );
      
  }

  function render() {
    require_once('./'. drupal_get_path('module', 'calendar') .'/includes/calendar.inc');
    
    // Transfer the style options to the view object so they
    // can be easily accessed in the theme.
    $style_options = $this->options;
    $this->view->summary_field = $style_options['summary_field'];
    $this->view->description_field = $style_options['description_field'];
    $this->view->location_field = $style_options['location_field'];
    
    // Evaluate our argument values and figure out which 
    // calendar display we need to create.
    $i = 0;   
    foreach ($this->view->argument as $id => $argument) {
      if ($argument->field == 'date_argument') {
        // TODO Decide if we want to provide a date here or not.
        // Adding this now is to prevent fatal errors later if the
        // view is used in unexpected ways without a date being set.
        if (empty($argument->min_date)) {
          $value = $argument->get_default_argument();
          $range = $argument->date_handler->arg_range($value);
          $argument->min_date = $range[0];
          $argument->max_date = $range[1];
        }
        $this->view->granularity = !empty($argument->granularity) ? $argument->granularity : $argument->options['granularity'];
        $this->view->date_arg = !empty($this->view->args) ? $this->view->args[$argument->position] : '';
        $this->view->date_arg_pos = $i;
        $this->view->year = isset($argument->year) ? $argument->year : NULL;
        $this->view->month = isset($argument->month) ? $argument->month: NULL;
        $this->view->day = isset($argument->day) ? $argument->day : NULL;
        $this->view->week = isset($argument->week) ? $argument->week : NULL;
        $this->view->min_date = $argument->min_date;
        $this->view->max_date = $argument->max_date;
      }
      $i++;
    }
    
    // Render each field into an output array.
    $items = array();
    $calendar_fields = date_api_fields();
    $calendar_fields = array_keys($calendar_fields['alias']);
    foreach ($this->view->result as $num => $row) {
      $items[$num] = $row;
      // Store the raw date values before formatting the results.
      foreach ($row as $key => $value) {
        if (in_array($key, $calendar_fields)) {
          $items[$num]->calendar_fields->$key = $value;
        }
      }
      foreach ($this->view->field as $field) {
        if (!empty($field) && is_object($field)) {
          $field_output = $field->theme($row);
          $items[$num]->{$field->field_alias} = $field_output;
        }
      }
    }

    // Massage the resulting items into formatted calendar items.
    $items = calendar_build_nodes($this->view, $items);
    
    // Merge in items from other sources.
    foreach (module_implements('calendar_add_items') as $module) {
      $function = $module .'_calendar_add_items';
      if (function_exists($function)) {
        if ($feeds = $function($this->view)) {
          foreach ($feeds as $feed) {
            $items = $feed;
          }
        }
      }
    }
    return theme($this->theme_functions(), $this->view, $this->options, $items);
  }
}
