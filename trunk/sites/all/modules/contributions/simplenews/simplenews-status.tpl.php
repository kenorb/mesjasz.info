<?php
// $Id: simplenews-status.tpl.php,v 1.2 2008/03/16 17:02:58 sutharsan Exp $

/**
 * @file simplenews-status.tpl.php
 * Default theme implementation to display the simplenews newsletter status.
 *
 * Available variables:
 * - $image: status image
 * - $alt: 'alt' message
 * - $title: 'title' message
 *
 * @see template_preprocess_simplenews_status()
 */
?>
    <img src="<?php print $image; ?>" width="15" height="15" alt="<?php print $alt; ?>" border="0" title="<?php print $title; ?>" />
