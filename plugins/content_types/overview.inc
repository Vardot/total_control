<?php

/**
 * @file overview.inc
 *
 * "Site overview" panels content type. Displays a summary of 
 * content and user statistics.
 *
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Overview - Content & User accounts'),
  'description' => t('An overview of both content and users in one pane'),
  // 'single' => TRUE means has no subtypes.
  'single' => TRUE,
  // Constructor.
  'content_types' => array('total_control_overview_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'total_control_overview_content_type_render',
  // The default context.
   'defaults' => array('types' => NULL, 'comments' => NULL, 'spam' => 1, 'user' => NULL, 'roles' => NULL),
  // This explicitly declares the config form. Without this line, the func would be
  // total_control_overview_content_type_edit_form.
  'edit form' => 'total_control_overview_content_type_edit_form',
  // Icon goes in the directory with the content type.
  'icon' => 'icon_node_form.png',
  'category' => t('Total Control'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_overview_content_type_admin_title($subtype = NULL, $conf = NULL, $context = NULL) {
  return t('Content and User overview');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_overview_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('An overview of both content and users in one pane');

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
function total_control_overview_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = t('Content and User overview');

  $overview_content = total_control_get_content_overview($conf);
  $overview_users = total_control_get_user_overview($conf);
  $block->content = theme('total_control_overview', array(
    'content' => $overview_content, 
    'users' => $overview_users,
    )
  );

  return $block;
}

/**
 * 'Edit form' callback for the content type.
 */
function total_control_overview_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  total_control_add_content_pane_settings($form, $form_state);
  total_control_add_user_pane_settings($form, $form_state);

  return $form;
}

/**
 * 'Edit form' submit callback for the content type.
 */
function total_control_overview_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
