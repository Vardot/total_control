<?php

/**
 * @file
 * Theme functions.
 *
 */

/*
 * Theme function for site overview.
 */
function theme_total_control_overview($overview) {
  $output = '<div class="total-control-site-overview">';
  $output .= ' <strong>' . t('Content') . '</strong>';
  $output .= theme('item_list', $overview['content']);
  $output .= ' <strong>' . t('Users') . '</strong>';
  $output .= theme('item_list', $overview['users']);
  $output .= '</div>';

  return $output;
}

/*
 * Theme function for content overview.
 */
function theme_total_control_overview_content($overview) {
  $output = '<div class="total-control-content-overview">';
  $output .= theme('item_list', $overview);
  $output .= '</div>';

  return $output;
}

/*
 * Theme function for user overview.
 */
function theme_total_control_overview_user($overview) {
  $output = '<div class="total-control-user-overview">';
  $output .= theme('item_list', $overview);
  $output .= '</div>';

  return $output;
}

/*
 * Theme function for create content pane.
 */
function theme_total_control_create($create) {
  $output = '<div class="total-control-create">';
  $output .= theme('item_list', $create);
  $output .= '</div>';

  return $output;
}

/*
 * Theme function for panel pages.
 */
function theme_total_control_admin_table($header, $rows, $link) {
  $output = '<div class="total-control-nodequeues">';
  $output .= theme('table', $header, $rows);
  $output .= '<div class="admin-page">' . $link . '</div>';
  $output .= '</div>';

  return $output;
}