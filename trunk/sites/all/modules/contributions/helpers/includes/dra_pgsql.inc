<?php
/* $Id */
 /**
 * Return a result array from the previous query. PostgreSql version.
 * This is very handy for building an option list for a form element.
 *
 * @param $result
 *   A database query result resource, as returned from db_query().
 * @return
 *   The resulting array or FALSE.
 *   If the query contains -- the result array would be 
 *        0 columns             (bool)FALSE
 *        1 column              value => value
 *        2 columns             1st value => 2nd value
 *        3 or more             1st value => array(2nd value, 3rd value, ...)
 */
function db_result_array($result) {
  $array = array();
  while ($row = mysql_fetch_array($result, MYSQL_NUM)) {
    $y = count($row);
    switch ($y) {
      case 0:
        drupal_set_message(t('Db_result_array found no columns in the result set.'), 'error');
        return false;

      case 1:
        $array[$row[0]] = $row[0];
        break;

      case 2:
        $array[$row[0]] = $row[1];
        break;

      default:
        $array[$row[0]] = array_slice($row, 1);
        break;
    }
  }
  return $array;
}
