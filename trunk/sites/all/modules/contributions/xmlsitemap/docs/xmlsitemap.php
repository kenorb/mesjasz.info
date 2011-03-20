<?php
// $Id: xmlsitemap.php,v 1.5 2008/05/20 17:48:46 darrenoh Exp $

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Define additional links to add to the site map.
 *
 * This hook allows modules to add additional links to the site map. Links
 * may be associated with nodes, terms, or users, as shown in the example.
 * @param $type:
 * If set, a string specifying the type of additional links to return. You
 * can use your own type or a type from one of the included modules:
 * - node:
 *   Links associated with nodes
 * - term:
 *   Links associated with terms
 * - user:
 *   Links associated with users
 * - xml:
 *   An XML site map (for including site maps from other modules)
 * You can define additional types by adding them to the switch statement.
 * @param $excludes:
 * Depends on the type of links being requested.
 * - For "node", an array of excluded node types
 * - For "term", an array of excluded vocabularies
 * - For "user", an array of included roles
 * @return
 * If $type is xml, return an XML site map. Otherwise, return an array of
 * links or an empty array. Each link should be an array with the
 * following keys:
 * - nid, tid, uid, or custom ID type:
 *   ID to associate with this link (If you have defined your own link type,
 *   use the ID key to group related links together.)
 * - #loc:
 *   The URL of the page
 * - #lastmod:
 *   Timestamp of last modification
 * - #changefreq:
 *   Number of seconds between changes
 * - #priority:
 *   A number between 0 and 1 indicating the link's priority
 */
function hook_xmlsitemap_links($type = NULL, $excludes = array()) {
  $links = array();
  switch ($type) {
    case 'node':
      break;
    case 'term':
      break;
    case 'user':
      // Load profiles.
      $result = db_query("
        SELECT u.uid, xu.last_changed, xu.previously_changed, xu.priority_override, SUM(xur.priority), ua.dst AS alias
        FROM {users} u
        LEFT JOIN {users_roles} ur ON ur.uid = u.uid
        LEFT JOIN {xmlsitemap_user_role} xur ON xur.rid = ur.rid
        LEFT JOIN {xmlsitemap_user} xu ON xu.uid = u.uid
        LEFT JOIN {url_alias} ua ON ua.pid = xu.pid
        WHERE (xu.priority_override IS NULL OR xu.priority_override >= 0) AND u.uid <> %d
        GROUP BY u.uid, xu.last_changed, xu.previously_changed, xu.priority_override, ua.dst
        HAVING MIN(xur.priority) <> -1
      ", _xmlsitemap_user_frontpage());
      // Create link array for each profile.
      while ($user = db_fetch_object($result)) {
        $age = time() - $user->last_changed;
        $interval = empty($user->previously_changed) ? 0 : $user->last_changed - $user->previously_changed;
        $links[] = array(
          'uid' => $user->uid,
          '#loc' => xmlsitemap_url("user/$user->uid", $user->alias, NULL, NULL, TRUE),
          '#lastmod' => $user->last_changed,
          '#changefreq' => max($age, $interval),
          '#priority' => _xmlsitemap_user_priority($user),
        );
      }
      // Add other user links to the links array.
      $links = array_merge($links, module_invoke_all('xmlsitemap_links', 'user'));
      // Sort links by user ID and URL.
      foreach ($links as $key => $link) {
        $uid[$key] = $link['uid'];
        $loc[$key] = $link['#loc'];
      }
      array_multisort($uid, $loc, $links);
      break;
    case 'xml':
      // Retrieve an XML site map.
      $links = example_sitemap();
      break;
    default:
      // Add arbitrary additional links.
      $result = db_query("
        SELECT xa.*, ua.dst AS alias FROM {xmlsitemap_additional} xa
        LEFT JOIN {url_alias} ua ON xa.pid = ua.pid
      ");
      while ($link = db_fetch_object($result)) {
        $age = time() - $link->last_changed;
        if (!empty($link->previously_changed)) {
          $interval = $link->last_changed - $link->previously_changed;
        }
        else {
          $interval = 0;
        }
        $entry = array(
          '#loc' => xmlsitemap_url($link->path, $link->alias, NULL, NULL, TRUE),
          '#lastmod' => $link->last_changed,
          '#changefreq' => max($age, $interval),
          '#priority' => $link->priority,
        );
        $additional[] = $entry;
      }
      break;
  }
  return $links;
}

/**
 * Define actions for search engines.
 * @param $op:
 * - form:
 *   Add search engine to form at admin/settings/xmlsitemap.
 * - ping:
 *   Submit site map to search engine.
 * - access:
 *   Log search engine access.
 * @param $type:
 * If $op is 'access', one of the following strings will indicate what was
 * downloaded:
 * - Site map:
 *   The site map was downloaded.
 * - Site map index
 *   The site map index was downloaded.
 * - Site map $chunk
 *   Chunk $chunk was downloaded.
 * @return
 * - form:
 *   Array of form elements for search engine settings
 * - ping:
 *   None
 * - access:
 *   Message string for access log
 */
function hook_xmlsitemap_engines($op, $type = NULL) {
  switch ($op) {
    case 'form':
      $form['google'] = array(
        '#type' => 'fieldset',
        '#title' => t('Google'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['google']['xmlsitemap_engines_google_submit'] = array(
        '#type' => 'checkbox',
        '#title' => t('Submit site map to Google.'),
        '#default_value' => variable_get('xmlsitemap_engines_google_submit', TRUE),
      );
      $form['google']['xmlsitemap_engines_google_url'] = array(
        '#type' => 'textfield',
        '#title' => t('Submission URL'),
        '#default_value' => variable_get('xmlsitemap_engines_google_url', 'http://www.google.com/webmasters/tools/ping?sitemap='. xmlsitemap_url('sitemap.xml', drupal_lookup_path('alias', 'sitemap.xml') ? drupal_lookup_path('alias', 'sitemap.xml') : NULL, NULL, NULL, TRUE)),
        '#description' => t('The URL to submit the site map to.'),
      );
      $form['google']['xmlsitemap_engines_google_verify'] = array(
        '#type' => 'textfield',
        '#title' => t('Verification link'),
        '#default_value' => variable_get('xmlsitemap_engines_google_verify', ''),
        '#description' => t('In order to show statistics, Google will ask you to verify that you control this site by creating a file with a certain name. Enter that name here and the XML Sitemap module will create a path to that file name. This will only work if you have clean URLs enabled.'),
      );
      return $form;
    case 'ping':
      if (variable_get('xmlsitemap_engines_google_submit', TRUE)) {
        $result = drupal_http_request(variable_get('xmlsitemap_engines_google_url', 'http://www.google.com/webmasters/tools/ping?sitemap='. xmlsitemap_url('sitemap.xml', drupal_lookup_path('alias', 'sitemap.xml') ? drupal_lookup_path('alias', 'sitemap.xml') : NULL, NULL, NULL, TRUE)));
        if ($result->code == 200) {
          watchdog('xmlsitemap', 'Sitemap successfully submitted to Google.');
        }
        else {
          watchdog('xmlsitemap', 'Error occurred submitting sitemap to Google: @code', array('@code' => $result->code), WATCHDOG_ERROR);
        }
      }
      break;
    case 'access':
      if (strpos($_SERVER['HTTP_USER_AGENT'], 'Googlebot') !== FALSE) {
        return t('!sitemap downloaded by Google.', array('!sitemap' => $type));
      }
      break;
  }
}

/**
 * @} End of "addtogroup hooks".
 */

