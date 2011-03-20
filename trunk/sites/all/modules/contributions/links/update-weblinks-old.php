<?php
// $Id: update-weblinks-old.php,v 1.3 2006/02/02 04:50:59 syscrusher Exp $

/**
 * @file
 * PHP page for updating legacy "weblink" nodes, from the old table format prior to
 * Drupal 4.6.
 *
 * WARNING 1: This script will NOT work with Ber Kessels' new weblink.module for
 * Drupal 4.6. Use update-weblinks.php instead!
 *
 * WARNING 2: Monitoring is not yet supported, and therefore this script does not
 * migrate URL monitoring parameters. If you need that feature, you need to wait
 * a bit before deploying the new links package.
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

$sql = "SELECT DISTINCT n.nid, n.title, wl.* FROM {node} n INNER JOIN {weblink} wl ON n.nid=wl.nid LEFT JOIN {links_node} ln ON wl.nid=ln.nid WHERE n.type='weblink' AND ln.nid IS NULL LIMIT 1000";
$sql2 = "INSERT INTO {links_node} (lid,nid,link_title,weight,clicks,module) values (%d,%d,'%s',-5,%d,'links_weblink')";

print('<html><body>');
$count=0;
$result = db_query($sql);
while ($row = db_fetch_object($result)) {
  $n_url = links_normalize_url($row->weblink);
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
  $title = links_suggest_link_title($lid, $row->title);
  $clicks = $row->click;
  $result2 = db_query($sql2, $lid, $row->nid, $title, $clicks);
  $count++;
}
print("<P>Processed $count weblink records. The script limits the number in one run to prevent timeouts. If no errors, rerun this script until the count is zero; it may take several iterations.");
print("<P>WARNING: Monitoring parameters are not yet supported and are currently ignored.");
print('</body></html>');
?>
