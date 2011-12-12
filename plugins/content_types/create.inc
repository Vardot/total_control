<?php

/**
 * @file create.inc
 *
 * "Create content" panels content type. It shows users links to
 * create new content of all types for which they have permissions,
 * and users with "administer content types" permission an optional
 * "configure" link as well.
 *
 */
 
$plugin = array(
  'single' => TRUE,
  'title' => t('Create content'),
  'defaults' => array('types' => NULL),
  'icon' => 'cog.png',
  'description' => t('Provides links to create new content.'),
  'category' => t('Total control'),
  'edit text' => t('Configure'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_create_content_type_admin_title($subtype, $conf, $context) {
  return t('Content cnode_type_get_typesreation');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_create_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Provides links to create new content.');

  return $block;
}

/**
 * Run-time rendering of the body of the pane.
 */
function total_control_create_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = t('Create Content');

  $types = node_type_get_types();
  $create = array();  

  foreach ($types as $type => $object) {
    $row = '';
    // compare against type option on pane config
    if ((isset($conf['types']) && $conf['types'][$type]) || ($conf == array())) {  
      
      // check access, then add a link to create content.
      if (node_access('create', $type)) { 
        $type_url_str = str_replace('_', '-', $object->type);  
        $row = l(t('Add new  ' . $object->name), 'node/add/' . $type_url_str);
        
        // Check access, then add a link to administer content types.
        if ((variable_get('total_control_configure_links', 1) == 1) 
          && user_access('administer content types')) {
          $row .= ' | ' . l(t('Configure'), 'admin/structure/types/manage/' . $type_url_str);
        }
      }

      if (isset($row)) { 
        $create[] = $row;
      }
    }
  }

  $block->content =  theme('total_control_create', array('create' => $create));

  return $block;
}

/**
 * 'Edit form' callback for the content type.
 */
function total_control_create_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $types = node_type_get_types();
  $type_options = array();
  $type_defaults = array();

  foreach ($types as $type => $object) {
    $type_options[$type] = $object->name;
    $type_defaults[] = $type;
  }

  if (isset($conf['types'])) {
    $type_defaults = $conf['types'];
  }

  $form['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Include Create links for Content Types'),
    '#options' => $type_options,
    '#default_value' => $type_defaults,
  );

  return $form;
}

/**
 * Submit handler for for create settings.
 */
function total_control_create_content_type_edit_form_submit($form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

