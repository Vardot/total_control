<?php

/**
 * @file panel_pages.inc
 *
 * "Panel Pages" content type. It shows users with permissions the panel
 * pages on the site, and provides links directly to the "content" so
 * it can be changed without so many clicks via the panels UI.
 *
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Admin - Panel Pages'),
  'description' => t('Provides links to administer panel pages.'),
  // 'single' => TRUE means has no subtypes.
  'single' => TRUE,
  // Constructor.
  'content_types' => array('total_control_panel_pages_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'total_control_panel_pages_content_type_render',
  // The default context.
  'defaults' => array(),
  // This explicitly declares the config form. Without this line, the func would be
  // total_control_panel_pages_content_type_edit_form.
  'edit form' => 'total_control_panel_pages_content_type_edit_form',
  // Icon goes in the directory with the content type.
  'icon' => 'icon_node_form.png',
  'category' => t('Total Control'),
);

/**
 * 'Admin title' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_admin_title.
 *
 */
function total_control_panel_pages_content_type_admin_title($subtype, $conf, $context) {
    return t('Administer Panel pages');
}

/**
 * 'Admin info' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_admin_info.
 *
 */
function total_control_panel_pages_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Provides links to administer panel pages.');

  return $block;
}

/**
 * Run-time rendering of the body of the block.
 *
 * @param $subtype
 * @param $conf
 *   Configuration as done at admin time.
 * @param $args
 * @param $context
 *   Context - in this case we don't have any.
 *
 * @return
 *   An object with at least title and content members.
 */
function total_control_panel_pages_content_type_render($subtype, $conf, $args, $context) {
  $items = array();
  
  // Get panels pages.
  module_load_include('inc', 'page_manager', 'page_manager.admin');
  $tasks = page_manager_get_tasks_by_type('page');
  $pages = array('operations' => array());

  page_manager_get_pages($tasks, $pages);
  $count = 0; // TODO
  $header = array(t('Page'), t('Operations'));
  $rows = array();
  
  foreach ($pages['rows'] as $id => $info) {
    // TODO: config setting?
    // Only show enabled panels on the pane.
    if (array_key_exists('data', $info['data']['operations'])
      && (stristr((string)$info['data']['operations']['data'], 'Enable') == FALSE)) {
      $rows[] = array(
        'data' => array(
          $info['data']['title'],
          $info['data']['operations'],
        ),
        'class' => $info['class'],
      );

      // Only show 10.
      if (++$count >= 10) {
        break;
      } 
    }
  }
  
  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('You have no panel pages yet.'),
        'colspan' => 3,
      )
    );
  }
  
  // Build a link to the page manager UI.
  if (user_access('use page manager')) {
    $link = l(t('Page manager administration'), 'admin/build/pages');
  }

  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = t('Administer Panel pages');
  $block->content = theme('total_control_admin_table', $header, $rows, $link);

  return $block;

}

/**
 * 'Edit form' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_edit_form.
 *
 */
function total_control_panel_pages_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * 'Edit form' submit callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_edit_form_submit.
 * The submit form stores the data in $conf.
 *
 */
function total_control_panel_pages_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}