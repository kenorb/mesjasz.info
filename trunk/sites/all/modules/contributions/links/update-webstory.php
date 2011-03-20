<?php
// $Id: update-webstory.php,v 1.2 2006/02/02 04:50:59 syscrusher Exp $

/**
 * @file
 * PHP page for updating legacy "webstory" nodes. "webstory.module" was never
 * released into production, but is used on Syscrusher's personal sites. This
 * script is released in hopes that it might be useful to someone as an
 * example, since I had to write it for myself anyway.
 *
 * THIS CODE IS EXPERIMENTAL -- NOT FOR PRODUCTION USE
 */

include_once 'includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

$sql = "SELECT n.nid, n.title, ws.* FROM {node} n INNER JOIN {webstory} ws ON n.nid=ws.nid WHERE n.type='webstory' ORDER BY n.nid DESC LIMIT 1000";
$sql2 = "INSERT INTO {links_node} (lid,nid,link_title,weight,clicks,module) values (%d,%d,'%s',-5,%d,'links_related')";

print('<html><body>');
$count=0;
$result = db_query($sql);
while ($row = db_fetch_object($result)) {
  $n_url = links_normalize_url($row->url);
  $lid = links_put_link($n_url);
  links_delete_links_for_node($row->nid);
  $title = links_suggest_link_title($lid, '');
  $clicks = $row->click;
  $result2 = db_query($sql2, $lid, $row->nid, $title, $clicks);
  $result3 = db_query("UPDATE {node} SET TYPE='story' WHERE nid=%d", $row->nid);
  $count++;
}
print("<P>Processed $count webstory records. The script limits the number in one run to prevent timeouts. If no errors, rerun this script until the count is zero; it may take several iterations.");
print('</body></html>');
?>
