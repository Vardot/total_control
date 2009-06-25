<?php

/**
 * Implementation of specially named hook_ctools_content_types()
 */
function total_control_overview_user_ctools_content_types() {
  return array(
    'single' => TRUE,
    'icon' => 'icon_node_form.png',
    'no title override' => TRUE,
    'title' => t('User Overview'),
    'description' => t('Displays a summary of user statistics including: number of total users, active users, blocked users, and users in each role.'),
    'category' => t('Admin Dashboard'),
    'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
    'defaults' => array('user' => NULL, 'roles' => NULL),
  );
}

function total_control_overview_user_content_type_admin_title($subtype, $conf, $context) {
  return t('User Overview');
}

function total_control_overview_user_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Displays a summary of user statistics including: number of total users, active users, blocked users, and users in each role.');
  return $block;
} 

function total_control_overview_user_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('total_control');

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
    if (!$conf['roles'] || (($conf['roles'][$rid]) && $rid != 2)){  // compare against roles option on pane config
      $user_role_query = db_query("SELECT count(*) FROM {users} u INNER JOIN {users_roles} r on u.uid = r.uid WHERE r.rid = %d", $rid);
      $total['users_role_'.$rid] .= format_plural(db_result($user_role_query), '1 user', '@count users');
      $total['users_role_'.$rid] .= ' in role: '.$role;
      $overview[] = $total['users_role_'.$rid];
    } // if not auth
  } // foreach
  
  $pane = total_control_overview_user_ctools_content_types();
  
  // assemble content
  $content =  '<div class="total-control-user-overview">';
  $content .= '  <h2 class="title">' . $pane['title'] . '</h2>';
  $content .= '  <div class="content">';
  $content .= theme('item_list', $overview);
  $content .= '  </div>';
  $content .= '</div>';

  $block->content = $content;
  return $block;
}

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
 * The submit form stores the data in $conf.
 */
function total_control_overview_user_content_type_edit_form_submit(&$form, &$form_state) {
  // For each part of the form defined in the 'defaults' array set when you
  // defined the content type, copy the value from the form into the array
  // of items to be saved. We don't ever want to use
  // $form_state['conf'] = $form_state['values'] because values contains
  // buttons, form id and other items we don't want stored. CTools will handle
  // the actual form submission.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
