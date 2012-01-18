<?php

/**
 * @file
 * 
 * Theme functions used in total control panel panes.
 *
 */

/*
 * Theme function for site overview.
 */
function theme_total_control_overview($variables) {
  $content = $variables['content'];
  $users = $variables['users'];

  $output  = '<div class="total-control-site-overview">';
  $output .= '<strong>' . t('Content') . '</strong>';
  $output .= theme('item_list', array('items' => $content));
  $output .= '<strong>' . t('Users') . '</strong>';
  $output .= theme('item_list', array('items' => $users));
  $output .= '</div>';

  return $output;
}

/*
 * Theme function for content overview.
 */
function theme_total_control_overview_content($variables) {
  $overview = $variables['overview'];

  $output  = '<div class="total-control-content-overview">';
  $output .= theme('item_list', array('items' => $overview));
  $output .= '</div>';

  return $output;
}

/*
 * Theme function for user overview.
 */
function theme_total_control_overview_user($variables) {
  $overview = $variables['overview'];

  $output  = '<div class="total-control-user-overview">';
  $output .= theme('item_list', array('items' => $overview));
  $output .= '</div>';

  return $output;
}

/*
 * Theme function for create content pane.
 */
function theme_total_control_create($variables) {
  $create = $variables['create'];

  $output  = '<div class="total-control-create">';
  $output .= theme('item_list', array('items' => $create));
  $output .= '</div>';

  return $output;
}

/*
 * Theme function for taxonomy pane.
 */
function theme_total_control_taxonomy($variables) {
  $taxonomy = $variables['taxonomy'];

  $output  = '<div class="total-control-taxonomy">';
  $output .= theme('item_list', array('items' => $taxonomy));
  $output .= '</div>';

  return $output;
}

/*
 * Theme function for menus.
 */
function theme_total_control_menus($variables) {
  $header = $variables['header'];
  $rows = $variables['rows'];
  $link = $variables['link'];

  $output  = '<div class="total-control-panel-panes">';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => 'panels-manage')));
  $output .= '  <div class="see-list">' . $link . '</div>';
  $output .= '</div>';

  return $output;
}

/*
 * Theme function for panel pages.
 */
function theme_total_control_panel_pages($variables) {
  $header = $variables['header'];
  $rows = $variables['rows'];
  $link = $variables['link'];

  $output  = '<div class="total-control-panel-panes">';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => 'panels-manage')));
  $output .= '  <div class="see-list">' . $link . '</div>';
  $output .= '</div>';

  return $output;
}

/*
 * Theme function for taxonomy pane.
 */
function theme_total_total_control_nodequeue($variables) {
  $header = $variables['header'];
  $rows = $variables['rows'];

  $output  = '<div class="total-control-nodequeues">';
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= '</div>';

  return $output;
}

