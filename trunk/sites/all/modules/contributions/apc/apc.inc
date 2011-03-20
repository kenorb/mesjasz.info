<?php

global $apc_page_stats;
$apc_page_stats = array (
  'get'       => 0,
  'set'       => 0,
  'hit'       => 0,
  'miss'      => 0,
  'delete'    => 0,
);


/**
 * Return data from the persistent cache.
 *
 * @param $key
 *   The key of the data to retrieve.
 * @param $bin
 *   The bin/silo to look in. Valid core values are 'cache_filter',
 *   'cache_menu', 'cache_page', or 'cache' for the default cache.
 */
function cache_get($cid, $bin = 'cache') {
  $cache = apc_handle_get($cid, $bin);
  if (is_object($cache) && $cache->serialized) {
    $cache->data = unserialize($cache->data);
  }
  return $cache;
}

/**
 * Store data in the persistent cache.
 *
 * @param $cid
 *   The cache ID of the data to store.
 * @param $bin
 *   The bin/silo to store the data in. Valid core values are 'cache_filter',
 *   'cache_menu', 'cache_page', or 'cache'.
 * @param $data
 *   The data to store in the cache.
 * @param $expire
 *   One of the following values:
 *   - CACHE_PERMANENT: Indicates that the item should never be removed unless
 *     explicitly told to using cache_clear_all() with a cache ID.
 *   - CACHE_TEMPORARY: Indicates that the item should be removed at the next
 *     general cache wipe.
 *   - A Unix timestamp: Indicates that the item should be kept at least until
 *     the given time, after which it behaves like CACHE_TEMPORARY.
 * @param $headers
 *   A string containing HTTP header information for cached pages.
 */
function cache_set($cid, $data, $bin = 'cache', $expire = CACHE_PERMANENT, $headers = NULL) {
  // Create new cache object.
  $cache = new stdClass;
  $cache->cid = $cid;
  $cache->created = time();
  $cache->expire = $expire;
  $cache->headers = $headers;
  if (is_array($data) || is_object($data)) {
    $cache->serialized = TRUE;
    $cache->data = serialize($data);
  }
  else {  
    $cache->serialized = FALSE;
    $cache->data = $data;
  }

  // If error on set, add it to the database cache
  if (!apc_handle_set($cid, $cache, $expire, $bin)) {
    watchdog("Unable to set apc cache: $bin -> $cid", WATCHDOG_ERROR);
  }
}

function cache_clear_all($cid = NULL, $bin = NULL, $wildcard = FALSE) {
  $table = empty($table) ? 'cache' : $table;
  if (empty($cid)) {
    apc_handle_flush($table);
  }
  else {
    apc_handle_delete($cid, $table);
  }
}

/**
 * Return data from the apc cache.
 *
 * @param $key
 *   The key of the data to retrieve.
 * @param $bin
 *   The bin/silo to look in. 
 */
function apc_handle_get($key, $bin = 'cache') {
  global $apc_page_stats;
  $apc_page_stats['get']++;
  
  $full_key = apc_handle_key($key, $bin);
  if (isset($full_key)) {
    $return = apc_fetch($full_key);
    if ($return === FALSE) {  
      watchdog('apc', 'Failed to get key: ' . $full_key, WATCHDOG_ERROR);
      $return = FALSE;
    }
    else {
      $apc_page_stats['hit']++;
    }
  }
  else {
        $apc_page_stats['miss']++;
    $return = FALSE; 
  }
  
  return $return;
}

/**
 * Set data into apc cache.  We have to keep track of the bins ourselves since apc doesn't have the concept
 * of "bins".  So we keep a lookup table hash to be able to clear a single bin via flush.
 *
 * @param $key
 *   The key of the data to retrieve.
 * @param $bin
 *   The bin/silo to look in. 
 */
