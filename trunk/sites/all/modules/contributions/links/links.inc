<?php
/** @file
 * $Id: links.inc,v 1.28.2.5 2009/01/21 04:16:33 syscrusher Exp $
 *
 * links.inc      Utility functions for the links modules
 *
 * Author: Scott Courtney (Drupal user "syscrusher") scott@4th.com
 *
 * NOTE: This file is co-dependent with links.module.
 */

/*! @addtogroup links_url URL editing and parsing
 *  @{
 */

/**
 * Certain common GET parameters, such as known session tags (e.g.,
 * PHPSESSID) don't really belong in a URL that is being permanently
 * stored. This function strips out these known-undesirable parms.
 *
 * WARNING: Changing this function, or others that it calls, may cause
 * the MD5 sums of URLs in the database to need to be recalculated. If
 * you make such changes, be sure to make a call to the function
 * _links_recalc_all_url_md5() in the upgrade script.
 */
function links_normalize_url($url) {
//  $url = links_remove_session_id($url);
//  $url = links_remove_empty_get_parms($url);
//  $url = links_check_trailing_slash($url);
//  $url = links_sort_get_parms($url);
  $parsed = links_parse_url($url);
  $url_n  = $parsed['normalized'];
  return $url_n;
}

/**
 * Remove empty GET parameters from a URL or bare query string.
 * For example:
 *    http://example.com/abc.php?this=&that=55&other=
 * becomes
 *    http://example.com/abc.php?that=55
 *
 * TODO: Correctly handle links of the form
 *    http://example.com/abc.xyz?parm=xyz&amp;parm2=123
 * (character entity encoding of ampersand delimiter)
 */
function links_remove_empty_get_parms($url) {
  $url = trim($url);
  // The four consecutive calls are ugly as hell, but the
  // pregexp misses consecutive empth parms otherwise. For
  // some reason, substitutions like \1\2 in the replacement
  // expression don't work as one would expect.
  $url = preg_replace('/([&?])[^=]+=(&|$)/','\1',$url);
  $url = preg_replace('/([&?])[^=]+=(&|$)/','\1',$url);
  $url = preg_replace('/([&?])[^=]+=(&|$)/','\1',$url);
  $url = preg_replace('/([&?])[^=]+=(&|$)/','\1',$url);
  // Special handling for empty parm at beginning of string
  $url = preg_replace('/^([^=]+=)(&.*|$)/','\2',$url);
  return links_cleanup_parms($url);
}

/**
 * Order the GET parameters for a URL. This helps to detect
 * URLs that are the same except for the parameter order.
 * For exammple:
 *    http://example.com/abc.php?xyz=98&def=yes&abc=49
 * becomes:
 *    http://example.com/abc.php?abc=49&def=yes&xyz=98
 *
 * Compare to links_sort_query_string() (which this function calls).
 */
function links_sort_get_parms($url) {
  $url = trim($url);
  $qmark = strpos($url, '?');
  if ($qmark === FALSE || $qmark == (strlen($url)-1)) {
    // No GET parms present
    return $url;
  }
  // Intentionally keep the question mark with this part
  $page = substr($url, 0, $qmark+1);
  $query = substr($url, $qmark+1);
  $url = $page . links_sort_query_string($query);
  return $url;
}

/**
 * Orders the parameters in an HTTP GET query string. This
 * helps to detect URLs that are the same except for the
 * parameter order.
 * For exammple:
 *    xyz=98&def=yes&abc=49
 * becomes:
 *    abc=49&def=yes&xyz=98
 *
 * Accepts either "&" or the W3C-preferred "&amp;" as a delimiter
 * between parameters.
 *
 * Compare to links_sort_get_parms() (which calls this function).
 */
function links_sort_query_string($query) {
  // Handles either "&" or "&amp;" as delimiters, or a mixture.
  $parms = preg_split('/(?:&amp;|&)/i', $query);
  $p_sort = array();
  foreach ($parms as $parm) {
    $eq = strpos($parm, '=');
    $key = ($eq === FALSE) ? $parm : substr($parm, 0, $eq);
    $val = $parm;
    $p_sort[$key] = $val;
  }
  ksort($p_sort);
  // See which delimiter they preferred...believe it or not,
  // &amp; is actually the W3C preference, but most don't
  // use it.
  $delim = strpos($query, '&amp;') === FALSE ? '&' : '&amp;';
  $query2 = implode($delim, $p_sort);
  return $query2;
}

/**
 * This function removes known session ID strings from a URL. These are
 * transient data from a particular browser on a particular day, and
 * definitely do not belong in a links database. The search is not
 * case sensitive. This also works on a bare query string rather than
 * a full URL, and can be used that way.
 */
function links_remove_session_id($url) {
  $url = trim($url);
  $url = preg_replace('/(^|[&?]|&amp;)[0-9A-Z_]*SESS(ION|_ID|ID)*=[0-9A-Z_]*/i','\1',$url);
  return links_cleanup_parms($url);
}

/**
 * If a link consists only of a protocol and hostname, add a trailing
 * slash to explicitly request the default or index document.
 * For example, "http://www.example.com" becomes "http://www.example.com/".
 * This slightly speeds up page retrieval for the visitor, and also keeps
 * from having these non-significant differences reflected in the database.
 */
function links_check_trailing_slash($url) {
  $url = trim($url);
  if (preg_match('!^[A-Z0-9]+://[^/?]+$!i', $url)) {
    $url .= '/';
  }
  return $url;
}

/**
 * The preg_replace() calls in the links_remove_...() functions
 * may leave some cruft in the URL, such as "&&", "&?", or "?" or "&" as
 * the first or last character. This function clears that problem up. It is
 * called by the links_remove_...() functions and may also be
 * of use to external applications.
 */
function links_cleanup_parms($url) {
  $patterns = array('/(\&amp;|\&)(\&amp;|\&)/', '/^(\&amp;|\&)/', '/\?(\&amp;|\&)/', '/(\&amp;|\&|\?)$/', '/(\&amp;|\&)\?/');
  $replaces = array('\1', '', '?', '', '?');
  $url = preg_replace($patterns, $replaces, $url);
  return $url;
}

/**
 * Given a link ID or URL, query the database to suggest a title for this link
 * as it is being associated with a node. Basically, this will obtain the
 * first assigned title (ordered by weight) of any existing node associations
 * for this same link. A default value can be specified in case there's no
 * good hint in the database. Failing all else, this function will return a
 * very stripped down version of the URL itself as a suggested title.
 */
function links_suggest_link_title($link_spec, $default="") {
  // We order the query by weight and then link title, excluding rows where title is empty.
  $sql = links_get_link_node_query_sql($link_spec, "weight,link_title", FALSE);
  $result = db_query_range($sql, 1, 1);
  if (db_error()) {
    watchdog("links", "links database error on query: @sql", array('@sql'=>$sql), WATCHDOG_ERROR);
    return $default;
  }
  $row = db_fetch_array($result);
  if (is_array($row) && !empty($row["link_title"])) {
    return $row["link_title"];
  }
  // At this point, we didn't find anything really good in the database.
  // If the caller specified a default, we'll go with that -- but if not,
  // we still have a few tricks up our sleeve (see below).
  if (! empty($default)) {
    return $default;
  }
  // Try mangling the URL to make something halfway friendly...
  $linkrec = links_get_link($link_spec);
  $url = is_array($linkrec) ? $linkrec["url"] : '';
  if (empty($url) && ! is_int($link_spec)) {
    // Sigh...couldn't look it up...but can use the link spec itself
    $url = $link_spec;
  }
  if (! empty($url)) {
    $url_split = explode('?', links_normalize_url($url));
    $pre_parms = $url_split[0];
    $title = empty($url_split[1]) ? $pre_parms : $pre_parms . "?...";
    // Trim that thing to a reasonable size
    if (strlen($title) > 80) {
      $title = substr($title, 0, 75) . '...';
    }
  } else {
    // Method of last resort
    $title = t("Web Link: ").$link_spec;
  }
  return $title;
}

