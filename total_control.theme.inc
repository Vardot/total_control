<?php

/**
 * @file
 * Theme functions.
 *
 */

/*
 * Theme function for site overview.
 */
function theme_total_control_overview($variables){
  $overview = $variables['overview'];

  $content = '<div class="total-control-site-overview">';
  $content .= ' <strong>'.t('Content').'</strong>';
  $content .= theme('item_list', array('items' => $overview['content']));
  $content .= ' <strong>'.t('Users').'</strong>';
  $content .= theme('item_list', array('items' => $overview['users']));
  $content .= '</div>';

  return $content;
}

/*
 * Theme function for content overview.
 */
function theme_total_control_overview_content($variables){
  $overview = $variables['overview'];

  $content = '<div class="total-control-content-overview">';
  $content .= theme('item_list', array('items' => $overview));
  $content .= '</div>';

  return $content;
}

/*
 * Theme function for user overview.
 */
function theme_total_control_overview_user($variables){
  $overview = $variables['overview'];

  $content = '<div class="total-control-user-overview">';
  $content .= theme('item_list', array('items' => $overview));
  $content .= '</div>';

  return $content;
}

/*
 * Theme function for create content pane.
 */
function theme_total_control_create($variables){
  $create = $variables['create'];

  $content = '<div class="total-control-create">';
  $content .= theme('item_list', array('items' => $create));
  $content .= '</div>';

  return $content;
}

/*
 * Theme function for taxonomy pane.
 */
function theme_total_control_taxonomy($varibales){
  $taxonomy = $varibales['taxonomy'];

  $content = '<div class="total-control-taxonomy">';
  $content .= theme('item_list', array('items' => $taxonomy));
  $content .= '</div>';

  return $content;
}

/*
 * Theme function for taxonomy pane.
 */
function theme_total_control_panel_pages($varibales){
  $header = $varibales['header'];
  $rows = $varibales['rows'];
  $link = $variables['link'];

  $content = '<div class="total-control-panel-panes">';
  $content .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => 'panels-manage')));
  $content .= '<div class="see-list">' . $link . '</div>';
  $content .= '</div>';

  return $content;
}

/*
 * Theme function for taxonomy pane.
 */
function theme_total_total_control_nodequeue($varibales){
  $header = $varibales['header'];
  $rows = $varibales['rows'];
  
  $content = '<div class="total-control-nodequeues">';
  $content .= theme('table', array('header' => $header, 'rows' => $rows));
  $content .= '</div>';
}

