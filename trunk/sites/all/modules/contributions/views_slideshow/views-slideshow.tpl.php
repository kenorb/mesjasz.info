<?php
// $Id: views-slideshow.tpl.php,v 1.1 2008/09/30 20:10:22 aaron Exp $
/**
 * @file views-slideshow.tpl.php
 * Default view template to display a slideshow.
 *
 * - $view: The view object.
 * - $options: Style options. See below.
 * - $rows: The output for the rows.
 * - $title: The title of this group of rows.  May be empty.
 *
 * - $options['type'] will either be ul or ol.
 * - $options['mode'] has the slideshow mode: VIEWS_SLIDESHOW_MODE_SINGLE_FRAME or VIEWS_SLIDESHOW_MODE_THUMBNAIL_HOVER
 * - $options['hover'] is 'hover' or 'hoverIntent'. Defines the mouse hover behavior, and depends on hoverIntent for the latter.
 * - $options['timer_delay'] is how many milliseconds before flipping to the next frame.
 * - $options['sort'] is VIEWS_SLIDESHOW_SORT_FORWARD, VIEWS_SLIDESHOW_SORT_REVERSE, or VIEWS_SLIDESHOW_SORT_RANDOM.
 * - $options['fade'] will either be TRUE or FALSE. If TRUE, then the slideshow will fade between frames.
 * - $options['fade_speed'] will be VIEWS_SLIDESHOW_FADE_SPEED_SLOW, VIEWS_SLIDESHOW_FADE_SPEED_NORMAL, or VIEWS_SLIDESHOW_FADE_SPEED_FAST.
 * - $options['fade_value'] determines the opacity to fade down to, defaulting at VIEWS_SLIDESHOW_DEFAULT_FADE_VALUE (0.25). Between 0 and 1.


 $sort, $fade, $fade_speed, $fade_value
 * @ingroup views_templates
 */

  $base = drupal_get_path('module', 'views_slideshow');
  drupal_add_js($base . '/js/views_slideshow.js', 'module');
  drupal_add_css($base . '/css/views_slideshow.css', 'module');

  $js = theme('views_slideshow_div_js', $rows, $options, $id);
  drupal_add_js($js, 'inline');

  $teaser = ($options['hover_breakout'] == VIEWS_SLIDESHOW_HOVER_BREAKOUT_TEASER ? TRUE : FALSE);

  // if we're using the 'thumbnail hover' mode, then we need to display all the view thumbnails
  if ($options['mode'] == VIEWS_SLIDESHOW_MODE_THUMBNAIL_HOVER) {
    $view_teasers = theme('views_slideshow_breakout_teasers', $rows, $id);
    if (!$options['teasers_last']) {
      $output .= $view_teasers;
    }
  }

  // these are hidden elements, used to cycle through the main div
  if ($options['mode'] != VIEWS_SLIDESHOW_MODE_JCAROUSEL) {
    $hidden_elements = theme('views_slideshow_no_display_section', $view, $rows, $id, $options['mode'], $teaser);
    $output .= theme('views_slideshow_main_section', $id, $hidden_elements);
  }
//  else {
//    $output .= theme('views_slideshow_jcarousel', $items, $id, $skin);
//  }

  if ($view_teasers && $options['teasers_last']) {
    $output .= $view_teasers;
  }

  print $output;
  return;

?>
<div class="views-slideshow item-list">
  <?php if (!empty($title)) : ?>
    <h3><?php print $title; ?></h3>
  <?php endif; ?>
  <<?php print $options['type']; ?>>
    <?php foreach ($rows as $row): ?>
      <li><?php print $row ?></li>
    <?php endforeach; ?>
  </<?php print $options['type']; ?>>
</div>