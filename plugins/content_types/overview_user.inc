<?php
// $Id: overview_user.inc,v 1.8 2010/03/02 22:10:32 jenlampton Exp $

/**
 * @file
 * "User overview" content type. Displays a summary of user statistics 
 * including: number of total users, active users, blocked users, and 
 * users in each role.
 *
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('User Overview'),
  'description' => t('Displays a summary of user statistics including: number of total users, active users, blocked users, and users in each role.'),
  // 'single' => TRUE means has no subtypes.
  'single' => TRUE,
  // Constructor.
  'content_types' => array('total_control_overview_user_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'total_control_overview_user_content_type_render',
  // The default context.
  'defaults' => array('user' => NULL, 'roles' => NULL),
  // This explicitly declares the config form. Without this line, the func would be
  // total_control_overview_user_content_type_edit_form.
  'edit form' => 'total_control_overview_user_content_type_edit_form',
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
function total_control_overview_user_content_type_admin_title($subtype, $conf, $context) {
  return t('User Overview');
}

/**
 * 'Admin info' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_admin_info.
 *
 */
function total_control_overview_user_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Displays a summary of user statistics including: number of total users, active users, blocked users, and users in each role.');
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
function total_control_overview_user_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('total_control');
  $pane = total_control_overview_user_ctools_content_types();
  $block->title = $pane['title'];

  $types = node_get_types('types');  
  $overview = array();
  
  // User Overview
  if (!$conf['user'] || $conf['user'] == 1){  // compare against user option on pane config
    $user_query = db_query("SELECT count(*) FROM {users}");
    $total['users_all'] = format_plural(db_result($user_query), '1 total user', '@count total users');
    $user_active_query = db_query("SELECT count(*) FROM {users} WHERE status = 1 AND login != 0");
    $total['users_active'] = format_plural(db_result($user_active_query), '1 active user', '@count active users');
    $user_block_query = db_query("SELECT count(*) FROM {users} WHERE status = 0");
    $total['users_block'] = format_plural(db_result($user_block_query), '1 blocked user', '@count blocked users');
    $overview[] = $total['users_all'];
    $overview[] = $total['users_active'];
    $overview[] = $total['users_block'];
  }
  
  // Roles Overview
  $roles = user_roles(TRUE);
  $total['users_roles'] = '';
  foreach ($roles as $rid => $role){
    if ((!$conf['roles'] || ($conf['roles'][$rid])) && $rid != 2){  // compare against roles option on pane config
      $user_role_query = db_query("SELECT count(*) FROM {users} u INNER JOIN {users_roles} r on u.uid = r.uid WHERE r.rid = %d", $rid);
      $total['users_role_'.$rid] .= format_plural(db_result($user_role_query), '1 user', '@count users');
      $total['users_role_'.$rid] .= ' in role: '.$role;
      $overview[] = $total['users_role_'.$rid];
    } // if not auth
  } // foreach
  
  // assemble content
  $content =  '<div class="total-control-user-overview">';
  $content .= theme('item_list', $overview);
  $content .= '</div>';

  $block->content = $content;
  return $block;
}

/**
 * 'Edit form' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_edit_form.
 *
 */
function total_control_overview_user_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  
  $form['user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include User Stats'),
    '#default_value' => ($conf['user'])? $conf['user']: TRUE,
    );
  $roles = user_roles(TRUE);
  $roles_options = array();
  $roles_defaults = array();
  foreach ($roles as $rid => $role){
    if ($rid != 2){
      $roles_options[$rid] = $role;
      $roles_defaults[] = $rid;
    }
  }
  if (!empty($roles_options)){
    $form['roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Include User counts for Roles'),
      '#options' => $roles_options,
      '#default_value' => ($conf['roles'])? $conf['roles']: $roles_defaults,
      );
  }

  return $form;
}

/**
 * 'Edit form' submit callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_edit_form_submit.
 * The submit form stores the data in $conf.
 *
 */
function total_control_overview_user_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
