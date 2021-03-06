<?php
// $Id: calendar_plugin_style.inc,v 1.1.2.1 2008/09/05 11:03:42 karens Exp $
/**
 * Style plugin to create the calendar navigation and links.
 * 
 * Used by the main calendar page and calendar block displays.
 */
class calendar_plugin_style extends views_plugin_style {
  /**
   * Init will be called after construct, when the plugin is attached to a
   * view and a display. 
   */
  function init(&$view, &$display) {
    parent::init($view, $display);
    $view->display_types = $this->display_types();
  }
    
  function display_types($granularity = NULL, $option_type = 'names') {
    $ids = array();
    $names = array();
    foreach (calendar_display_types() as $name => $type) {
      foreach ($this->view->display as $id => $display) {
        if (in_array($display->display_plugin, array('calendar_year', 'calendar_month', 'calendar_day', 'calendar_week'))) {
          if ($display->display_plugin == 'calendar_'. $name) {
            $attachments = array_filter($display->display_options['displays']);
            if (isset($attachments['calendar'])) {
              $ids[$name] = $id;
              $names[$name] = $display->display_title;
            }
          }
        }
      }
    }
    if ($granularity) {
      return $$option_type[$granularity];
    }
    return $$option_type;
  }
  
  /**
   * Calendar argument date fields used in this view.
   */
  function date_fields() {
    $date_fields = array();
    $calendar_fields = date_api_fields();
    $arguments = $this->display->handler->get_option('arguments');
    foreach ($arguments as $name => $argument) {
      if (isset($argument['date_fields'])) {
        foreach ($argument['date_fields'] as $date_field) {
          $field = $calendar_fields['name'][$date_field];
          $handler = views_get_handler($field['table_name'], $field['field_name'], 'field');
          if ($handler) {
            $date_fields[$date_field] = $field;
            $date_fields[$date_field]['name'] = $handler->ui_name();
          }
        }
      }
    }
    return ($date_fields);
  }
  
  /**
   * Style validation.
   */
  function validate() {
    $errors = parent::validate();
    
    $arguments = $this->display->handler->get_option('arguments');
    if (!in_array('date_argument', array_keys($arguments))) {
      $errors[] = t('Style @style will not work without the Date argument.', array('@style' => $this->definition['title']));      
    }
    elseif ($arguments['date_argument']['default_action'] != 'default' || $arguments['date_argument']['default_argument_type'] != 'date') {
      $errors[] = t('The Date argument must be set up to provide a default value set to the current date.');      
    }
      
    
    // TODO fix the following validation code to work correctly in Views2.
  
    // CCK date fields cannot use grouped handler.
    //$calendar_fields = array_keys(date_api_fields());
    //foreach ($view['field'] as $delta => $field) {
    //  if (in_array($field['field_name'], $calendar_fields) && $field['id'] == 'content_views_field_handler_group') {
        //form_error($form['field_name'][$delta], t('Calendar CCK Date fields must be set to \'Do not group multiple values\'.'));
    //  }
    //}
    return $errors;
  }

  function query() {
    
    require_once('./'. drupal_get_path('module', 'date_api') .'/date_api_sql.inc');
        
    $style_options = $this->view->style_plugin->options;
    
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
        $this->view->min_date_date = date_format($this->view->min_date, DATE_FORMAT_DATE);
        $this->view->max_date_date = date_format($this->view->max_date, DATE_FORMAT_DATE);
      }
      $i++;
    }
    $this->view->display_types = $this->display_types();
    $keys = drupal_map_assoc(array_keys(calendar_display_types()));
    $this->view->calendar_display = $keys[$this->view->granularity];
           
    // bring the node type into the query so we can use it in the theme
    $this->view->query->add_field('node', 'type');
      
    parent::query();
  }
  
  /**
   * Render the calendar navigation style.
   */
  function render() {
    return theme($this->theme_functions(), $this->view, $this->options, array());
  }
}

