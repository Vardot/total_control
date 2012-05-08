<?php

/**
 * @file nodequeue.inc
 *
 * "Nodequeue" content type. It shows users with permissions all of
 * the nodequeues and provides links directly manage them.
 *
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Admin - Webforms'),
  'description' => t('Provides links to administer Webforms.'),
  // 'single' => TRUE means has no subtypes.
  'single' => TRUE,
  // Constructor.
  'content_types' => array('webform_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'webform_content_type_render',
  // The default context.
  'defaults' => array(),
  // This explicitly declares the config form. Without this line, the func would be
  // ctools_plugin_example_webform_content_type_edit_form.
  'edit form' => 'webform_content_type_edit_form',
  // Icon goes in the directory with the content type.
  'icon' => 'icon_node_form.png',
  'category' => t('Dashboard'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_webform_content_type_admin_title($subtype, $conf, $context) {
  return t('Administer Webforms');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_webform_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Provides links to administer Webforms.');

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
function webform_content_type_render($subtype, $conf, $args, $context) {
  $items = array();

  //Check for Webform module.
  if (module_exists('webform')) {
    // Get webforms.
    $webform_types = webform_variable_get('webform_node_types');
    $nodes = array();
    if ($webform_types) {
      $placeholders = implode(', ', array_fill(0, count($webform_types), "'%s'"));
      $result = db_query(db_rewrite_sql("SELECT n.*, r.* FROM {node} n INNER JOIN {node_revisions} r ON n.vid = r.vid WHERE n.type IN ($placeholders)", 'n', 'nid', $webform_types), $webform_types);
      while ($node = db_fetch_object($result)) {
        $nodes[] = $node;
      }
    }

    $options = array('query' => array('destination' => 'admin/dashboard'));
    $header = array(
      array('data' => t('Title'), 'field' => 'title', 'sort' => 'asc'),
      array('data' => t('Submissions'), 'field' => 'size'),
      array('data' => t('Operations')),
    );

    $rows = array();
    foreach ($nodes as $node) {
      // Count query.
      $query = 'SELECT count(*) FROM {webform_submissions} WHERE nid = %d AND is_draft = 0';
      $count = db_result(db_query($query, array($node->nid)));

      $operations = array(
        l(t('Configure'), "node/$node->nid/webform", $options),
        l(t('View results'), "node/$node->nid/webform-results/table", $options),
      );

      $rows[] = array(
        array('class' => 'webform-title', 'data' => l($node->title, 'node/' . $node->nid)),
        array('class' => 'webform-no-subs', 'data' => $count == 0 ? t('No submissions') : $count),
        array('class' => 'webform-operation', 'data' => implode(' | ', $operations)),
      );
    }
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('There are no nodequeues to display.'),
        'colspan' => 3,
      )
    );
  }

  if (user_access('administer nodequeue')) {
    $link = l(t('Nodequeue administration'), 'admin/content/nodequeue');
  }

  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = t('Administer Webforms');
  $block->content = theme('total_control_admin_table', $header, $rows, $link);

  return $block;
}

/**
 * 'Edit form' callback for the content type.
 *
 * ctools_plugin_example_webform_content_type_edit_form.
 *
 */
function webform_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  // TODO add admin config to choose node types.

  return $form;
}

/**
 * 'Edit form' submit callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_edit_form_submit.
 * The submit form stores the data in $conf.
 *
 */
function webform_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array('item1', 'item2') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}