<?php

/**
 * @file
 *
 * Helper functions for total control.
 *
 */
 
/**
 * Submit function for content type form
 *
 * @param $form
 *   The form triggering the display to be added
 * @param $form_state
 *   The state of the form when the request is made
 *
 */ 
function total_control_add_type_submit($form, &$form_state) {
  // Get the type names.
  $machine_type = $form_state['values']['type'];
  $content_type = $form_state['values']['name'];
  total_control_views_add_display($machine_type, $content_type);
}

/**
 * Submit function for delete content type confirm form
 *
 * @param $form
 *   The form triggering the display to be added
 * @param $form_state
 *   The state of the form when the request is made
 *
 */ 
function total_control_remove_type_submit($form, &$form_state) {
  // get the type  
  $machine_type = $form_state['values']['type'];
  total_control_views_delete_page_display($machine_type);
  total_control_views_delete_pane_display($machine_type);
}

/**
 * Adds view displays.
 *
 * @param $machine_type
 *   The machine readable name of the content type
 * @param $content_type
 *   The human readable name of the content type
 *
 */
function total_control_views_add_display($machine_type, $content_type) {
  if (!$machine_type) {
    drupal_set_message(t('no content type provided'), 'error');
    return;
  }

  // Get total control settings.
  $auto_page_default = variable_get('total_control_auto_pages', array());
  $auto_pane_default = variable_get('total_control_auto_panels', array());

  if (!($auto_page_default['type'] === 0)) {
    $view = views_get_view('control_content');
    if (!$view->display['page_tc_' . $machine_type]) { 
      total_control_add_views_page_display($view, $content_type, $machine_type);
      $defaults = variable_get('total_control_type_pages', array());
      $defaults[$machine_type] = $machine_type;
      $defaults = variable_set('total_control_type_pages', $defaults);
    }
    else {
      drupal_set_message(t('Total Control was unable to create your 
        administrative view because a view already exists with the name: 
        !name', array('!name' => 'page_tc_' . $machine_type)));
    }
  }
  
  $auto_page_default = variable_get('total_control_auto_pages', array());
  if (!($auto_pane_default['type'] === 0)) {
    $paneview = views_get_view('control_content_panes');
    $auto_panes = variable_get('total_control_auto_panels', array());
    if (!$paneview->display['pane_tc_' . $machine_type]) {
      total_control_add_views_pane_display($paneview, $content_type, $machine_type);
      $defaults = variable_get('total_control_type_panes', array());
      $defaults[$machine_type] = $machine_type;
      $defaults = variable_set('total_control_type_panes', $defaults);
    }
    else {
      drupal_set_message(t('Total Control was unable to create your 
        administrative view because a view already exists with the name: 
        !name', array('!name' => 'pane_tc_' . $machine_type)));
    }
  }

 return;
}

/**
 * Removes view page displays.
 *
 * @param $type
 *   The machine readable name of the content type
 *
 */
function total_control_views_delete_page_display($type) {
  $view = views_get_view('control_content');
  unset($view->display['page_tc_' . $type]);
  $view->save();
  menu_cache_clear_all();
  return;
}

/**
 * Removes view pane displays.
 *
 * @param $type
 *   The machine readable name of the content type
 *
 */
function total_control_views_delete_pane_display($type) {
  $view = views_get_view('control_content_panes');
  unset($view->display['pane_tc_' . $type]);
  $view->save();
  return;
}

/**
 * Adds a pane display to the view
 *
 * @param $view
 *   The view to which the display is added
 * @param $machine_type
 *   The machine-readable content type name
 * @param $content_type
 *   The human-readable content type name
 *
 */
function total_control_add_views_pane_display(&$view, $content_type, $machine_type) {  
  // Build the display.
  $handler = $view->new_display('panel_pane', $content_type . ' pane', 'panel_pane_tc_' . $machine_type);
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        $machine_type => $machine_type,
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('title', 'New ' . $content_type);
  $handler->override_option('pane_title', 'New ' . $content_type . ' content');
  $handler->override_option('pane_description', 'Total Control ' . $content_type . ' summary');  
  $handler->override_option('pane_category', array(
    'name' => 'Total Control',
    'weight' => '0',
  ));
  $handler->override_option('allow', array(
    'use_pager' => FALSE,
    'items_per_page' => 'items_per_page',
    'offset' => FALSE,
    'link_to_view' => FALSE,
    'more_link' => FALSE,
    'path_override' => FALSE,
    'title_override' => FALSE,
    'exposed_form' => FALSE,
  ));
  $handler->override_option('argument_input', array());
  $handler->override_option('link_to_view', 0);
  $handler->override_option('inherit_panels_path', 0);
  
  // save the view and return
  $view->save();
  return;
}

/**
 * Adds a page display to an existing view
 *
 * @param $view
 *   The view to which the display is added
 * @param $content_type
 *   The human-readable content type name
 * @param $machine_type
 *   The machine-readable content type name
 *
 */
function total_control_add_views_page_display(&$view, $content_type, $machine_type) {  
  // Build the display.
  $handler = $view->new_display('page', $content_type . ' Page', 'page_tc_' . $machine_type);
  
  // filters differ depending on search module 
  $filters = total_control_add_filters($machine_type);
  $handler->override_option('filters', $filters);
  
  $machine_path =  str_replace('_', '-', $machine_type);
  
  $handler->override_option('title', $content_type . ' content');
  $handler->override_option('header', '');
  $handler->override_option('header_format', '1');
  $handler->override_option('header_empty', 1);
  $handler->override_option('path', 'admin/dashboard/content/' . $machine_path);
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => $content_type . ' content',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
  ));
  
  // save the view, flush the menu cache, and return
  $view->save();
  menu_cache_clear_all();
  return;
}

/**
 * Returns filters to a view - tests for existence of search module
 *
 * @param $machine_type
 *   The machine-readable content type name
 *
 * @return $filters
 *   An array of filters for the view
 *
 */
function total_control_add_filters($machine_type = NULL) {
  $filters = array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'operator' => '',
        'identifier' => 'status',
        'label' => 'Published',
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Override',
      ),
    ),
    'uid' => array(
      'operator' => 'in',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'uid_op',
        'identifier' => 'uid',
        'label' => 'Author',
        'optional' => 1,
        'remember' => 0,
        'reduce' => 0,
      ),
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'relationship' => 'none',
    )
  );
  
  if ($machine_type) {
    $filters['type_1'] = array(
      'operator' => 'in',
      'value' => array(
        $machine_type => $machine_type,
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type_1',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    );
  }
  
  if (module_exists('search')) {
    $filters['keys'] = array(
      'operator' => 'optional',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'keys_op',
        'identifier' => 'keys',
        'label' => 'Search:',
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'keys',
      'table' => 'search_index',
      'field' => 'keys',
      'relationship' => 'none',
    );
  }
  return $filters;
}

/**
 * Retrieves overview data for all content on the site.
 *
 * @param $conf
 *   Panel content pane config data.
 *
 * @return $items
 *   An array of items for a bulleted list.
 *
 */
function total_control_get_content_overview($conf = array()) {
  $items = array();
  $types = node_type_get_types();
  foreach ($types as $type => $object) {
    // Compare against type option on pane config.
    if ((!array_key_exists($type, $conf['types'])) || (isset($conf['types']) && $conf['types'][$type])) {  
      $type_count = db_query("SELECT count(*) FROM {node} WHERE type = :type and status = 1", array(':type' => $type))->fetchField();
      $content_data[$type] = format_plural($type_count, '1 ' . $object->name . ' item', '@count ' . $object->name . ' items');

      // compare against comment option on pane config.
      $comment_setting = variable_get('comment_' . $type, 'comment');

      if (module_exists('comment') && ($comment_setting != 0 && !array_key_exists($type, $conf['comments']) && $conf['comments'][$type] == 1)) {
        $comment_count = db_query("SELECT count(DISTINCT cid) FROM {comment} c INNER JOIN {node} n ON c.nid = n.nid WHERE n.type = :type and c.status = 1 AND n.status = 1", array(':type' => $type))->fetchField();
        $content_data[$type . '_comments'] =  format_plural($comment_count, '1 comment', '@count comments');

        if (isset($conf['spam']) && $conf['spam'] == 1) { // compare against comment option on pane config
          $spam_count = db_query("SELECT count(DISTINCT c.cid) FROM {comment} c INNER JOIN {node} n ON c.nid = n.nid WHERE n.type = :type and c.status = 0 AND n.status = 1", array(':type' => $type))->fetchField();
          $content_data[$type . '_comments_spam'] = format_plural($spam_count, '1 spam', '@count spam');
        }
      }

      $line =  $content_data[$type];
      $line .= (isset($content_data[$type . '_comments']))? ' with ' . $content_data[$type . '_comments'] : '' ;
      $line .= (isset($content_data[$type . '_comments_spam']))? ' (' . $content_data[$type . '_comments_spam'] . ')' : '' ;
      $items[] = $line;
    }
  }

  return $items;
}

/**
 * Adds content settings to the panel pane config form.
 *
 * @param $form
 *   Panel pane config form.
 * @param $conf
 *   Panel content pane config data.
 *
 */
function total_control_add_content_pane_settings(&$form, $conf = array()) {
  $types = node_type_get_types();
  $type_options = array();
  $type_defaults = array();
  $comment_defaults = array();

  foreach ($types as $machine_type => $type) {
    $type_options[$machine_type] = $type->name;
    $type_defaults[] = $machine_type;
    if ($machine_type == 'blog' || $machine_type == 'forum topic') {
      $comment_defaults[] = $machine_type;
    }
  }

  $form['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show <strong>post</strong> counts for the following content types'),
    '#options' => $type_options,
    '#default_value' => ($conf['types'])? $conf['types'] : $type_defaults,
  );

  if (module_exists('comment')) {
    $form['comments'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Show <strong>comment</strong> counts for the following content types'),
      '#options' => $type_options,
      '#default_value' => ($conf['comments'])? $conf['comments'] : $comment_defaults,
    );
    $spam_options = array(
      0 => t('no'), 
      1 => t('Include spam counts with comments')
    );
    $form['spam'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include spam counts with comments'),
      '#options' => $spam_options,
      '#default_value' => ($form_state['op'] == 'add')? 1 : $conf['spam'],
    );
  }
}

