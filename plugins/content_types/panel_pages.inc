<?php
// $Id: panel_pages.inc,v 1.3 2010/03/02 22:10:32 jenlampton Exp $

/**
 * @file
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
  'title' => t('Panel Pages list'),
  'description' => t('A list of panels on the site.'),
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
    return t('Panel Pages');
}

/**
 * 'Admin info' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_admin_info.
 *
 */
function total_control_panel_pages_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Displays a list of Panel Pages, with "edit" links.');
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
  $block = new stdClass();

  // The title actually used in rendering
  $block->title = check_plain("Panel Pages");
  
  $items = array();
  
  // get panels pages
  if (module_exists('page_manager')) {
    module_load_include('inc', 'page_manager', 'page_manager.admin');
    $tasks = page_manager_get_tasks_by_type('page');
    $pages = array('operations' => array());

    page_manager_get_pages($tasks, $pages);
    $count = 0;
    $rows = array();
    
    foreach ($pages['rows'] as $id => $info) {
      // TODO: config setting?
      // Only show enabled panels on the pane.
      if (array_key_exists('data', $info['data']['operations'])
        && (stristr((string)$info['data']['operations']['data'], 'Enable') == FALSE)){
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
    $header = array(t('Page'), t('Operations'));
    $output = theme('table', $header, $rows, array('class' => 'panels-manage'));
    $output .= '<div class="see-list">' . l(t('Go to list'), 'admin/build/pages') . '</div>';
  }
  else {
    $output = '<p>' . t('You must install and activate the Page Manager module to manage pages. This module is located within the Chaos Tools Suite.') . '</p>';
  }
  
  $block->content = $output;

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
  foreach (array('item1', 'item2') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}