/**
 * Checks the provided URL for valid syntax, and returns an associative array
 * to report the findings. The array contains the following elements:
 *
 * 'url'        => The original URL as passed to the function
 * 'normalized' => The normalized version of the URL, which is also
 *                 validated after normalization. This will be filled
 *                 even if the source URL is not valid, though in that
 *                 situation its results are undefined.
 * 'valid'      => Boolean TRUE or FALSE to indicate overall status
 * 'errors'     => A sub-array containing one or more error messages
 *                 which the caller may optionally provide to the user.
 *                 These have NOT been translated with t() before
 *                 being returned; that is up to the calling application.
 *                 This element will be UNSET if there were no errors.
 * 'debug'      => A sub-array containing diagnostics about the validation
 *                 process. Used for testing; each element will be a string,
 *                 though these are for programmers and not end users.
 * 'scheme'     => The URL scheme, if present or implied by type (based
 *                 on the result URL, not the original), without trailing colon
 * 'host'       => The server name (host and/or domain), also set for
 *                 bare email addresses if appropriate options enabled
 * 'port'       => Optional port number, if specified
 * 'user'       => Optional username, if specified, for regular URLS, or
 *                 the local part of email addresses
 * 'pass'       => Optional password, if specified
 * 'path'       => An absolute or relative path depending on the URL format
 * 'query'      => GET parameters from the URL as a single string
 * 'fragment'   => Anything after a hashmark (#) character from the
 *                 local part of the URL
 * 'type'       => One of the following, depending on the URL category:
 *                 'REMOTE'     A traditional URL with sheme and hostname
 *                 'ABSOLUTE'   A local absolute path
 *                 'RELATIVE'   A local relative (or Drupal) path
 *                 'EMAIL'      A bare email address or "mailto:" URL
 *                 unset        Unknown or invalid URL
 * 'md5'        => The MD5 hash of the lowercased, normalized URL
 *
 * The parameters to the function are:
 *
 * $url             The URL to validate
 * $accept_bare     If TRUE, the function will also accept URLs with no
 *                  scheme (that is, without "http://" etc.) as long as
 *                  they begin with a valid hostname or match an email
 *                  address. In this situation, the returned normalized
 *                  URL will add the scheme as appropriate, including
 *                  "mailto:" preceding an email address. The default scheme
 *                  for anything not beginning with "ftp." is "http", except
 *                  that "mailto" will be the scheme for email addresses.
 * $accept_relative If TRUE, the function will accept relative paths (e.g.,
 *                  "node/1234"). In this situation, the returned normalized
 *                  URL will not add a scheme or server name. If this is TRUE,
 *                  then $accept_absolute is forced TRUE as well.
 * $accept_absolute If TRUE, the function will accept local absolute paths
 *                  (like a Drupal path, only beginning with a slash). in
 *                  this situation, the returned normalized URL will not
 *                  add a scheme or server name.
 * 
 * The PHP parse_url() function is deliberately not used here, because its
 * validation is not extensive since it is intended only as a parser and
 * not as a validator. The resemblence of this function's return values to
 * those of parse_url() is not, however, an accident.
 */
function links_parse_url($url, $accept_bare=TRUE, $accept_relative=TRUE, $accept_absolute=TRUE) {
  $result = array(
    'url' => $url,
    'valid' => FALSE,
    'debug' => array(),
  );
  $url_n = trim($url);

  // Trivial rejection
  if (empty($url_n)) {
    $result['errors'] = array('Empty URL');
    return $result;
  }

  // URI scheme (protocol spec)
  $p_scheme = "(?:([A-Za-z0-9]+)://|(mailto:))";
  // Fully qualified domain name (hostname or server name)
  $p_fqdn   = "([a-zA-Z0-9]+\.[a-zA-Z0-9-.]*[a-zA-Z0-9])";
  // Port specifier
  $p_port   = "(?::([0-9]+))";
  // Userid or local_part for email address
  $p_user   = "([a-zA-Z0-9_\-.%$]+)";
  // Password for URLs of the form "scheme://username:password@server/path"
  $p_passwd = "([a-zA-Z0-9%_~#?&=.,;-]+)";
  // Combined userid and password authentication spec (see above)
  $p_auth   = "(?:(".$p_user."(?::".$p_passwd.")?)@)";
  // Combined specification of authentication credentials, hostname, and port
  $p_server = "(".$p_auth."?".$p_fqdn.$p_port."?)";
  // Parameters for GET query string
//  $p_query  = "(?:\?([a-zA-Z0-9{}@:%_~&=.,/;-]+))";
  $p_query  = "(?:\?([^#]*))";
  // Fragment indicator (after "#")
  $p_frag   = "(?:#([a-zA-Z0-9@:%_?~&=.,/;-]*))";
  // The path catches the leading slash, if present, separately
  // so that we can later distinguish between relative and absolute
//  $p_path   = "(/?)([a-zA-Z0-9@:%_~&=.,/;-]*)";
  $p_path   = "(/?)([^?#]*)";
  $p_local  = $p_path . $p_query . "?" . $p_frag . "?";

  // Set some path components based on optional parameters
  if ($accept_bare) {
    $p_scheme .= "?";
  }
  $p_inet = "(".$p_scheme.$p_server.")";
  if ($accept_relative) {
    $accept_absolute = TRUE;
  }
  if ($accept_absolute) {
    // The entire remote host spec is optional
    $p_inet .= "?";
  }

  // Now try to find a match
  $pattern = "!".$p_inet.$p_local."$!i";
  $result['debug'][] = "Matching for regular URL using pattern: ".$pattern;
  $matches = array();
  preg_match($pattern, $url_n, $matches);
  if (count($matches) > 0) {
    if ($matches[3] == "mailto:"
       || (empty($matches[2]) && empty($matches[3])
          && !empty($matches[6]) && !empty($matches[8])
          && empty($matches[10]) && empty($matches[11]))) {
      $result['scheme'] = "mailto";
      $result['type'] = 'EMAIL';
    }
    if (! isset($result['type'])) {
      $result['scheme'] = $matches[2];
      if (! empty($result['scheme'])) {
        $result['type'] = 'REMOTE';
        $result['path'] = "/" . $matches[11];
        $result['port'] = $matches[9];
      } else if (! empty($matches[8])) {
        if (preg_match("!^ftp.!", $matches[8])) {
          $result['debug'][] = "Assuming remote FTP protocol for bare URL based on host name";
          $result['scheme'] = 'ftp';
        } else {
          $result['debug'][] = "Assuming remote HTTP protocol for bare URL";
          $result['scheme'] = 'http';
        }
        $result['type'] = 'REMOTE';
        $result['path'] = "/" . $matches[11];
        $result['port'] = $matches[9];
      } else {
        if (!empty($matches[11])) {
          if (empty($matches[10])) {
            // Local relative path
            $result['type'] = 'RELATIVE';
            $result['path'] = $matches[11];
          } else {
            // Local absolute path
            $result['type'] = 'ABSOLUTE';
            $result['path'] = '/' . $matches[11];
          }
        }
      }
      if (isset($result['type'])) {
        $result['pass'] = $matches[7];
        $result['query'] = $matches[12];
        $result['fragment'] = $matches[13];
      }
    }
    if (isset($result['type'])) {
      $result['host'] = $matches[8];
      $result['user'] = $matches[6];
    }
  }
  if (isset($result['type'])) {
    $result['valid'] = TRUE;
    // Build the normalized URL
    $norm = '';
    switch ($result['scheme']) {
      case 'mailto':
         $norm .= 'mailto:';
         break;
      case '':
         break;
      default:
         $norm .= $result['scheme'] . '://';
    }
    $type = $result['type'];
    if (! empty($result['user'])) {
      $norm .= $result['user'];
      if (! empty($result['pass'])) {
        $norm .= ':' . $result['pass'];
      }
      $norm .= '@';
    }
    $norm .= $result['host'];
    if (! empty($result['port'])) {
      $norm .= ':' . $result['port'];
    }
    $norm .= $result['path'];
    if (! empty($result['query'])) {
      $result['debug'][] = "Raw query as extracted: ".$result['query'];
      $result['query'] = links_remove_session_id($result['query']);
      $result['debug'][] = "Query after removing session IDs: ".$result['query'];
      $result['query'] = links_remove_empty_get_parms($result['query']);
      $result['debug'][] = "Query after removing empty parms: ".$result['query'];
      $result['query'] = links_sort_query_string($result['query']);
      $result['debug'][] = "Sorted query string: ".$result['query'];
    }
    if (! empty($result['query'])) {
      $norm .= '?' . $result['query'];
    }
    if (! empty($result['fragment'])) {
      $norm .= '#' . $result['fragment'];
    } 
    $result['normalized'] = $norm;
    $result['md5'] = md5(strtolower($norm));
  }
  return $result;
}

