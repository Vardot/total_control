<?php

/**
 * Implementation of specially named hook_ctools_content_types()
 */
function total_control_create_ctools_content_types() {
  return array(
    'single' => TRUE,
    'icon' => 'icon_node_form.png',
    'no title override' => TRUE,
    'title' => t('Content Creation'),
    'description' => t('Provides links to create new content.'),
    'category' => t('Total Control'),
    'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
    'defaults' => array('types' => NULL),
  );
}

function total_control_create_content_type_admin_title($subtype, $conf, $context) {
    return t('Content Creation');
}

function total_control_create_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Provides links to create new content.');
  return $block;
}

function total_control_create_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = t('Create Content');
    
  $types = node_get_types('types');
  $create = array();  
  
  foreach ($types as $type => $object){
    $row = '';
    // compare against type option on pane config
        
    if ((isset($conf['types']) && $conf['types'][$type]) || ($conf == array())){  
      
      // check access to create nodes of this type
      if (node_access('create', $type)) { 
        $object_path = str_replace('_', '-', $object->type);
        $row = l(t('Add New '.$object->name), 'node/add/'.$object_path);
      }
      
      // check access to administer content types
      if ((variable_get('total_control_configure_links', 1) == 1) 
        && user_access('administer content types')){
        $row .= ' | ' . l(t('Configure'), 'admin/content/node-type/'.$object_path);
      }
      
      if ($row) { 
        $create[] = $row;
      }
      
    } // end if type
  }
      
  // assemble content
  $content =  '<div class="total-control-content-overview">';
  $content .=     theme('item_list', $create);
  $content .= '</div>';

  $block->content = $content;
  return $block;
}

function total_control_create_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  
  $types = node_get_types('types');
  $type_options = array();
  $type_defaults = array();
  foreach ($types as $type => $object){
    $type_options[$type] = $object->name;
    $type_defaults[] = $type;
  }
  $form['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Include Create links for Content Types'),
    '#options' => $type_options,
    '#default_value' => ($form_state['op'] == 'add')? $type_defaults : $conf['types'],
    );
  
  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function total_control_create_content_type_edit_form_submit(&$form, &$form_state) {
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
