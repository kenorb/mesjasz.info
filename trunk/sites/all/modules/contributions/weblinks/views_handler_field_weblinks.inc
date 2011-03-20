<?php
// $Id: views_handler_field_weblinks.inc,v 1.1.2.2 2008/11/19 01:22:06 nancyw Exp $
/**
 * @file
 * Provide views data and handlers for weblinks.module
 */

/**
 * Field handler to provide an embedded image.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_weblinks extends views_handler_field {

  /**
   * Define options available for this field.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['link_type'] = array('default' => 'url');

    return $options;
  }

  /**
   * Build option configuration form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_type'] = array(
      '#title' => t('Link points to URL or node'),
      '#type' => 'select',
      '#options' => array('url' => 'URL', 'node' => 'Node'),
      '#default_value' => !empty($this->options['link_type']),
    );
  }

  /**
   * Render field output to the browser.
   */
  function render($values) {
    $type = $values->{$this->field_alias};
    if (!$type) {
      return;
    }

    if ($type == 'url') {
      $link = l($values->weblinks_url, $values->weblinks_url);
    }
    else {
      $link = l($values->weblinks_url, 'node/'. $values->nid);
    }

    return $link;
  }
}
