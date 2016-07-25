<?php

/**
 * @file
 * Custom Drupal 6 RequestMehod class for Google reCAPTCHA library.
 */

namespace ReCaptcha\RequestMethod;

use ReCaptcha\RequestMethod;
use ReCaptcha\RequestParameters;

/**
 * Sends POST requests to the reCAPTCHA service.
 */
class Drupal6Post implements RequestMethod {

  /**
   * URL to which requests are POSTed.
   * @const string
   */
  const SITE_VERIFY_URL = 'https://www.google.com/recaptcha/api/siteverify';

  /**
   * Submit the POST request with the specified parameters.
   *
   * @param RequestParameters $params Request parameters
   * @return string Body of the reCAPTCHA response
   */
  public function submit(RequestParameters $params) {

    $response = drupal_http_request(self::SITE_VERIFY_URL, array('Content-type' => 'application/x-www-form-urlencoded'), 'POST', $params->toQueryString());

    return isset($response->data) ? $response->data : '';
  }
}
