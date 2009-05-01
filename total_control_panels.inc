<?php

// default panel panes not from views
include_once('total_control_panes.inc');
// options on pane config
include_once('total_control_panes_edit.inc');

/*
 * generates data for panes
 */
function total_control_panels_content_types_data($machine = NULL){
 $items = array();
 
 // Admin Blocks
 $items['admin_overview'] = array(
   'title' => 'Site Overview',
   'category' => 'Admin Dashboard',
   'config' => TRUE,
 );
 $items['admin_users'] = array(
   'title' => 'User Overview',
   'category' => 'Admin Dashboard',
   'config' => TRUE,
 );
 $items['admin_content'] = array(
   'title' => 'Content Overview',
   'category' => 'Admin Dashboard',
   'config' => TRUE,
 );
 $items['admin_create'] = array(
   'title' => 'Create New Content',
   'category' => 'Admin Dashboard',
   'config' => TRUE,
 );
 $items['admin_tags'] = array(
   'title' => 'Categories and Tags',
   'category' => 'Admin Dashboard',
   'config' => TRUE,
 );
 
 return ($machine) ? $items[$machine] : $items; 
}

/*
 * These functions Return Pane Titles
 */
function total_control_panels_title_admin_overview() {
  $item = total_control_panels_content_types_data('admin_overview');
  return $item['title'];
}
function total_control_panels_title_admin_users() {
  $item = total_control_panels_content_types_data('admin_users');
  return $item['title'];
}
function total_control_panels_title_admin_content() {
  $item = total_control_panels_content_types_data('admin_content');
  return $item['title'];
}
function total_control_panels_title_admin_create() {
  $item = total_control_panels_content_types_data('admin_create');
  return $item['title'];
}
function total_control_panels_title_admin_tags() {
  $item = total_control_panels_content_types_data('admin_tags');
  return $item['title'];
}

/*
 * These functions Return Pane Categories too
 */
function total_control_panels_ct_admin_overview() {
  $item = total_control_panels_content_types_data('admin_overview');
  return total_control_panels_content_types_create($item['title'], $item['category']);
}
function total_control_panels_ct_admin_users() {
  $item = total_control_panels_content_types_data('admin_users');
  return total_control_panels_content_types_create($item['title'], $item['category']);
}
function total_control_panels_ct_admin_content() {
  $item = total_control_panels_content_types_data('admin_content');
  return total_control_panels_content_types_create($item['title'], $item['category']);
}
function total_control_panels_ct_admin_create() {
  $item = total_control_panels_content_types_data('admin_create');
  return total_control_panels_content_types_create($item['title'], $item['category']);
}
function total_control_panels_ct_admin_tags() {
  $item = total_control_panels_content_types_data('admin_tags');
  return total_control_panels_content_types_create($item['title'], $item['category']);
}

/*
 * General Panels Creation Function
 */
function total_control_panels_content_types_create($title, $category) {
  return array(
    'description' => array(
      'title' => $title,
      'icon' => 'icon_contrib_block.png',
      'path' => panels_get_path('plugins/content_types/block'),
      'description' => 'A panel pane to create ' . $title,
      'category' => array($category, 0),
    ),
  );
}

/*
 * General Panels Rendering Function
 */
function total_control_panels_render($title, $content, $link = NULL) {
  $block = new stdClass();
  $block->module = 'total_control';
  if ($link) {
    $block->title = l($title, $link);
  } else {
    $block->title = $title;
  }
  $block->content = $content;
  return $block;
}

/*
 * Implements hook_panels_content_types
 * generates panes for panels page
 */
function total_control_panels_content_types() {
  $data = total_control_panels_content_types_data();
  $items = array();
  foreach($data as $key => $values) {    
    $items[$key] = array(
      'title' => $values['title'],
      'single' => FALSE,
      'content_types' => 'total_control_panels_ct_' . $key,
      'title callback' => 'total_control_panels_title_' . $key,
      'render callback' => 'total_control_panels_render_' . $key,
    );
    if ($values['config']) {
      $items[$key]['add callback'] = 'total_control_panels_edit_'.$key;
      $items[$key]['edit callback'] = 'total_control_panels_edit_'.$key;
    }
    if ($values['submit']) {
      $items[$key]['add submit callback'] = $values['submit'];
      $items[$key]['edit submit callback'] = $values['submit'];
    }
    if ($values['validate']) {
      $items[$key]['add validate callback'] = $values['validate'];
      $items[$key]['edit validate callback'] = $values['validate'];
    }
  }
  return $items;
}
