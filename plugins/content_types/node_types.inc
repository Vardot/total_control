<?php

/**
 * @file panel_pages.inc
 *
 * "Panel Pages" content type. It shows users with permissions the panel
 * pages on the site, and provides links directly to the "content" so
 * it can be changed without so many clicks via the panels UI.
 *
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Admin - Content types'),
  'description' => t('Provides links to administer content types.'),
  // 'single' => TRUE means has no subtypes.
  'single' => TRUE,
  // Constructor.
  'content_types' => array('total_control_node_types_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'total_control_node_types_content_type_render',
  // The default context.
  'defaults' => array('types' => array()),
  // This explicitly declares the config form. Without this line, the func would be
  // total_control_node_types_content_type_edit_form.
  'edit form' => 'total_control_node_types_content_type_edit_form',
  // Icon goes in the directory with the content type.
  'icon' => 'icon_node_form.png',
  'category' => t('Total Control'),
);

/**
 * 'Admin title' callback for the content type.
 *
 * ctools_plugin_example_total_control_node_types_content_type_admin_title.
 *
 */
function total_control_node_types_content_type_admin_title($subtype, $conf, $context) {
  return t('Administer Content types');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_node_types_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Provides links to administer content types.');

  return $block;
}

/**
 * Run-time rendering of the body of the block.
 *
 * @param $subtype
 * @param $conf
 *   Configuration as done at admin time.
 * @param $args
 * @param $context
 *   Context - in this case we don't have any.
 *
 * @return
 *   An object with at least title and content members.
 */
function total_control_node_types_content_type_render($subtype, $conf, $args, $context) {
  $header = array(t('Page'), t('Operations'));
  $rows = array();

  $types = node_get_types('types');
  $access = user_access('administer content types');
  $options = array('query' => array('destination' => 'admin/dashboard'));
  $header = array(
    array('data' => t('Content type')), 
    array('data' => t('Operations'), 'colspan' => 3)
  );
  $rows = array();

  foreach ($types as $type => $object) {
    // Config data says to include the type.
    if ((!array_key_exists($type, $conf['types'])) || (isset($conf['types']) && $conf['types'][$type] == $type)) {  
      // Check access, then add a link to create content.
      if ($access) {
        $type_url_str = str_replace('_', '-', $object->type);  
        $rows[$type] = array(
          'data' => array(
            t($object->name),
            l(t('Configure'), 'admin/content/node-type/' . $type_url_str, $options),
          ),
        );
        if (module_exists('content')) {
          $rows[$type]['data'][] = l(t('Manage fields'), 'admin/content/node-type/' . $type_url_str . '/fields', $options);
          $rows[$type]['data'][] = l(t('Manage display'), 'admin/content/node-type/' . $type_url_str . '/display', $options);
        }
      }
    }
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('There are no content types to display.'),
        'colspan' => 4,
      )
    );
  }

  // Build a link to the menu UI.
  if (user_access('administer content types')) {
    $link = l(t('Content type administration'), 'admin/content/types');
  }

  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = t('Administer Content types');
  $block->content = theme('total_control_admin_table', $header, $rows, $link);

  return $block;

}

/**
 * 'Edit form' callback for the content type.
 */
function total_control_node_types_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  $types = node_get_types('types');
  $type_options = array();
  $type_defaults = array();

  if (isset($conf['types'])) {
    $type_defaults = $conf['types'];
  }

  foreach ($types as $type => $object) {
    $type_options[$type] = $object->name;
    if (!array_key_exists($type, $type_defaults)) {
      $type_defaults[$type] = $type;
    }
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
 * 'Edit form' submit callback for the content type.
 */
function total_control_node_types_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}