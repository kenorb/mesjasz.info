<?php
// $Id: weblinks_errors.inc,v 1.1.2.2 2008/08/21 01:06:51 nancyw Exp $

/**
 * @file
 *   HTTP Error codes
 */
function weblinks_error_codes() {
  return array(
    '100' => t('100 - Continue'),
    '101' => t('101 - Switching Protocols'),
    '200' => t('200 - OK'),
    '201' => t('201 - Created'),
    '202' => t('202 - Accepted'),
    '203' => t('203 - Non-Authoritative Information'),
    '204' => t('204 - No Content'),
    '205' => t('205 - Reset Content'),
    '206' => t('206 - Partial Content'),
    '300' => t('300 - Multiple Choices'),
    '301' => t('301 - Moved Permanently'),
    '302' => t('302 - Found'),
    '303' => t('303 - See Other'),
    '304' => t('304 - Not Modified'),
    '305' => t('305 - Use Proxy'),
    '306' => t('306 - (Unused)'),
    '307' => t('307 - Temporary Redirect'),
    '400' => t('400 - Bad Request'),
    '401' => t('401 - Unauthorized'),
    '402' => t('402 - Payment Required'),
    '403' => t('403 - Forbidden'),
    '404' => t('404 - Not Found'),
    '405' => t('405 - Method Not Allowed'),
    '406' => t('406 - Not Acceptable'),
    '407' => t('407 - Proxy Authentication Required'),
    '408' => t('408 - Request Timeout'),
    '409' => t('409 - Conflict'),
    '410' => t('410 - Gone'),
    '411' => t('411 - Length Required'),
    '412' => t('412 - Precondition Failed'),
    '413' => t('413 - Request Entity Too Large'),
    '414' => t('414 - Request-URI Too Long'),
    '415' => t('415 - Unsupported Media Type'),
    '416' => t('416 - Requested Range Not Satisfiable'),
    '417' => t('417 - Expectation Failed'),
    '500' => t('500 - Internal Server Error'),
    '501' => t('501 - Not Implemented'),
    '502' => t('502 - Bad Gateway'),
    '503' => t('503 - Service Unavailable'),
    '504' => t('504 - Gateway Timeout'),
    '505' => t('505 - HTTP Version Not Supported'),
    );
}