/*!
 * @}
 */

/*! @addtogroup links_text Text search and edit functions
 *  @{
 */

/**
 * Search a text string, potentially multiline, and return an array
 * containing all of the URLs found in that string. The array is
 * an integer-subscripted array with the subscript reflecting the
 * order in which the links were found.
 *
 * Each element of the main array is an inner array per link. These are
 * associative, with the keys being:
 *    "matched" => the actual text matched in the search, which is
 *                 later to be replaced with a "goto" link.
 *    "url"     => the normalized URL to which this link should go;
 *                 it may not be identical to the URL in the text.
 *                 "http://" is added to bare URLs that don't specify
 *                 a protocol.
 *    "title"   => a nominal title string for the link. For links that
 *                 had an HTML <A> tag around them, this is the text
 *                 inside the tag. For bare links, this is the same
 *                 as the url originally found in the text (the assumption
 *                 being that this is what the author wanted to present).
 *
 * Some of the pregexps are adapted from the weblinks module, and others
 * are adapted from examples on php.net. In all cases, they are complex
 * and should be changed with great care and tested thoroughly, as small
 * changes here could have large and unintended consequences!
 */
function links_find_links($text) {
  $links = array();
  $text = ' ' . $text . ' ';
  $patterns = array();
  // Finds links with a protocol spec (adapted from urlfilter.module)
  $patterns['bare_url'] = "!(?:<p>|<br>|<br />|[ \n\r\t\(])((?:(?:[A-Za-z0-9]+://)|mailto:)(?:[a-zA-Z0-9@:%_~#?&=.,/;-]*[a-zA-Z0-9@:%_~#&=/;-]))([.,?]?)(?=(</p>|[ \n\r\t\)]))!i";
  // Finds WWW or FTP links without a protocol spec (adapted from urlfilter.module)
  $patterns['bare_no_protocol'] = "!(?:<p>|<br>|<br />|[ \n\r\t\(])((?:www|ftp)\.[a-zA-Z0-9@:%_~#?&=.,/;-]*[a-zA-Z0-9@:%_~#\&=/;-])(?:[.,?]?)(?=(</p>|[ \n\r\t\)]))!i";
  // Finds pre-tagged links (pattern adapted from one posted to php.net by "martin at vertikal dot dk")
  $patterns['tag'] = "!<a href=(?:\"([^\">]+)\"[^>]*|([^\" >]+?)[^>]*)>(.*)</a>!Uis";
  reset($patterns);
  while (list($type, $pattern) = each($patterns)) {
    $found = array();
    $count = preg_match_all($pattern, $text, $found);
    $howmany = count($found[0]);
    switch ($type) {
      case 'bare_url':
        $found[0] = $found[1];
        $found[2] = $found[1];
        unset($found[3]);
        break;
      case 'bare_no_protocol':
        $found[0] = $found[1];
        $found[2] = $found[1];
        for ($i=0; $i<$howmany; $i++) {
          $found[1][$i] = "http://" . $found[1][$i];
        }
        break;
      case 'tag':
        $found[2] = $found[3];
        break;
    }
    // At this point, $found is a nested array where outer subscripts
    // are as follows:
    // 0 => array of all the text matched in the original item
    // 1 => array of all the physical URLs that should be linked
    // 2 => array of nominal display text for the links
    for ($i=0; $i<$howmany; $i++) {
      // Create an associative array for one link
      $matched = $found[0][$i];
      $url = links_normalize_url($found[1][$i]);
      $title = $found[2][$i];
      // We may have line breaks in the title -- remove them
      $title = preg_replace("![\n\r\t]!",' ',$title,-1);
      $link = array("matched"=>$matched, "url"=>$url, "title"=>$title);
      $links[] = $link;
    }
  }
  return $links;
}

/*!
 * @}
 */

// ********* FUNCTIONS FOR MANAGING THE URL HASH ***************

/*!
 * @addtogroup links_hash URL hash management
 * @{
 */

/**
 * Given a URL, this function returns an MD5 hash of a case-insensitive,
 * normalized version of that URL. The idea of this function is that two
 * URLs that differ only in ways that are not significant to a web
 * browser will have the same MD5 in the database. See the warning for
 * function links_normalize_url() for information concerning
 * changes made to this code in new versions.
 */
function links_hash_url($url) {
  $url = strtolower(links_normalize_url($url));
  return md5($url);
}

/*!
 * @}
 */

// ************** FUNCTIONS FOR DATABASE QUERIES AND UPDATES ****************

/*!
 * @addtogroup links_db Link storage and retrieval
 * @{
 */

/**
 * This internal function creates an SQL WHERE clause to select a
 * record from the {links} table based on an integer link ID
 * or a URL (which is normalized and hashed before the query). The
 * returned SQL will be a WHERE clause with leading and trailing blanks
 * for convenient concatenation. Note that no query is executed here.
 *
 * Accepts an MD5 string for the $link_spec, to match directly against
 * the url_md5 field in the database. The assumption is that a raw
 * hex number exactly 32 characters in length won't be a URL. The hex
 * characters A-F are matched case-insensitively.
 *
 * The $tablename optional parameter defaults to {links} but can
 * specify a table alias if this WHERE is to be used in a join query.
 * Remember to use the curly-brackets for actual table names, but not
 * for alias names, so that database prefixes will work right.
 */
function links_get_link_where_sql($link_spec, $tablename="{links}") {
  $where = " WHERE ".$tablename;
  // @TODO fix this - it just fails if the hash happens to start with a numeric (DOH)
  if (intval($link_spec)) {
    $where = $where . ".lid=" . $link_spec . " ";
  } else {
    if (preg_match('/^[0-9a-f]{32}$/i', $link_spec)) {
      $hash = $link_spec;
    } else {
      $hash = links_hash_url($link_spec);
    }
    $where = $where . ".url_md5='" . $hash . "' ";
  }
  return $where;
}

/**
 * Given a URL, URL hash, or link ID (lid), retrieves any existing link record
 * from the database as an array.
 * 
 * @param string $link_spec The URL or hash of the URL to be fetched
 */
function links_get_link($link_spec) {
  $where = links_get_link_where_sql($link_spec);
  $sql = "SELECT * FROM {links}" . $where;
  $result = db_query($sql);
  if (db_error()) {
    watchdog("links","links could not retrieve link matching '".$link_spec."'", array(), WATCHDOG_ERROR);
    return NULL;
  }
  if ($link = db_fetch_array($result)){
    $link['link_title'] = trim($link['link_title']);
    $link['url'] = trim($link['url']);
  }
  return $link;
}

/**
 * This function deletes a link record specified by its lid.
 * Before doing this, it invokes the hook 'links_delete_link_references'
 * with the specified link ID so that other modules can do what they
 * need to do in order to adjust. In some cases, this may actually
 * mean the other modules need to delete nodes.
 */
