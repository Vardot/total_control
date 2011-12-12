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
  'defaults' => array('type' => NULL, 'comments' => NULL, 'spam' => 1, 'user' => NULL, 'roles' => NULL),
  'icon' => 'cog.png',
  'description' => t('An overview of both content and users in one pane'),
  'category' => t('Total control'),
  'edit text' => t('Configure'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_overview_content_type_admin_title($subtype, $conf, $context) {
  return t('Content and Usernode_type_get_types overview');
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
  $pane = total_control_overview_ctools_content_types();
  $block->title = $pane['title'];

  $types = node_get_types('types');  
  $overview_cnt = array();
  $overview_usr = array();

  // Content Overview.
  foreach ($types as $type => $object) {
    if (($conf['types'] == NULL) || $conf['types'][$type]) {  
      
      // Compare against type option on pane config.
      $type_sql = "SELECT count(*) FROM {node} WHERE type = '%s' and status = 1";
      $type_query = db_query($type_sql, $type);
      $type_result = db_result($type_query);
      $type_singular = '1 ' . $object->name . ' item';
      $type_plural = '@count ' . $object->name . ' items';
      $total[$type] = format_plural($type_result, $type_singular, $type_plural);
      
      // Compare against comment option on pane config.
      $comment_setting = variable_get('comment_' . $type, 'comment');
      if (($comment_setting != 0) || $conf['comments'][$type]) { 
        $comment_sql = "SELECT count(DISTINCT cid) 
                          FROM {comments} c INNER JOIN {node} n ON c.nid = n.nid 
                          WHERE n.type = '%s' AND c.status = 1 AND n.status = 1";
        $comment_query = db_query($coment_sql, $type);
        $comment_result = db_result($comment_query);
        $total[$type . '_comments'] =  format_plural($comment_result, '1 comment', '@count comments');
        
        // Compare against comment option on pane config.
        if ($conf['spam'] == 1) { 
          $spam_sql = "SELECT count(DISTINCT c.cid) 
                        FROM {comments} c INNER JOIN {node} n ON c.nid = n.nid 
                        WHERE n.type = '%s' and c.status = 0 AND n.status = 1";
          $spam_query = db_query($spam_sql, $type);
          $total[$type . '_comments_spam'] = format_plural(db_result($spam_query), '1 spam', '@count spam');
        } // if spam
      } // if comment
      
      $line =  $total[$type];
      $line .= ($total[$type . '_comments'])? ' with ' . $total[$type . '_comments'] : '' ;
      $line .= ($total[$type . '_comments_spam'])? ' (' . $total[$type . '_comments_spam'] . ')' : '' ;
      $overview_cnt[] = $line;
    } 
  } 

  // User Overview.
  if ($conf['user'] == 1) {  // compare against user option on pane config.
    $user_query = db_query("SELECT count(*) FROM {users}");
    $total['users_all'] = format_plural(db_result($user_query), '1 total user', '@count total users');
    $user_active_query = db_query("SELECT count(*) FROM {users} WHERE status = 1 AND login <> 0");
    $total['users_active'] = format_plural(db_result($user_active_query), '1 active', '@count active');
    $user_block_query = db_query("SELECT count(*) FROM {users} WHERE status = 0");
    $total['users_block'] = format_plural(db_result($user_block_query), '1 blocked', '@count blocked');
    $overview_usr[] = $total['users_all'] . ' | ' . $total['users_active'] . ' | ' . $total['users_block'];
  }

  // Roles Overview.
  $roles = user_roles(TRUE);
  $total['users_roles'] = '';
  foreach ($roles as $rid => $role) {
    if (($conf['roles'][$rid]) && $rid != 2) {  // compare against roles option on pane config
      $user_role_query = db_query("SELECT count(*) FROM {users} u INNER JOIN {users_roles} r on u.uid = r.uid WHERE r.rid = %d", $rid);
      $total['users_role_' . $rid] .= format_plural(db_result($user_role_query), '1 user', '@count users');
      $total['users_role_' . $rid] .= ' in role: ' . $role;
      $overview_usr[] = $total['users_role_' . $rid];
    } // if not auth
  } // foreach

  $block->content = theme('total_control_overview', $overview_cnt, $overview_usr);
  return $block;
}

/**
 * 'Edit form' callback for the content type.
 */
function total_control_overview_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $types = node_get_types('types');
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
