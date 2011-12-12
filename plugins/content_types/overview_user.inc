<?php

/**
 * @file
 *
 * "User overview" content type. Displays a summary of user statistics 
 * including: number of total users, active users, blocked users, and 
 * users in each role.
 *
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('User Overview'),
  'no title override' => TRUE,
  'defaults' => array('user' => NULL, 'roles' => NULL),
  'icon' => 'cog.png',
  'description' => t('Displays a summary of user statistics including: number 
    of total users, active users, blocked users, and users in each role.'),
  'category' => t('Total control'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_overview_user_content_type_admin_title($subtype = NULL, $conf = NULL, $context = NULL) {
  return t('User overview');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_overview_user_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Displays a summary of user statistics including: number of total users, active users, blocked users, and users in each role.');

  return $block;
} 

/**
 * Run-time rendering of the body of the block.
 */
function total_control_overview_user_content_type_render($subtype, $conf, $panel_args, &$context) {
  $overview = array();

  // User Overview
  if (!$conf['user'] || $conf['user'] == 1) {
    $user_query = db_query("SELECT count(*) FROM {users}")->fetchField();
    $total['users_all'] = format_plural($user_query, '1 total user', '@count total users');
    $user_active_query = db_query("SELECT count(*) FROM {users} WHERE status = 1 AND login != 0")->fetchField();
    $total['users_active'] = format_plural($user_active_query, '1 active user', '@count active users');
    $user_block_query = db_query("SELECT count(*) FROM {users} WHERE status = 0")->fetchField();
    $total['users_block'] = format_plural($user_block_query, '1 blocked user', '@count blocked users');
    $overview[] = $total['users_all'];
    $overview[] = $total['users_active'];
    $overview[] = $total['users_block'];
  }

  // Roles Overview
  $roles = user_roles(TRUE);
  $total['users_roles'] = '';
  foreach ($roles as $rid => $role){
    if (($rid != 2) && (array_key_exists($rid, $conf['roles']) && $conf['roles'][$rid])) {
      $user_role_query = db_query("SELECT count(*) FROM {users} u INNER JOIN {users_roles} r on u.uid = r.uid WHERE r.rid = :rid", array(':rid' => $rid))->fetchField();
      $total['users_role_' . $rid] = format_plural($user_role_query, '1 user', '@count users');
      $total['users_role_' . $rid] .= ' in role: ' . $role;
      $overview[] = $total['users_role_' . $rid];
    }
  }

  $block = new stdClass();
  $block->title = total_control_overview_user_content_type_admin_title();
  $block->content = theme('total_control_overview_user', array('overview' => $overview));

  return $block;
}

/**
 * 'Edit form' callback for the content type.
 */
function total_control_overview_user_content_type_edit_form($form, &$form_state) {
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
 */
function total_control_overview_user_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