function links_delete_link($lid) {
  module_invoke_all('links_delete_link_reference', $lid);
  // This query exists in case a module implementor failed to
  // implement the hook. It makes sure the table is tidy,
  // but provides no opportunity for the module to add
  // customized behavior.
  $sql = "DELETE FROM {links_node} WHERE lid=%d";
  db_query($sql, $lid);
  if (db_error()) {
    watchdog("links","Links could not delete link !lid from {links_node} table", array('!lid'=>$lid), WATCHDOG_ERROR);
    // Treat this one as non-fatal, so no return here
  } else {
    watchdog("links","Executed backstop clearing of {links_node} for lid=!lid", array('!lid'=>$lid), WATCHDOG_INFO);
  }
  // Now delete the actual link.
  $sql = "DELETE FROM {links} WHERE lid=%d";
  db_query($sql, $lid);
  if (db_error()) {
    watchdog("links", "Failed to delete link !lid", array('!lid'=>$lid), WATCHDOG_ERROR);
    return FALSE;
  } else {
    watchdog("links", "Deleted link !lid", array('!lid'=>$lid), WATCHDOG_INFO);
    return TRUE;
  }
}

/**
 * This low-level function updates an existing link record in place. If the
 * URL has changed, it checks for another existing record that already matches
 * the *new* URL, and returns that link ID if found. Otherwise, updates the
 * URL and/or title of the existing record.
 *
 * If the existing record is not found, an insert will be attempted.
 *
 * In any case, if things are successful, the *new* $lid (which may or
 * may not be the same as the old one) is returned. The caller should
 * then call module_invoke_all('links_update_link_reference', $old_lid, $new_lid)
 * to let other modules update any referring tables if needed.
 *
 * @see links_update_link
 */
function links_update_link_basic($old_lid, $url, $title='') {
  if ($old_lid) {
    $old = links_get_link($old_lid);
  } else {
    $old = links_get_link($url);
  }
  if (! is_array($old)) {
    // No extant record. Attempt insert.
    return links_put_link($url, $title);
  } else {
    $title = check_plain(trim($title));
    $old_lid = $old['lid'];
    $new_hash = links_hash_url($url);
    if ($new_hash == $old['url_md5']) {
      if ($title == $old['link_title']) {
        // Nothing to do!
        return $old_lid;
      }
      // Safe to update the title in place
      $sql = "UPDATE {links} SET link_title='%s' WHERE lid=%d";
      $result = db_query($sql, $title, $old_lid);
      if (db_error()) {
        drupal_set_message(t('Update of title in existing link record failed.'), 'error');
        watchdog("links","links update for existing link !lid failed", array('!lid'=>$old_lid), WATCHDOG_ERROR);
        return 0;
      }
      return $old_lid;
    } else {
      // The URL has changed
      $extant = links_get_link($url);
      if (is_array($extant) && $extant['lid'] != $old_lid) {
        // An attempt was made to change this link's URL to match one already in the
        // database. What we need to do, therefore, is to return the existing record's
        // key. Higher-level code should update references and then delete the old
        // record ($old_lid) and keep the new one ($extant['lid']) instead.
        return $extant['lid'];
      } else {
        // It's actually safe to update the URL and the title in place
        $sql = "UPDATE {links} SET link_title='%s', url='%s', url_md5='%s' WHERE lid=%d";
        $title = trim($title);
        $result = db_query($sql, $title, $url, $new_hash, $old_lid);
        if (db_error()) {
          drupal_set_message(t('Update of URL and title in existing link record failed.'), 'error');
          watchdog("links","Links update for existing link !lid failed", array('!lid'=>$old_lid), WATCHDOG_ERROR);
          return 0;
        } else {
          watchdog("links","Changed URL for existing link !lid to !url", array('!lid'=>$old_lid, '!url'=>$url), WATCHDOG_INFO);
        }
        return $old_lid;
      }
    }
  }
}

/**
 * For a specified link ID, checks all existing node references to be sure
 * the title is really needed in the local record. If appropriate, the
 * {links_node} record is set to use the master record's title. This is
 * typically invoked after changing the title in the master record, because
 * one or more of the node records may no longer need its title.
 */
function links_check_node_titles($lid) {
  $master = links_get_link($lid);
  if (! $master['lid']) {
    // Don't do anything if no master record.
    return;
  }
  $result = db_query("SELECT * FROM {links_node} WHERE lid=%d", $lid);
  while ($row = db_fetch_array($result)) {
    // In most cases this become a no-op
    links_set_local_title($lid, $row['nid'], $row['module'], $row['link_title']);   
  }
}

/**
 * This is a higher-level wrapper around links_update_link_basic() that is
 * well-behaved with respect to updating references to tables. Anything in
 * {links_node} will be taken care of by our own code. Other modules only
 * need to implement the hook if they use custom tables.
 */
function links_update_link($old_lid, $url, $title='') {
  $new_lid = links_update_link_basic($old_lid, $url, $title);
  links_check_node_titles($new_lid);
  if ($new_lid && $old_lid && ($new_lid != $old_lid)) {
    watchdog("links", "Updated link ID from !old to !new. Calling hook to update references, then old link will be deleted.", array('!old'=>$old_lid, '!new'=>$new_lid), WATCHDOG_INFO);
    module_invoke_all('links_update_link_reference', $old_lid, $new_lid);
    // Now delete the old link that is no longer used anywhere
    links_delete_link($old_lid);
  }
}

/**
 * Forces all of the references in {links_node} for the
 * specified $lid to use the master title rather than their
 * node-local title, if any.
 *
 * $lid is required; the other two parameters are optional.
 */
function links_force_master_title($lid, $nid=0, $module='') {
  if ($nid==0) {
    if (empty($module)) {
      db_query("UPDATE {links_node} SET link_title='' WHERE lid=%d", $lid);
    } else {
      db_query("UPDATE {links_node} SET link_title='' WHERE lid=%d AND module='%s'", $lid, $module);
    }
  } else {
    if (empty($module)) {
      db_query("UPDATE {links_node} SET link_title='' WHERE lid=%d AND nid=%d", $lid, $nid);
    } else {
      db_query("UPDATE {links_node} SET link_title='' WHERE lid=%d AND nid=%d AND module='%s'", $lid, $nid, $module);
    }
  }
  if (db_error()) {
    watchdog("links", "Failed to force master title for link !lid", array('!lid'=>$lid), WATCHDOG_ERROR);
  } else {
    watchdog("links", "Forcing master title for link !lid", array('!lid'=>$lid), WATCHDOG_INFO);
  }
}

/**
 * Returns the count of how many references exist for a given link in
 * {links_node}. If $module is specified, the query is limited to that
 * module only.
 *
 * TODO: Add more robust error handling.
 */
function links_count_nodes($link_spec, $module=NULL) {
  $link = links_get_link($link_spec);
  if (! is_array($link)) {
    return 0;
  }
  $lid = intval($link['lid']);
  if (empty($module)) {
    $result = db_query("SELECT COUNT(*) AS n FROM {links_node} WHERE lid=%d", $lid);
  } else {
    $result = db_query("SELECT COUNT(*) AS n FROM {links_node} WHERE lid=%d AND module='%s'", $lid, $module);
  }
  $row = db_fetch_array($result);
  if (is_array($row)) {
    return intval($row['n']);
  } else {
    return 0;
  }
}

/**
 * Implements hook_links_update_link_reference to change $old_lid to
 * $new_lid in our own tables. Here we update only {links_node}.
 */
function links_links_update_link_reference($old_lid, $new_lid) {
  db_query("UPDATE {links_node} SET lid=%d WHERE lid=%d", $new_lid, $old_lid);
  if (db_error()) {
    watchdog("links","links update for existing link record failed", array(), WATCHDOG_ERROR);
  }
}

/*
 * Gets a specific record from {links_node}
 */
