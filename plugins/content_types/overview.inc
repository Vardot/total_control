<?php

/**
 * @file
 *
 * "Site overview" panel pane. Displays a summary of both content and user 
 * statistics.
 *
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Site Overview'),
  'no title override' => TRUE,
  'defaults' => array('types' => NULL, 'comments' => NULL, 'spam' => 1, 'user' => NULL, 'roles' => NULL),
  'icon' => 'cog.png',
  'description' => t('An overview of both content and users in one pane'),
  'category' => t('Total control'),
  'edit text' => t('Configure'),
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
function total_control_overview_content_type_admin_info($subtype = NULL, $conf = NULL, $context = NULL) {
  $block = new stdClass();
  $block->title = t('An overview of both content and users in one pane');

  return $block;
}

/**
 * Run-time rendering of the body of the block.
 */
function total_control_overview_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = total_control_overview_content_type_admin_title();

  $overview_cnt = total_control_get_content_overview($conf);
  $overview_usr = array();

  // User Overview.
  if ($conf['user'] == 1) { // Compare against user option on pane config.
    $users = db_query("SELECT count(*) FROM {users}")->fetchField();
    $total['users_all'] = format_plural($users, '1 total user', '@count total users');
    $users_active = db_query("SELECT count(*) FROM {users} WHERE status = 1 AND login <> 0")->fetchField();
    $total['users_active'] = format_plural($users_active, '1 active', '@count active');
    $users_blocked = db_query("SELECT count(*) FROM {users} WHERE status = 0")->fetchField();
    $total['users_block'] = format_plural($users_blocked, '1 blocked', '@count blocked');
    $overview_usr[] = $total['users_all'] . ' | ' . $total['users_active'] . ' | ' . $total['users_block'];
  }

  // Roles Overview.
  $roles = user_roles(TRUE);
  $total['users_roles'] = '';
  foreach ($roles as $rid => $role) {
    // Compare against roles option on pane config.
    if (($rid != 2) && (array_key_exists($rid, $conf['roles']) && $conf['roles'][$rid])) {
      $user_roles = db_query("SELECT count(*) FROM {users} u INNER JOIN {users_roles} r on u.uid = r.uid WHERE r.rid = :rid", array(':rid' => $rid))->fetchField();
      $total['users_role_' . $rid] = format_plural($user_roles, '1 user', '@count users');
      $total['users_role_' . $rid] .= ' in role: ' . $role;
      $overview_usr[] = $total['users_role_' . $rid];
    } // if not auth
  } // foreach

  $block->content = theme('total_control_overview', array('content' => $overview_cnt, 'users' => $overview_usr));
  return $block;
}

/**
 * 'Edit form' callback for the content type.
 */
function total_control_overview_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];  
  // Add content settings to form.
  total_control_add_content_pane_settings($form, $conf);

  $user_options = array(
    0 => t('no'), 
    1 => t('Show User Stats'));
  $form['user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include User Counts'),
    '#options' => $user_options,
    '#default_value' => ($form_state['op'] == 'add')? TRUE : $conf['user'],
    );
  $roles = user_roles(TRUE);
  $roles_options = array();
  $roles_defaults = array();
  foreach ($roles as $rid => $role) {
    if ($rid != 2) {
      $roles_options[$rid] = $role;
      $roles_defaults[] = $rid;
    }
  }
  if (!empty($roles_options)) {
    $form['roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Include User counts in Roles'),
      '#options' => $roles_options,
      '#default_value' => ($form_state['op'] == 'add')? $roles_defaults : $conf['roles'],
      );
  }

  return $form;
}

/**
 * 'Edit form' submit callback for the content type.
 */
function total_control_overview_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
