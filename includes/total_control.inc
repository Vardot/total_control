<?php
/**
 * @file
 * Helper functions for total control
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
function total_control_add_type_submit($form, &$form_state){
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
function total_control_remove_type_submit($form, &$form_state){
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
function total_control_views_add_display($machine_type, $content_type){
  if (!$machine_type){
    drupal_set_message('no content type provided', 'error');
    return;
  }
  
  $auto_pane_default = variable_get('total_control_auto_panels', array());
  if (!($auto_page_default['type'] === 0)){
    $view = views_get_view('control_content');
    if (!$view->display['page_tc_'.$machine_type]) { 
      total_control_add_views_page_display($view, $content_type, $machine_type);
      $defaults = variable_get('total_control_type_pages', array());
      $defaults[$machine_type] = $machine_type;
      $defaults = variable_set('total_control_type_pages', $defaults);
    }
    else {
      drupal_set_message(t('Total Control was unable to create your administrative view because a view already exists with the name: '.'page_tc_'.$machine_type));
    }
  }
  
  $auto_page_default = variable_get('total_control_auto_pages', array());
  if (!($auto_pane_default['type'] === 0)){
    $paneview = views_get_view('control_content_panes');
    $auto_panes = variable_get('total_control_auto_panels', array());
    if (!$paneview->display['pane_tc_'.$machine_type]) {
      total_control_add_views_pane_display($paneview, $content_type, $machine_type);
      $defaults = variable_get('total_control_type_panes', array());
      $defaults[$machine_type] = $machine_type;
      $defaults = variable_set('total_control_type_panes', $defaults);
    }
    else {
      drupal_set_message(t('Total Control was unable to create your administrative view because a view already exists with the name: '.'pane_tc_'.$machine_type));
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
function total_control_views_delete_page_display($type){
  $view = views_get_view('control_content');
  unset($view->display['page_tc_'.$type]);
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
function total_control_views_delete_pane_display($type){
  $view = views_get_view('control_content_panes');
  unset($view->display['pane_tc_'.$type]);
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
function total_control_add_views_pane_display(&$view, $machine_type, $content_type){
  // get plural
  $plural_type = _total_control_pluralize($content_type);
  
  // build display
  $handler = $view->new_display('panel_pane', 'New '.$plural_type.' pane', 'panel_pane_tc_'.$machine_type);
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
  $handler->override_option('title', 'New '.$plural_type);
  $handler->override_option('pane_title', 'New '.$plural_type);
  $handler->override_option('pane_description', 'Total Control '.$content_type.' summary');  
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
function total_control_add_views_page_display(&$view, $content_type, $machine_type){
  // get the plural
  $plural_type = _total_control_pluralize($content_type);
  
  // build the display
  $handler = $view->new_display('page', ucwords($plural_type.' Page'), 'page_tc_'.$machine_type);
  
  // filters differ depending on search module 
  $filters = total_control_add_filters($machine_type);
  $handler->override_option('filters', $filters);
  
  $machine_path =  str_replace('_', '-', $object->type);
  
  $handler->override_option('title', $plural_type);
  $handler->override_option('header', '<a href="/node/add/'.$machine_path.'" title="Create a new '.$content_type.'">Add a New '.$content_type.'</a>');
  $handler->override_option('header_format', '1');
  $handler->override_option('header_empty', 1);
  $handler->override_option('path', 'admin/dashboard/content/'.$machine_path);
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => $plural_type,
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
function total_control_add_filters($machine_type = NULL){
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
  
  if ($machine_type){
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
  
  if (module_exists('search')){
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
 * Changes type names into plurals.
 *
 * @param $word
 *   The word to be pluralized
 *
 */
function _total_control_pluralize($word){
 $word = trim($word);
 if (substr($word, -1, 1) == 'y'){
   $removed = strlen($word) - 1;
   $plural = substr($word, 0, $removed) . 'ies';
 }
 else if (substr($word, -1, 1) == 's'){
   $plural = $word;
 }
 else {
   $plural = $word . 's';
 }
 return $plural;
}
