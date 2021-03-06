<?php
// $Id: content_handler_field.inc,v 1.1.2.8 2008/10/09 14:31:15 karens Exp $

/**
 * @file
 * The subclass adds basic field and formatter info,
 * for field-specific subclasses to use if they need to.
 *
 * Fields could extend this class if they want field and formatter handling
 * but don't want the multiple value grouping options created by
 * content_handler_field_multiple.
 */
class content_handler_field extends views_handler_field_node {
  var $content_field;

  function construct() {
    parent::construct();
    $this->content_field = content_fields($this->definition['content_field_name']);
  }

  function options(&$options) {
    parent::options($options);
    $field = $this->content_field;

    // Override views_handler_field_node's default label
    $options['label'] = '';
    $options['label_type'] = 'widget';
    $options['format'] = 'default';
  }

  /**
   * Provide formatter option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // TODO: do we want the 'link to node' checkbox ?
    // That's usually formatters business...

    $field = $this->content_field;
    $options = $this->options;

    $form['label_type'] = array(
      '#title' => t('Label'),
      '#type' => 'radios',
      '#options' => array(
        'none' => t('None'),
        'widget' => t('Widget label (@label)', array('@label' => $field['widget']['label'])),
        'custom' => t('Custom'),
      ),
      '#default_value' => $options['label_type'],
      '#weight' => 2,
    );
    $form['label'] = array(
      '#title' => t('Custom label'),
      '#type' => 'textfield',
      '#default_value' => $options['label'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array('radio:options[label_type]' => array('custom')),
      '#weight' => 3,
     );

    $field_types = _content_field_types();
    $formatters = array();
    if (is_array($field_types[$field['type']]['formatters'])) {
      foreach ($field_types[$field['type']]['formatters'] as $name => $info) {
        $formatters[$name] = $info['label'];
      }
    }
    $form['format'] = array(
      '#title' => t('Format'),
      '#type' => 'select',
      '#options' => $formatters,
      '#required' => TRUE,
      '#default_value' => $options['format'],
      '#weight' => 4,
    );
  }

  
  /**
   * Make sure some value is stored as a label.
   * 
   * Don't use t(), since Views' views_handler_field already has 
   * $this->options['label'] marked as a translatable field.
   * 
   * @see http://drupal.org/node/285470
   */
  function options_submit($form, &$form_state) {
    switch ($form_state['values']['options']['label_type']) {
      case 'none':
        $form_state['values']['options']['label'] = '';
        break;
      case 'widget':
        $form_state['values']['options']['label'] = $this->content_field['widget']['label'];
        break;
    }
  }

  /**
   * @TODO 
   * Now that we save the label in the submit process above we could 
   * get rid of this function. Leave it here for now to be sure the 
   * label works for fields that haven't been updated since this
   * change was made, since $this->options['label'] will be missing a 
   * value until it is updated in the view.
   * 
   * Don't use t(), since Views' views_handler_field already has 
   * $this->options['label'] marked as a translatable field.
   */
  function label() {
    $field = $this->content_field;
    switch ($this->options['label_type']) {
      case 'none':
        return '';
      case 'widget':
        return $field['widget']['label'];
      default:
        return $this->options['label'];
    }
  }

  function options_validate($form, &$form_state) { }

  /**
   * Provide text for the administrative summary
   */
  function admin_summary() {
    // Display the formatter name.
    $field = $this->content_field;
    $field_types = _content_field_types();
    if (isset($field_types[$field['type']]['formatters'][$this->options['format']])) {
      return t($field_types[$field['type']]['formatters'][$this->options['format']]['label']);
    }
  }

  function render($values) {
    $field = $this->content_field;
    $options = $this->options;
    $db_info = content_database_info($field);

    // $values will be used as a fake $node object;
    // we provide a build_mode for rendering.
    $values->build_mode = NODE_BUILD_NORMAL;

    $item = array();
    foreach ($db_info['columns'] as $column => $attributes) {
      $item[$column] = $values->{$this->table_alias .'_'. $attributes['column']};
    }
    $item['#delta'] = !empty($values->{$this->table_alias . '_delta'}) ? $values->{$this->table_alias . '_delta'} : 0;
    return $this->render_link(content_format($field, $item, $options['format'], $values), $values);
  }

}