function links_get_node_reference($lid, $nid, $module) {
  $result = db_query("SELECT * FROM {links_node} WHERE lid=%d AND nid=%d AND module='%s'", $lid, $nid, $module);
  if (db_error()) {
    watchdog("links", "database error while retrieving link reference lid=!lid, nid=!nid, module=!module", array('!lid'=>$lid, '!nid'=>$nid, '!module'=>$module), WATCHDOG_ERROR);
    return NULL;
  }
  if ($row = db_fetch_array($result)) {
    return $row;
  } else {
    return array();
  }
}

/*
 * Sets the local title for a specified link reference. If the affected
 * node is the ONLY node using the link, this will also update the master
 * link if the user has access to do so. Note that this function presumes
 * a pre-existing {links_node} record.
 */
function links_set_local_title($lid, $nid, $module, $new_title) {
  $link = links_get_link($lid);
  if (! is_array($link)) {
    watchdog("links", "cannot find master link for !lid while updating title in node reference", array('!lid'=>$lid), WATCHDOG_ERROR);
    return;
  }
  $master_title = $link['link_title'];
  $new_title = check_plain(trim($new_title));
  if (empty($new_title) || $new_title == $master_title) {
    // An empty local title, or one matching the master title, says to use the master title
    links_force_master_title($lid, $nid, $module);
    return;
  }
  // Get the existing local title
  $old = links_get_node_reference($lid, $nid, $module);
  $old_title = $old['link_title'];
   
  // If the new and old titles are equal, nothing to do. Otherwise, update
  // and set the master title if this is the only referring node.
  if ($new_title != $old_title) {
    $refcount = links_count_nodes($lid);
    if ($refcount > 1 || ! user_access('administer links')) {
      // Conventional local update
      db_query("UPDATE {links_node} SET link_title='%s' WHERE lid=%d AND nid=%d AND module='%s'", $new_title, $lid, $nid, $module);
    } else {
      // Update the master record
      db_query("UPDATE {links} SET link_title='%s' WHERE lid=%d", $new_title, $lid);
      if (! db_error()) {
        db_query("UPDATE {links_node} SET link_title='' WHERE lid=%d AND nid=%d AND module='%s'", $lid, $nid, $module);
      }
    }
    if (db_error()) {
      watchdog("links", "links update for link !lid, node !nid failed", array('!lid'=>$lid, '!nid'=>$nid), WATCHDOG_ERROR);
    }
  }
}

/**
 * Implements hook_links_delete_link_reference to remove $lid from
 * the {links_node} table for cases where the 'module' colum is
 * empty or null (which it shouldn't be, but...). Any module that
 * uses {links_node} should implement this hook as well.
 */
function links_links_delete_link_reference($lid) {
  // Here we just delete anything for null or empty module,
  // which at least in theory shouldn't happen. If you implement
  // this hook in your module, put your module's name in the
  // appropriate field as a constant.
  db_query("DELETE FROM {links_node} WHERE lid=%d AND module=''", $lid);
  if (db_error()) {
    watchdog("links","links delete references for link !lid failed for null module", array('!lid'=>$lid), WATCHDOG_ERROR);
  } else {
    watchdog("links","Deleted references for link !lid for null module", array('!lid'=>$lid), WATCHDOG_INFO);
  }
}

/**
 * This low-level function simply adds a bare URL record, with no associated
 * node, to the database. It returns the new link ID number
 * if successful, or 0 if it fails.
 *
 * The function checks for an existing matching link before adding a new
 * record, and if one is found, that links ID is returned instead of a new
 * one being assigned.
 *
 * The optional title field is only used if a new link is being inserted.
 */
function links_put_link($url, $title='') {
  $hash = links_hash_url($url);
  $sql = "SELECT * FROM {links} WHERE url_md5='%s'";
  $result = db_query($sql,$hash);
  if (db_error()) {
    // Log an error message and return failure
                drupal_set_message(t('Query for existing link record failed.'), 'error');
    watchdog("links","links query for existing link record failed", array(), WATCHDOG_ERROR);
    return 0;
  }
  if ($result) {
    $row = db_fetch_array($result);
    if ($row["lid"]) {
      // Existing record found
      return $row["lid"];
    }
  }
  // Need to create a new record
  $url = links_normalize_url($url);
  if (empty($title)) {
    $title = links_suggest_link_title($url);
  }
  $link_obj = new StdClass();
  $link_obj->url = $url;
  $link_obj->url_md5 = $hash;
  $link_obj->link_title = $title;
  $result = drupal_write_record('links', $link_obj);
  if ($result != SAVED_NEW) {
    // Failed to add record -- log and fail
    watchdog("links","Unable to insert URL @url into {links} table", array('@url'=>$link_obj->url), WATCHDOG_ERROR);
    return 0;
  }
  // Success!
  return $link_obj->lid;
}

/*!
 * @}
 *
 * @addtogroup links_node Link-Node associations
 * @{
 */

/**
 * Given a link ID, URL, or URL hash, creates an SQL query to obtain brief node data for
 * nodes that share that link's record. Columns returned from this query include:
 *    lid         The link ID number (integer)
 *    url         The actual (full) URL from the link record
 *    nid         The node ID number (integer)
 *    node_title  The node title (string)
 *    weight      The weight of this link-node pairing, which determines the
 *                order when links are listed together.
 *    clicks      The number of times a browser has followed this particular
 *                instance of this particular link-node association.
 *    link_title  The title given to the link (string); this is the text that
 *                is displayed for the user as the title of the link. How and
 *                where it appears (on the page, mouse over, not at all...)
 *                depends on the settings of user and admin preferences.
 *    module      If a specific module wants to own the link-node association,
 *                insert that module's name here. The default is 'links', which
 *                represents the entire links package collectively.
 *
 * Note that lid and url fields are the same on all returned rows; they are
 * here for convenience.
 *
 * The optional parameter $order determines whether the outer array is
 * ordered by the link's weight, by link_title, or by node_title. If
 * $order is an empty string (not the default!), no ORDER BY will be added; this
 * allows the caller more control, such as appending a WHERE clause or doing
 * a compound ordering on multiple columns. The default orders by weight and
 * then by link_title.
 *
 * Left joins are intentionally used in this query, with the deliberate possibility
 * to return rows with NULL values in cases of (1) a link with no associated nodes,
 * or (2) a link associated with node IDs that no longer exist. This is useful
 * to find orphan records. To suppress this feature, set the optional parameter
 * $orphans to FALSE (it defaults to TRUE).
 *
 * Note that this function returns an SQL statement but does not actually
 * execute the query.
 */
function links_get_link_node_query_sql($link_spec, $order="weight,link_title", $orphans=TRUE, $module='links') {
  $sql = "SELECT l.lid, l.url, ln.nid, n.title AS node_title, ln.weight, ln.clicks, ln.link_title AS link_title ";
  $sql .= "FROM {links} AS l LEFT JOIN {links_node} AS ln ON l.lid=ln.nid ";
  $sql .= "LEFT JOIN {node} AS n ON ln.nid=n.nid ";
  $sql .= links_get_link_where_sql($link_spec, "l");
  if (! $orphans) {
    $sql .= " AND ln.nid IS NOT NULL AND n.nid IS NOT NULL ";
  }
        if (! empty($module)) {
    $sql .= " AND ln.module='" . $module . "' ";
  }
  switch ($order) {
    case "":
      break;
    case "node_title":
    case "link_title":
      $sql .= " ORDER BY " . $order;
      break;
    default:
      $sql .= " ORDER BY weight, link_title";
  }
  $sql .= " ";
  return $sql;
}

/**
 * Delete all the links for the specified node (object)
 * or nid (integer)
 */
function links_delete_links_for_node($node, $module='links_links') {
  $sql = "DELETE FROM {links_node} WHERE nid=%d";
  if (! empty($module)) {
    $sql .= " AND module='" . $module . "' ";
  }
  if (is_object($node)) {
    return db_query($sql, $node->nid);
  } else {
    return db_query($sql, $node);
  }
}

