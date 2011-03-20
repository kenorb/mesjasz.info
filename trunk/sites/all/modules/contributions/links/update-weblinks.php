<?php
// $Id: update-weblinks.php,v 1.4 2007/12/14 18:52:15 syscrusher Exp $

/**
 * @file
 * PHP page for updating legacy "weblink" nodes, from the new table format from
 * Drupal 4.6.
 *
 * Adapted from a patch contributed by John Hwang.
 *
 * WARNING 1: This script works ONLY with Ber Kessels' new weblink.module for
 * Drupal 4.6. Use update-weblinks-old.php instead for older versions!
 *
 * WARNING 2: Monitoring is not yet supported, and therefore this script does not
 * migrate URL monitoring parameters. If you need that feature, you need to wait
 * a bit before deploying the new links package.
 *
 * WARNING 3: This script plays a little fast and loose with {links}.last_click_time
 * in a few cases. For the vast majority of links, it will get the correct value.
 * In the rare case where two links that were different in the old weblinks
 * module but which normalize to the same {links} record in the Links Package,
 * this converter uses the last value for last_click_time that it encounters.
 * So it isn't guaranteed to get the absolute latest value in those rare cases.
 * This is a byproduct of the fact that the old weblinks module prevented exact
 * duplicate links but did not rigorously normalize and rationalize them.
 *
 * TO USE: First, update your site to Drupal 4.7 so that your {node} table gets
 *         converted.
 *
 *         Second, install the new links module package, including links_related and
 *         links_weblink.
 *
 *         Next, copy this script to your Drupal root directory (the "top level" of
 *         your Drupal site) and run it. It will convert no more than 1000 links at
 *         a time, to minimize the chance of timeouts. You can safely re-run the
 *         script multiple times.
 *
 * THIS CODE IS EXPERIMENTAL -- NOT FOR PRODUCTION USE
 */

require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

$sql = "SELECT DISTINCT n.nid, n.title, wl.*, wln.link_title, wln.weight FROM {node} n INNER JOIN {weblinks_node} wln ON n.nid=wln.nid INNER JOIN {weblinks} wl ON wl.lid=wln.lid LEFT JOIN {links_node} ln ON wln.nid=ln.nid WHERE n.type='weblink' AND ln.nid IS NULL LIMIT 1000";
$sql2 = "INSERT INTO {links_node} (lid,nid,link_title,weight,clicks,module) values (%d,%d,'%s',%d,%d,'links_weblink')";
$sql3 = "UPDATE {links} SET last_click_time=%d WHERE lid=%d";

print('<html><body>');
$count=0;
$result = db_query($sql);
while ($row = db_fetch_object($result)) {
  $n_url = links_normalize_url($row->url);
  if (empty($n_url)) {
    print("<br>Node ".$row->nid." was not converted (empty URL)");
    continue;
  }
  $lid = links_put_link($n_url);
  if (! $lid) {
    print("<br>Node ".$row->nid." was not converted (failed link insert)");
    continue;
  } 
  links_delete_links_for_node($row->nid);
  $title = links_suggest_link_title($lid, (empty($row->link_title) ? $row->title : $row->link_title));
  $clicks = $row->clicks;
  $clicktime = $row->last_click_time;
  $result2 = db_query($sql2, $lid, $row->nid, $title, $row->weight, $clicks);
  $result3 = db_query($sql3, $clicktime, $lid);
  $count++;
}
print("<P>Processed $count weblink records. The script limits the number in one run to prevent timeouts. If no errors, rerun this script until the count is zero; it may take several iterations.");
print("<P>WARNING: Monitoring parameters are not yet supported and are currently ignored.");
print('</body></html>');
?>
