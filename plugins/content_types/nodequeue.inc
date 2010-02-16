<?php
// $Id$

/**
 * @file
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
  'title' => t('Nodeqeue list'),
  'description' => t('A list of nodequeues on the site.'),

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
  $block->title = check_plain("Node Queues");
  
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

function nodequeue_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array('item1', 'item2') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}