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
function total_control_overview_content_type_admin_title($subtype, $conf, $context) {
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
 */
function total_control_overview_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = total_control_overview_content_type_admin_title();

  $types = node_type_get_types('types');  
  $overview_cnt = array();
  $overview_usr = array();

  // Content Overview.
  foreach ($types as $type => $object) {
    if (!isset($conf) || (!array_key_exists($type, $conf['types'])) || $conf['types'][$type]) {  
      
      // Compare against type option on pane config.
      $type_sql = "SELECT count(*) FROM {node} WHERE type = :type and status = 1";
      $types = db_query($type_sql, array(':type' => $type))->fetchField();
      $type_singular = '1 ' . $object->name . ' item';
      $type_plural = '@count ' . $object->name . ' items';
      $total[$type] = format_plural($types, $type_singular, $type_plural);
      
      // Compare against comment option on pane config.
      $comment_setting = variable_get('comment_' . $type, 'comment');
      if (($comment_setting != 0) || $conf['comments'][$type]) { 
        $comment_sql = "SELECT count(DISTINCT cid) 
                          FROM {comment} c INNER JOIN {node} n ON c.nid = n.nid 
                          WHERE n.type = :type AND c.status = 1 AND n.status = 1";
        $comments = db_query($comment_sql, array(':type' => $type))->fetchField();
        $total[$type . '_comments'] =  format_plural($comments, '1 comment', '@count comments');
        
        // Compare against comment option on pane config.
        if ($conf['spam'] == 1) { 
          $spam_sql = "SELECT count(DISTINCT c.cid) 
                        FROM {comment} c INNER JOIN {node} n ON c.nid = n.nid 
                        WHERE n.type = :type and c.status = 0 AND n.status = 1";
          $spam = db_query($spam_sql, array(':type' => $type))->fetchField();
          $total[$type . '_comments_spam'] = format_plural($spam, '1 spam', '@count spam');
        } // if spam
      } // if comment
      
      $line =  $total[$type];
      $line .= (array_key_exists($type . '_comments', $total))? ' with ' . $total[$type . '_comments'] : '' ;
      $line .= (array_key_exists($type . '_comments_spam', $total))? ' (' . $total[$type . '_comments_spam'] . ')' : '' ;
      $overview_cnt[] = $line;
    } 
  } 

  // User Overview.
  if ($conf['user'] == 1) {  // compare against user option on pane config.
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
    if ((!array_key_exists($rid, $conf['roles'])) || ($conf['roles'][$rid]) && $rid != 2) {
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

  $types = node_type_get_types('types');
  $type_options = array();
  $type_defaults = array();
  $comment_defaults = array();
  foreach ($types as $type => $object) {
    $type_options[$type] = $object->name;
    $type_defaults[] = $type;
    if ($type == 'blog' || $type == 'forum topic') {
      $comment_defaults[] = $type;
    }
  }
  $form['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Include Stats on Content Types'),
    '#options' => $type_options,
    '#default_value' => ($form_state['op'] == 'add')? $type_defaults : $conf['types'],
    );
  $form['comments'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Include Comment Stats for Content Types'),
    '#options' => $type_options,
    '#default_value' => ($form_state['op'] == 'add')? $comment_defaults : $conf['comments'],
    );
  $spam_options = array(
    0 => t('no'), 
    1 => t('Include Spam Comment count'));
  $form['spam'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Spam Comment count'),
    '#options' => $spam_options,
    '#default_value' => ($form_state['op'] == 'add')? TRUE : $conf['spam'],
    );
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
