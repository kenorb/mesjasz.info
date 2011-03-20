<?php
// $Id: simplenews_handler_filter_newsletter_s_status.inc,v 1.1.2.2 2008/09/14 13:28:28 sutharsan Exp $

/**
 * Filter bases on newsletter send status.
 */
class simplenews_handler_filter_newsletter_s_status extends views_handler_filter_in_operator {
  function get_value_options() {
    $this->value_options = array(
      SIMPLENEWS_STATUS_SEND_NOT => t('Not send'),
      SIMPLENEWS_STATUS_SEND_PENDING => t('Pending'),
      SIMPLENEWS_STATUS_SEND_READY => t('Send'),
    );
  }
}
