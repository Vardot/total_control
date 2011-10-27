<?php
// $Id: nodequeue.inc,v 1.3 2010/03/02 22:10:32 jenlampton Exp $

/**
 * @file
 * "Nodequeue" content type. It shows users with permissions all of
 * the nodequeues and provides links directly manage them.
 *
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('NodeQueue list'),
  'description' => t('A list of NodeQueues on the site.'),
  // 'single' => TRUE means has no subtypes.
  'single' => TRUE,
  // Constructor.
  'content_types' => array('nodequeue_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'nodequeue_content_type_render',
  // The default context.
  'defaults' => array(),
  // This explicitly declares the config form. Without this line, the func would be
  // ctools_plugin_example_nodequeue_content_type_edit_form.
  'edit form' => 'nodequeue_content_type_edit_form',
  // Icon goes in the directory with the content type.
  'icon' => 'icon_node_form.png',
  'category' => t('Total Control'),
);

/**
 * 'Admin title' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_admin_title.
 *
 */
function total_control_nodequeue_content_type_admin_title($subtype, $conf, $context) {
    return t('NodeQueues');
}

/**
 * 'Admin info' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_admin_info.
 *
 */
function total_control_nodequeue_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Provides a list of all NodeQueues on the site.');
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
function nodequeue_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdClass();

  // The title actually used in rendering
  $block->title = check_plain("NodeQueues");
  
  $items = array();
  
  // Get nodequeues.
  if (module_exists('nodequeue')) {
    module_load_include('module', 'nodequeue', 'nodequeue');
    $output = '';
    
    $queues = nodequeue_load_queues(nodequeue_get_all_qids(25));
    foreach ($queues as $queue) {
      if (!nodequeue_queue_access($queue)) {
        unset($queues[$queue->qid]);
      }
    }

    if (empty($queues)) {
      return $output . t('No nodequeues exist.');
    }

    $header = array(
      array('data' => t('Title'), 'field' => 'title', 'sort' => 'asc'),
      array('data' => t('Max'), 'field' => 'size'),
      array('data' => t('Operations')),
    );
    
    $rows = array();
    foreach ($queues as $queue) {
      
      // Special love for Total Control.
      $options = array('query' => array('destination' => 'admin/dashboard'));
      $operations = array(l(t('Manage'), "admin/content/nodequeue/$queue->qid/view", $options));

      $rows[] = array(
        array('class' => 'nodequeue-title', 'data' => check_plain($queue->title)),
        array('class' => 'nodequeue-max-nodes', 'data' => $queue->size == 0 ? t('Infinite') : $queue->size),
        array('class' => 'nodequeue-operation', 'data' => implode(' | ', $operations)),
      );
    }

    $output .= theme('table', $header, $rows);
  }
  
  $block->content = $output;

  return $block;

}

/**
 * 'Edit form' callback for the content type.
 *
 * ctools_plugin_example_nodequeue_content_type_edit_form.
 *
 */
function nodequeue_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * 'Edit form' submit callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_edit_form_submit.
 * The submit form stores the data in $conf.
 *
 */
function nodequeue_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array('item1', 'item2') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}