function apc_handle_set($key, $data, $expire = CACHE_PERMENANT, $bin = 'cache') {
  global $apc_page_stats;
  $apc_page_stats['set']++;
  
  if (!empty($key) && !empty($data)) {
    if (!apc_get_lock($bin)) {
      watchdog('apc', 'Unable to get lock for bin: ' . $bin, WATCHDOG_ERROR);
      return FALSE;
    }
    
    // Get lookup table to be able to keep track of bins
    $lookup = apc_fetch('apc_lookup_' . $bin);
    
    // If the lookup table is empty, initialize table
    if (empty($lookup)) {
      $lookup = array();
    }
    
    // Get full key for storage and set it to 1 so we can keep track of the bin
    $full_key = apc_handle_key($key, $bin);
    $lookup[$full_key] = 1;
    
    // Attempt to store full key and value
    if (!apc_store($full_key, $data, $expire)) {
      unset($lookup[$full_key]);
      $return = FALSE;
    }
    else {
      $return = TRUE;
    }
  }
  // Resave the lookup table (even on failure)
  apc_store('apc_lookup_' . $bin, $lookup);
  
  apc_remove_lock($bin);
  
  return $return;
}

/**
 * Delete data from the apc cache
 *
 * @param $key
 *   The key of the data to retrieve.
 * @param $bin
 *   The bin/silo to look in. 
 */
function apc_handle_delete($key, $bin = 'cache') {
  global $apc_page_stats;
  $apc_page_stats['delete']++;
  
  if (!empty($key) && !empty($data)) {
    $full_key = apc_handle_key($key, $bin);
    if (!apc_delete($full_key)) {
      watchdog('apc', 'Failed to delete key: ' . $full_key, WATCHDOG_ERROR);
      $return = FALSE;
    }
    else {
      $return = TRUE;
    }
  }
}

/**
 * Flush data from apc cache
 *
 * @param $key
 *   The key of the data to retrieve.
 * @param $bin
 *   The bin/silo to look in. 
 */
function apc_handle_flush($bin = 'cache') {
  if (!apc_get_lock($bin)) {
    watchdog('apc', 'Unable to get lock for bin: ' . $bin, WATCHDOG_ERROR);
    return FALSE;
  }
  
  // Get lookup table to be able to keep track of bins
  $lookup = apc_fetch('apc_lookup_' . $bin);
  
  // If the lookup table is empty, remove lock and return
  if (empty($lookup)) {
    apc_remove_lock($bin);
    return TRUE;
  }
  
  // Cycle through keys and remove each entry from the cache
  foreach ($lookup as $key => $val) {
    if (!apc_delete($key)) {
      watchdog('apc', "Unable to remove $key from $bin");
    }
    else {
      unset($lookup[$key]);
    }
  }
  
  // Resave the lookup table (even on failure)
  apc_store('apc_lookup_' . $bin, $lookup);
  
  // Remove lock
  apc_remove_lock($bin);
  
  return TRUE;
}

/**
 * Return stats about the apc cache
 *
 * @param $key
 *   The key of the data to retrieve.
 * @param $bin
 *   The bin/silo to look in. 
 */
function apc_handle_stats($bin = 'cache') {
  //TODO: need to figure out what i want to do here
  return print_r(apc_cache_info('user'), TRUE);
}

/**
 * Return key for apc cache
 *
 * @param $key
 *   The key of the data to retrieve.
 * @param $bin
 *   The bin/silo to look in. 
 */
function apc_handle_key($key, $bin) {
  static $prefix;

  if (empty($prefix)) {
    $prefix = variable_get('apc_key_prefix', '');
  }
  $full_key = ($prefix ? $prefix . '-' : '') . $bin . '-' . $key;
  
  return urlencode($full_key);
}

function apc_get_lock($bin) {
  apc_store('apc_lock_' . $bin, TRUE);
  return TRUE;
}

function apc_remove_lock($bin) {
  // Remove lock
  apc_delete('apc_lock_' . $bin); 
}