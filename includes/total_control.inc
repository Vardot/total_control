<?php

/**
 * @file total_control.inc
 *
 * Helper functions for total control
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
  // get the type  
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
    drupal_set_message('no content type provided', 'error');
    return;
  }

  $auto_pane_default = variable_get('total_control_auto_panels', array());
  $auto_page_default = variable_get('total_control_auto_pages', array());

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
        administrative view because a view already exists with the 
        name: '.'page_tc_' . $machine_type));
    }
  }
  
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
        administrative view because a view already exists with the name: ' . 
        'pane_tc_' . $machine_type));
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
  $types = node_get_types('types');  
  $items = array();

  foreach ($types as $type => $object) {
    // Compare against type option on pane config.
    if (empty($conf['types']) || (isset($conf['types']) && $conf['types'][$type] == $type)) {  
      $type_sql = "SELECT count(*) FROM {node} 
                  WHERE type = '%s' and status = 1";
      $type_query = db_query($type_sql, $type);
      $singular = '1 ' . $object->name . ' item';
      $plural = '@count ' . $object->name . ' items';
      $total[$type] = format_plural(db_result($type_query), $singular, $plural);

      // Check if comments module is enabled.
      if (module_exists('comment')) { 

        // Compare against comment options on pane config.
        if (is_array($conf['comments']) && array_key_exists($type, $conf['comments']) && $conf['comments'][$type] === $type) {
          $comment_sql = "SELECT count(DISTINCT cid) FROM {comments} c 
                        INNER JOIN {node} n ON c.nid = n.nid 
                        WHERE n.type = '%s' and c.status = 1 AND n.status = 1";
          $comment_query = db_query($comment_sql, $type);
          $total[$type . '_comments'] =  format_plural(db_result($comment_query), '1 comment', '@count comments');

          // Compare against spam option checkbox on pane config.
          if (isset($conf['spam']) && $conf['spam'] == 1) {
            $spam_sql = "SELECT count(DISTINCT c.cid) FROM {comments} c 
                        INNER JOIN {node} n ON c.nid = n.nid 
                        WHERE n.type = '%s' and c.status = 0 AND n.status = 1";
            $spam_query = db_query($spam_sql, $type);
            $total[$type . '_comments_spam'] = format_plural(db_result($spam_query), '1 spam', '@count spam');
          }
        }
      }

      $line =  $total[$type];
      $line .= (isset($total[$type . '_comments']))? ' with ' . $total[$type . '_comments'] : '' ;
      $line .= (isset($total[$type . '_comments_spam']))? ' (' . $total[$type . '_comments_spam'] . ')' : '' ;
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
function total_control_add_content_pane_settings(&$form, &$form_state) {
  $conf = $form_state['conf'];
  $types = node_get_types('types');
  $type_options = array();
  $type_defaults = array();
  $comment_defaults = array();

  // Set defaults based on pane config.
  if (isset($conf['types'])) {
    $type_defaults = $conf['types'];
  }

  if (isset($conf['comments'])) {
    $comment_defaults = $conf['comments'];
  }

  foreach ($types as $machine_type => $type) {
    $type_options[$machine_type] = $type->name;
    // Display new content types by default.
    if (!array_key_exists($machine_type, $type_defaults)) {
      $type_defaults[$machine_type] = $machine_type;
    }
    // Do not display comments on new types by default unless it's a blog or a forum.
    if (!array_key_exists($machine_type, $comment_defaults)) {
      if ($machine_type == 'blog' || $machine_type == 'forum topic') {
        $comment_defaults[$machine_type] = $machine_type;
      }
      else {
        $comment_defaults[$machine_type] = 0;
      }
    }
  }

  $form['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show <strong>post</strong> counts for the following content types'),
    '#options' => $type_options,
    '#default_value' => $type_defaults,
  );

  if (module_exists('comment')) {
    $form['comments'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Show <strong>comment</strong> counts for the following content types'),
      '#options' => $type_options,
      '#default_value' => $comment_defaults,
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
 *   An array of items for a bulleted list.
 *
 */
function total_control_get_user_overview($conf = array()) {
  $items = array();

  // Compare against user option on pane config.
  if (!$conf['user'] || $conf['user'] == 1) { 
    $user_query = db_query("SELECT count(*) FROM {users}");
    $total['users_all'] = format_plural(db_result($user_query), '1 total user', '@count total users');
    $user_active_query = db_query("SELECT count(*) FROM {users} WHERE status = 1 AND login <> 0");
    $total['users_active'] = format_plural(db_result($user_active_query), '1 active user', '@count active users');
    $user_block_query = db_query("SELECT count(*) FROM {users} WHERE status = 0");
    $total['users_block'] = format_plural(db_result($user_block_query), '1 blocked user', '@count blocked users');
    $items[] = $total['users_all'];
    $items[] = $total['users_active'];
    $items[] = $total['users_block'];
  }

  // Roles Overview
  $roles = user_roles(TRUE);
  $total['users_roles'] = '';
  foreach ($roles as $rid => $role) {
    // Compare against roles option on pane config.
    if ((!$conf['roles'] || ($conf['roles'][$rid])) && $rid != 2) {
      $user_role_query = db_query("SELECT count(*) FROM {users} u INNER JOIN {users_roles} r on u.uid = r.uid WHERE r.rid = %d", $rid);
      $total['users_role_' . $rid] .= format_plural(db_result($user_role_query), '1 user', '@count users');
      $total['users_role_' . $rid] .= ' in role: ' . $role;
      $items[] = $total['users_role_' . $rid];
    } // if not auth
  } // foreach

  return $items;
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
function total_control_add_user_pane_settings(&$form, &$form_state) {
  $conf = $form_state['conf'];

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
      '#default_value' => ($form_state['op'] == 'add' || !isset($conf['roles'])) ? $roles_defaults : $conf['roles'],
      );
  }
}