/**
 * Retrieves overview data for all users on the site.
 *
 * @param $conf
 *   Panel content pane config data.
 *
 * @return $items
 *   A structured array of items containing user data, in the following format:
 *     $user_data = array(
 *       'all' => 16,
 *       'active' => 9,
 *       'blocked' => 2,
 *       'roles' => array(
 *         3 => array(
 *           'name' => 'Administrator',
 *           'count' => 4,
 *         ),
 *       ),
 *     );
 */
function total_control_get_user_overview($conf = array()) {
  $user_data = array();

  // User Overview
  if (!$conf['user'] || $conf['user'] == 1) {
    $user_data['all'] = db_query("SELECT count(*) FROM {users}")->fetchField();
    $user_data['active'] = db_query("SELECT count(*) FROM {users} WHERE status = 1 AND login <> 0")->fetchField();
    $user_data['blocked'] = db_query("SELECT count(*) FROM {users} WHERE status = 0")->fetchField();
  }

  // Roles Overview
  $roles = user_roles(TRUE);
  $user_data['roles'] = array();
  foreach ($roles as $rid => $role) {
    if (($rid != 2) && (array_key_exists($rid, $conf['roles']) && $conf['roles'][$rid])) {
      $user_role_count = db_query("SELECT count(*) FROM {users} u INNER JOIN {users_roles} r on u.uid = r.uid WHERE r.rid = :rid", array(':rid' => $rid))->fetchField();
      $user_data['roles'][$rid] = array();
      $user_data['roles'][$rid]['name'] = $role;
      $user_data['roles'][$rid]['count'] = $user_role_count;
    }
  }

  return $user_data;
}

/**
 * Adds user settings to the panel pane config form.
 *
 * @param $form
 *   Panel pane config form.
 * @param $conf
 *   Panel content pane config data.
 *
 */
function total_control_add_user_pane_settings(&$form, $conf = array()) {
  $user_options = array(
    0 => t('no'), 
    1 => t('Include the total number of user accounts, including active and blocked.'));
  $form['user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include the total number of user accounts, including active and blocked.'),
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
      '#title' => t('Include the number of user accounts with the following roles:'),
      '#options' => $roles_options,
      '#default_value' => ($conf['roles'])? $conf['roles']: $roles_defaults,
      );
  }
}