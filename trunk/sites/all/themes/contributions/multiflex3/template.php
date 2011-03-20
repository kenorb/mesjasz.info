<?php
// $Id: template.php,v 1.2.2.1 2008/06/19 07:18:26 hswong3i Exp $

function multiflex3_theme() {
  return array(
    'mission' => array(
      'arguments' => array('form' => NULL),
    ),
  );
}

/**
 * Return a themed mission trail.
 *
 * @return
 *   a string containing the mission output, or execute PHP code snippet if
 *   mission is enclosed with <?php ?>.
 */
function multiflex3_mission() {
  $mission = theme_get_setting('mission');
  if (preg_match('/^<\?php/', $mission)) {
    $mission = drupal_eval($mission);
  }
  else {
    $mission = filter_xss_admin($mission);
  }
  return isset($mission) ? $mission : '';
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb">'. implode('', $breadcrumb) .'</div>';
  }
}

/**
 * Generates IE CSS links for LTR and RTL languages.
 */
function phptemplate_get_ie_styles() {
  global $language;

  $iecss = '<link type="text/css" rel="stylesheet" media="all" href="'. base_path() . path_to_theme() .'/fix-ie.css" />';
  if (defined('LANGUAGE_RTL') && $language->direction == LANGUAGE_RTL) {
    $iecss .= '<style type="text/css" media="all">@import "'. base_path() . path_to_theme() .'/fix-ie-rtl.css";</style>';
  }

  return $iecss;
}

/**
 * Return a cascade primary links.
 * Clone implementation from user_block().
 *
 * @return
 *   a themed cascade primary links.
 */
function phptemplate_get_primary_links() {
  return menu_tree(variable_get('menu_primary_links_source', 'primary-links'));
}
