<?php

/**
 * @file panel_pages.inc
 *
 * "Panel Pages" panels content pane. It shows users with permissions the
 * panel pages on the site, and provides links directly to the "content" tab.
 *
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Panel Pages list'),
  'defaults' => array(),
  'icon' => 'cog.png',
  'description' => t('A list of panels on the site.'),
  'category' => t('Total control'),
  'edit text' => t('Configure'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_panel_pages_content_type_admin_title($subtype, $conf, $context) {
  return t('Panel page overview');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_panel_pages_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Displays a list of Panel Pages, with "edit" links.');

  return $block;
}

/**
 * Run-time rendering of the body of the block.
 */
function total_control_panel_pages_content_type_render($subtype, $conf, $args, $context) {
  $items = array();

  // Get panels pages.
  module_load_include('inc', 'page_manager', 'page_manager.admin');
  $tasks = page_manager_get_tasks_by_type('page');
  $pages = array('operations' => array());

  page_manager_get_pages($tasks, $pages);
  $count = 0;

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

  // Build a link to the page manager UI.
  if (user_access('use page manager')) {
    $link = l(t('Go to list'), 'admin/build/pages');
  }

  $block = new stdClass();
  $block->title = check_plain("Panel Pages");
  $block->content = theme('total_control_panel_pages', $header, $rows, $link);

  return $block;

}

/**
 * 'Edit form' callback for the content type.
 */
function total_control_panel_pages_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * 'Edit form' submit callback for the content type.
 */
function total_control_panel_pages_content_type_edit_form_submit($form, &$form_state) {
  foreach (array('item1', 'item2') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