/**
 * Save all the links for the specified node (object). The links to be
 * saved are assumed to exist as sub-arrays inside the main integer-
 * subscripted array named $node->$module, where $module is the
 * parameter that specifies the context for these links.
 *
 * Each link is an associative sub-array containing the fields 'url',
 * 'weight', and 'link_title'. The 'url' field is self-explanatory.
 * 'weight' will determine, when multiple links exist, their display
 * order when the node is rendered. 'link_title' is stored as either
 * the node-local title for the link, or as the global link title if
 * the link is new to the catalog. 'url' is the only required field;
 * the others have defaults.
 *
 * There is no equivalent function to save a single link for a node. If
 * modules require that functionality, they should simply pass an array
 * with only one link sub-array.
 *
 * @param &$node   The node for which links are being saved
 * @param $module  The module controlling the link(s)
 * @param $global  If TRUE, modification of a URL for any of these
 *                 links will change that link's URL globally. If
 *                 FALSE (the default), a changed URL results in a
 *                 separate catalog entry, leaving other nodes' URLs
 *                 untouched. $global is ignored if the current user
 *                 lacks the 'change url globally' permission (set in
 *                 links.module).
 */
function links_save_links_for_node(&$node, $module='links_links', $global=FALSE) {
  $oldlinks =& links_load_links_for_node($node->nid, $module, NULL, NULL, 'lid');
  $links =& $node->$module;
  $all_ok = TRUE;
  // This will track the link IDs that should be kept
  $lids = array();
  foreach ($links as $i => $link) {
    $url = links_normalize_url($link['url']);
    $link['url'] = $url;
    $title = trim($link['link_title']);
    $link['link_title'] = $title;
    if (empty($url) || $link['delete']) {
      // Ignore empty URL, either from unused form fields or from
      // blanked-out existing URL field. In the latter case, this
      // link will be removed from the node later in this function.
      // Same action if the "delete" flag (boolean) is set in the
      // array (this normally would happen from a UI form).
      continue;
    }
    // Make sure the base link record exists, one way
    // or another.
    $lid = links_put_link($url, $link['link_title']);
    // $base_rec gets the full record from {links} whether
    // it was inserted or just retrieved
    $base_rec = links_get_link($lid);
    if (isset($oldlinks[$lid])) {
      // Old record from this node
      $old_rec = $oldlinks[$lid];
    } else {
      // Defaults for some fields
      $old_rec = $base_rec;
    }
    if (is_array($base_rec) && $base_rec['lid']) {
      $link['lid'] = $old_rec['lid'];
      $lids[] = $old_rec['lid'];
      $new_title = trim($link['link_title']);
      $master_title = trim($base_rec['link_title']);
      if (empty($new_title)) {
        $suggested = TRUE;
        $new_title = links_suggest_link_title($base_rec['lid'], '');
      } else {
        $suggested = FALSE;
      }
      // If we are first to title an existing link, make that
      // update directly before anything else
      if (empty($master_title)) {
        $sql = "UPDATE {links} SET link_title='%s' WHERE lid=%d";
        $result = db_query($sql, $new_title, $lid);
        $master_title = $new_title;
      }
      // Now do the rest
      if ($new_title == $master_title || $suggested) {
        // Leave the node-specific title empty if we
        // already have it in the master record. Also,
        // an API-suggested title goes *only* in the master
        // record, and if applicable, we did that above.
        $new_title = '';
      } else {
        if (empty($new_title)) {
          $new_title = $master_title;
        }
      }
    } else {
      drupal_set_message(t('Cannot find or create link record for %url',array('%url'=>$url)),'error');
      $all_ok = FALSE;
    }
    $weight = intval($link['weight']);
    if (isset($oldlinks[$lid])) {
      $sql = "UPDATE {links_node} SET link_title='%s', weight=%d WHERE lid=%d AND nid=%d AND module='%s'";
      $op  = t('update');
    } else {
      $sql = "INSERT INTO {links_node} (link_title, weight, lid, nid, clicks, module) VALUES ('%s',%d,%d,%d,0,'%s')";
      $op  = t('insert');
    }
    $result = db_query($sql, $new_title, $weight, $lid, $node->nid, $module);
    if (! $result) {
      drupal_set_message(t('Unable to %op link &quot;%t&quot; in database.', array('%op'=>$op, '%t'=>$link['link_title'])),'error');
      $all_ok = FALSE;
    }
  }
  // Now delete any leftover links from before, that weren't in the group we
  // just confirmed should stay
  if (count($lids)) {
    $inlist = implode(',',$lids);
    $sql = "DELETE FROM {links_node} WHERE nid=%d AND lid NOT IN (%s) AND module='%s'";
    db_query($sql,$node->nid,$inlist, $module);
  } else {
    // There now are NO links for this node
    links_delete_links_for_node($node, $module);
  }
  return $all_ok;
}

/**
 * Given a node ID, returns all of the links currently defined for that node in
 * the database. The return value is a compound array, with the outer subscript
 * being an integer index and the elements associative arrays with the fields
 * lid, url, url_md5, title, clicks, and weight.
 *
 * For each record returned, there are actually up to *three* title fields:
 *   link_link_title         the title from the main {links} record
 *   node_link_title         the title, if any, from the {links_node} record
 *   link_title              the node_link_title if present, or the link_link_title
 *                           otherwise. This is normally the field that applications
 *                           will want to use for display.
 *
 * If a specific link ID is provided, returns ONLY that record. If $firstonly
 * is TRUE, returns only one record. $firstonly is provided as a convenience for
 * handling a "goto" URL pattern from the browser that specifies nid but not lid.
 * This is used, among other things, for legacy compatibility with weblink.module.
 */
function links_load_links_for_node($nid, $module='links', $lid=0, $firstonly=FALSE, $indexedby = NULL) {
  $links = array();
  if ($lid) {
    $sql = "SELECT l.lid, ln.nid, url, url_md5, weight, clicks, module, l.link_title AS link_link_title, ln.link_title AS node_link_title FROM {links_node} ln LEFT JOIN {links} l ON l.lid=ln.lid WHERE ln.nid=%d AND l.lid=%d";
    if (! empty($module)) {
      $sql .= " AND module='" . $module . "' ";
    }
    $sql .= " ORDER BY weight, l.link_title";
    $result = db_query($sql, array($nid, $lid));
  } else {
    $sql = "SELECT l.lid, ln.nid, url, url_md5, weight, clicks, module, l.link_title AS link_link_title, ln.link_title AS node_link_title FROM {links_node} ln LEFT JOIN {links} l ON l.lid=ln.lid WHERE ln.nid=%d";
    if (! empty($module)) {
      $sql .= " AND module='" . $module . "' ";
    }
    $sql .= " ORDER BY weight, l.link_title";
    $result = db_query($sql, array($nid));
  }
  if (db_error($result)) {
    watchdog("links", "links database error on query: @sql", array('@sql'=>$sql), WATCHDOG_ERROR);
  } else {
    while ($row = db_fetch_array($result)) {
      $row['link_title'] = trim($row['link_title']);
      $row['link_title'] = trim(empty($row['node_link_title']) ? $row['link_link_title'] : $row['node_link_title']);
      if($indexedby){
        $links[$row[$indexedby]] = $row;
      } 
      else {
        $links[] = $row;
      }        
      if ($firstonly) {
        break;
      }
    }
  }
  return $links;
}

/**
 * Given a node ID and an optional link ID, goes to either the specified
 * link ID (with click credit to the appropriate {links_node} record) or
 * to the first [or only] link for the specified node, again with appropriate
 * click credit.
 *
 * It is valid for $nid to be zero, if $lid is nonzero. In that case, the
 * function will redirect to the URL specified by the link, but will not
 * tally the link for a particular node.
 *
 * Unlike other instances of $module, here we default to an empty string to
 * fetch any link regardless of context. Callers can override that behavior,
 * and indeed almost always will want to do so.
 *
 * Returns TRUE if a redirect was actually sent to the browser, FALSE if not.
 */
