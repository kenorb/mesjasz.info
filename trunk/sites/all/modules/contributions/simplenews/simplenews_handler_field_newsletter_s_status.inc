<?php
// $Id: simplenews_handler_field_newsletter_s_status.inc,v 1.1.2.2 2008/09/14 13:28:28 sutharsan Exp $

/**
 * Display newsletter send status.
 */
class simplenews_handler_field_newsletter_s_status extends views_handler_field {
  function render($values) {
    switch ($values->{$this->field_alias}) {
      case SIMPLENEWS_STATUS_SEND_NOT:
      default:
        return t('Not send');
      case SIMPLENEWS_STATUS_SEND_PENDING:
        return t('Pending');
      case SIMPLENEWS_STATUS_SEND_READY:
        return t('Send');
    }
  }
}
