<?php

/**
 * @file
 * Theme functions.
 *
 */

/*
 * Theme function for site overview.
 */
function theme_total_control_overview($overview){
  $content = '<div class="total-control-site-overview">';
  $content .= ' <strong>'.t('Content').'</strong>';
  $content .= theme('item_list', $overview['content']);
  $content .= ' <strong>'.t('Users').'</strong>';
  $content .= theme('item_list', $overview['users']);
  $content .= '</div>';

  return $content;
}

/*
 * Theme function for content overview.
 */
function theme_total_control_overview_content($overview){
  $content = '<div class="total-control-content-overview">';
  $content .= theme('item_list', $overview);
  $content .= '</div>';

  return $content;
}

/*
 * Theme function for user overview.
 */
function theme_total_control_overview_users($overview){
  $content = '<div class="total-control-user-overview">';
  $content .= theme('item_list', $overview);
  $content .= '</div>';

  return $content;
}

/*
 * Theme function for create content pane.
 */
function theme_total_control_create($create){
  $content = '<div class="total-control-create">';
  $content .= theme('item_list', $create);
  $content .= '</div>';

  return $content;
}

/*
 * Theme function for taxonomy pane.
 */
function theme_total_control_taxonomy($taxonomy){
  $content = '<div class="total-control-taxonomy">';
  $content .= theme('item_list', $taxonomy);
  $content .= '</div>';

  return $content;
}

/*
 * Theme function for taxonomy pane.
 */
function theme_total_control_panel_pages($header, $rows, $link){
  $content = '<div class="total-control-panel-panes">';
  $content .= theme('table', $header, $rows, array('class' => 'panels-manage'));
  $content .= '<div class="see-list">' . $link . '</div>';
  $content .= '</div>';

  return $content;
}

/*
 * Theme function for taxonomy pane.
 */
function theme_total_total_control_nodequeue($header, $rows){
  $content = '<div class="total-control-nodequeues">';
  $content .= theme('table', $header, $rows);
  $content .= '</div>';
}