function links_goto_bynode($nid, $lid=0, $module='') {
  if ($nid == 0 && $lid > 0) {
    // Goto a link without crediting any specific node
    $link = array(links_get_link($lid));
  } else {
    // Look it up by node ID
    $link =& links_load_links_for_node($nid, $module, $lid, TRUE);
  }
  if (! is_array($link)) {
    return FALSE;
  }
  return links_goto_link($link[0], $module);
} 

/**
 * Given a link ID or the URL or its MD5 hash, redirects to that location and tallies one
 * click in the links_node table.
 *
 * If $module is not specified, the redirection will still work as long as
 * the link spec is valid, but the outbound click won't be tallied in the
 * database. $module is needed for tallying because it is legal for the
 * same node to refer to the same link from multiple module contexts.
 *
 * Returns TRUE if a redirect was actually sent to the browser, FALSE if not.
 */
function links_goto($link_spec, $module='') {
  $link = links_get_link($link_spec);
  if (! is_array($link)) {
    return FALSE;
  }
  return links_goto_link($link, $module);
}

/**
 * Given a link record array, redirects to the URL contained in the array.
 *
 * Returns TRUE if a redirect was actually sent to the browser, FALSE if not.
 */
function links_goto_link($link, $module='') {
  if ($link['lid']) {
    # We can only tally the click if we know the associated module, because the
    # same link could be referenced in different ways from the same node.
    if ($link['nid'] && !empty($module)) {
      db_query("UPDATE {links_node} SET clicks=clicks+1 where lid='%d' and nid='%d' AND module='%s'", $link['lid'], $link['nid'], $module);
    }
    # The last click time goes with the link itself, so we don't need the module
    # name to update that.
    db_query("UPDATE {links} SET last_click_time='%d' WHERE lid='%d'", time(), $link['lid'], $module);
    $url = links_is_external($link["url"]) ? $link["url"] : url($link["url"]);
    if (variable_get("links_log_redirect", 0)) {
      watchdog("links", "links_goto: lid=@lid, nid=@nid, url=@url", array('@lid'=>$link['lid'], '@nid'=>$link['nid'], '@url'=>$url), WATCHDOG_INFO);
    }
    header("Location: " . $url);
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
 * Make a Drupal-standard URL that points to the proper
 * "goto" page in this module. The returned URL is a Drupal-style
 * URL, so it needs to be processed through url() or l() later
 * to account for sites not using clean URLs.
 *
 * This will return the DIRECT link if certain settings are
 * enabled in the admin page for links.module.
 */
function links_format_goto_url($nid=0, $lid=0, $module='') {
  // See if we need to check the actual URL in case we
  // might need to skip the redirect.
  $skip_on_internal = variable_get("links_direct_internal", 0);
  $skip_on_external = variable_get("links_direct_external", 0);
  $link = NULL;
  if ($skip_on_internal && $skip_on_external) {
    // We will always skip the redirect in this case.
    $link = links_get_link($lid);
  } else if ($skip_on_internal || $skip_on_external) {
    // Maybe we skip the redirect
    $link = links_get_link($lid);
    $url = $link['url'];
    if (links_is_external($url)) {
      if (! $skip_on_external) {
        // No redirect; reset
        $link = NULL;
      }
    } else {
      if (! $skip_on_internal) {
        // No redirect; reset
        $link = NULL;
      }
    }
  }
  if (is_array($link)) {
    // If the link array is still set here, it means we
    // should skip the redirection page.
    return $link['url'];
  }
  if ($lid) {
    if (empty($module)) {
      return 'links/goto/'.$nid.'/'.$lid;
    } else {
      return 'links/goto/'.$nid.'/'.$lid.'/'.$module;
    }
  } else {
    return 'links/goto/'.$nid;
  }
}

/**
 * Returns a boolean to indicate whether a link is external or not.
 * The $link_spec can be a link ID or a URL. If it is a URL that is
 * also a "links/goto" URL, then the appropriate node and link IDs
 * are obtained from the URL, and then the actual target is looked
 * up for evaluation, rather than the "links/goto" itself. An empty
 * link spec assumes an external link.
 */
function links_is_external($link_spec=0) {
  // The cache works only on repeated calls with the same link_spec
  // in the same page fetch operation.
  static $cache;
  if (! is_array($cache)) {
    $cache = array();
  }
  if (isset($cache[$link_spec])) {
    return $cache[$link_spec];
  }
  $preg1 = '!^[a-zA-Z0-9]+:(?:/)*!';
  $preg2 = '!^(?:/)*links/goto!';
  // This is a NEGATIVE match string on the local base URL.
  $preg3 = '!^' . $GLOBALS['base_url'] . '!';
  if (preg_match($preg2, $link_spec)) {
    preg_match('!' . '(?:links/goto/([0-9]+)(?:/([0-9]+)(?:/(.*$)))*)!', $link_spec, $matches);
    if (intval($matches[1])) {
      // The nid is specified
      if (intval($matches[2])) {
        // The link ID is also specified
        $link = links_get_link($matches[2]);
        $url = is_array($link) ? $link['url'] : '';
      } else {
        // No link ID specified. Assume the first one
        $links =& links_load_links_for_node(intval($matches[1]), intval($matches[3]), TRUE);
        $url = is_array($link) ? $links[0]['url'] : '';
      }
    } else {
      // If it couldn't be looked up, leave it alone as an internal so the
      // error is handled more sensibly later.
      $cache[$link_spec] = FALSE;
      return FALSE;
    }
  } else if (intval($link_spec)) {
    $link = links_get_link($link_spec);
    $url = is_array($link) ? $link['url'] : '';
  } else {
    $url = $link_spec;
  }
  if (empty($url) || preg_match($preg2, $url) || (preg_match($preg1, $url) && ! preg_match($preg3, $url))) {
    $cache[$link_spec] = TRUE;
    $external = TRUE;
  } else {
    $cache[$link_spec] = FALSE;
    $external = FALSE;
  }
  return $external;
}

/**
 * Returns the correct link target ("_blank" or "") depending on
 * global settings and, if enabled, user preference settings. The
 * target may depend on whether the link is internal or external.
 * If that is the case, and the optional link_spec parameter is not
 * supplied, an external link will be assumed.
 */
function links_get_goto_target($link_spec=0) {
  global $user;
  $t_value = intval(variable_get("links_target", 0)); 
  if (intval(variable_get("links_target_user", 0))) {
    if (isset($user->links_target_new)) {
      $t_value = intval($user->links_target_new);
    }
  }
  switch ($t_value) {
    case 1:
      $target = '_blank';
      break;
    case 2:
      $external = links_is_external($link_spec);
      $target = $external ? '_blank' : '';
      break;
    default:
      $target = '';
  }
  return $target;
}

/*!
 * @}
 *
 * @addtogroup links_prefs Preference settings management
 * @{
 */

/**
 * Returns a simple integer-subscripted array with a list of links that
 * should be displayed with a node (this is for use in hook_link()
 * implementations). The module context is used for looking up site
 * settings for how the links are formatted (e.g., title vs. URL,
 * truncation, how many allowed, etc.).
 *
 * @param module establishes a context for looking up site settings
 * @param node the node for which links are to be obtained and displayed
 * @param page indicates the list is in full-page mode (default FALSE)
 * @param teaser indicates the list is in teaser mode (default FALSE)
 * @param statistics (default FALSE) indicates that the number of clicks should be
 * shown if the current user has authority to view link statistics
 */
function links_get_list($module, &$node, $page=FALSE, $teaser=FALSE, $statistics=FALSE) {
  if ($page) {
    // In endnotes mode, always display the whole list
    $count = -1;
  } else if ($teaser) {
    $count = variable_get($module.'_display_teaser','1');
  } else {
    $count = variable_get($module.'_display_full','-1');
  }
  $count = intval($count);
  $visit = $page ? '' : t('Visit');
  $list = array();
  if (is_object($node)) {
    $links =& links_load_links_for_node($node->nid, $module);
    reset($links);
    $n = 0;
    // If $count == 0, do nothing. If $count == -1, do infinity. Otherwise, do $count.
    while (($count == -1 || $n++ < $count) && list($lid, $link) = each($links)) {
      $title = $link['link_title'];
      // Note that the $url is *not* the actual URL from the database, but rather
      // uses the links_goto() function so we can tally the click count.
      $url = links_format_goto_url($node->nid, $link['lid'], $module);
      switch(variable_get("links_link_display", 0)) {
        case 0:
          $linktext = t('!v !link', array("!v"=>$visit, "!link" => $title));
          $hover = $link['url'];
          break;
        case 1:
          $linktext = t('!v !link', array("!v"=>$visit, "!link" => $link['url']));
          $hover = $title;
          break;
        default:
          $linktext = t('visit linked page');
          $hover = $title;
      }
      $trim = intval(variable_get($module.'_display_length', 0));
      if ($trim && !$page) {
        // Add 3 to the length when seeing if we should trim, because there
        // is no purpose in replacing characters with an ellipsis if the
        // result isn't any shorter than the original!
        if (strlen($linktext) > ($trim+3)) {
          $linktext = substr($linktext, 0, $trim) . '...';
        }
      }
      if ($statistics && ($page || $teaser) && user_access("access links statistics")) {
        // The current link's click count may be not-applicable depending
        // on admin settings. If so, clear $statistics to FALSE.
        if (links_is_external($url)) {
          if (! variable_get("links_direct_external", 0)) {
            $linktext .= " ({$link['clicks']})";
          }
        } else {
          if (! variable_get("links_direct_internal", 0)) {
            $linktext .= " ({$link['clicks']})";
          }
        }
      }
      $attr = array('title'=>$hover);
      $attr['class'] = 'outgoing';
      $target = links_get_goto_target($url);
      if (! empty($target)) {
        $attr['target'] = $target;
      }
      if (empty($linktext)) {
        $linktext = links_suggest_link_title($url);
      }
      if (variable_get('links_rel_nofollow',0) && links_is_external($url)) {
        $attr['rel'] = 'nofollow';
      }
      $list[] = array(
        'title' => $linktext,
        'href' => $url,
        'attributes' => $attr,
      );
    }
  }
  return $list;
}

/*!
 * @}
 *
 * @addtogroup links_prefs Preference settings management
 * @{
 */

/**
 * Returns a boolean to indicate whether or not the specified function
 * may be overridden by a user preference, by the current user. If no
 * user preference is allowed for this particular variable (based on
 * the system-wide setting), then always returns false. The application
 * doesn't have to care *why* the setting is disallowed.
 *
 * To obtain the system-wide setting, without respect for user-specific
 * overrides (if applicable), simply use variable_get() from Drupal core.
 *
 * Parameters:
 *     $name         name of the variable to test
 *     $permission   name of the permission to test, if this variable
 *                   has different permissions for some roles than for
 *                   others
 */
function links_prefs_is_user_specific($name, $permission = '') {
  global $user;
  // Trivial rejection
  if (empty($name)) return false;
  $key = $name . '_is_user_specific';
  $sys_allowed = variable_get($key, 0) ? true : false;
  if ($sys_allowed) {
    // At least *some* users are allowed to override setting
    if (empty($permission)) {
       // Same for all users
       $allowed = true;
    } else {
       // Check based on THIS user's specific roles
       $allowed = user_access($permission);
    }
  } else {
    // No users allowed to override
    $allowed = false;
  }
  return $allowed;
}

/**
 * Returns the system-wide or user-specific setting for a variable,
 * as appropriate. $name and $permission are defined the same as
 * for links_prefs_user_allowed(). $default is simply the default
 * value to return if this variable is unset.
 *
 * To obtain the system-wide setting, without respect for user-specific
 * overrides (if applicable), simply use variable_get() from Drupal core.
 */
function links_prefs_get($name, $default='', $permission='') {
  global $user;
  $u_allow = links_prefs_is_user_specific($name, $permission);
  if ($u_allow && isset($user->$name) && !empty($user->$name)) {
    $value = $user->$name;
  } else {
    $value = variable_get($name, $default);
  }
  return $value;
}

/*!
 * @}
 *
 * @addtogroup links_stats Links statistics
 * @{
 */

/**
 * THIS FUNCTION IS EXPERIMENTAL -- USE AT OWN RISK
 * Please send test reports to the maintainer.
 *
 * Returns an associative array containing the following statistics for the
 * specified link_spec:
 *
 *     lid, url, url_md5, link_title, last_click_time -- same as {links} table
 *     node_count   -- number of nodes that refer to the specified link
 *     total_clicks -- number of visitors sent to this link, via any node
 *
 * As with other query functions, $link_spec can be a URL, a URL hash, or an integer link ID.
 */
function links_stats_for_link($link_spec) {
  $where = links_get_link_where_sql($link_spec, "l");
  $sql = "select l.*, count(ln.nid) as node_count, sum(ln.clicks) as total_clicks from {links} l left join {links_node} ln on l.lid=ln.lid ";
  $sql .= $where;
  $sql .= " group by l.lid";
  $result = db_query($sql, array($nid));
  if (db_error($result)) {
    watchdog("links", "links database error on query: @sql", array('@sql'=>$sql), WATCHDOG_ERROR);
    return array();
  } else {
    $row = db_fetch_array($result);
    return $row;
  }
}

/**
 * THIS FUNCTION IS EXPERIMENTAL -- USE AT OWN RISK
 * Please send test reports to the maintainer.
 *
 * Returns the statistics (as links_stats_for_link(), but inside an outer array)
 * for the most-clicked or most-referenced URLs on the site. Records are retrieved
 * in descending order of the specified $order field; if ascending order is desired,
 * the calling application can either use PHP's array reversal function or simply
 * process the elements in reverse order. Links that are not referred to by any node
 * are automatically excluded.
 *
 * @param $limit Maximum number of records to return (default 10). Function may
 * return less if there are fewer matches to the other criteria.
 * @param $order The field in the output records by which to order (default "total_clicks",
 * recommended alternative "node_count")
 * @param $min_nodes If nonzero, excludes links having fewer than this many referring nodes
 * @param $min_clicks If nonzero, excludes links having fewer than this many total clicks
 */
function links_stats_popular($limit=10, $order="total_clicks", $min_nodes=1, $min_clicks=0) {
  $sql = "select l.*, count(ln.nid) as node_count, sum(ln.clicks) as total_clicks from {links} l left join {links_node} ln on l.lid=ln.lid group by l.lid";
  $cond = array();
  if ($min_nodes > 0) {
    $cond[] = "node_count >= ".$min_nodes;
  }
  if ($min_clicks > 0) {
    $cond[] = "total_clicks >= ".$min_clicks;
  }
  if (count($cond) > 0) {
    $sql .= " HAVING " . implode(" AND ", $cond);
  }
  $sql .= " ORDER BY ".$order." DESC";
  if ($limit > 0) {
    $sql .= " LIMIT ".$limit;
  }
  $rows = array();
  $result = db_queryd($sql);
  if (db_error($result)) {
    watchdog("links", "links database error on query: @sql", array('@sql'=>$sql), WATCHDOG_ERROR);
  } else {
    while ($row = db_fetch_array($result)) {
      $rows[] = $row;
    }
  }
  return $rows;
}

/**
 * return a trimmed rendition of a potentially long URL
 */
function theme_display_url($url){
  $parts = parse_url($url);
  $parts['host'] = str_replace('www.','',$parts['host']);
  if(strlen($parts['host']) > 20){
    $parts['host'] = substr( $parts['host'], 0, 17 ) . '...';
  }
  if(strlen($parts['path']) > 20){
    $parts['path'] = '/...' . substr( $parts['path'], strlen($parts['path']) -17 );
  }
  return $parts['host'].$parts['path'];
}
