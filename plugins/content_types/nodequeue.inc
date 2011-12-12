<?php

/**
 * @file nodequeue.inc
 *
 * "Nodequeue" content type. It shows users with permissions all of the 
 * nodequeues and provides links directly manage them.
 *
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('NodeQueue list'),
  'defaults' => array(),
  'icon' => 'cog.png',
  'description' => t('A list of NodeQueues on the site.'),
  'category' => t('Total control'),
  'edit text' => t('Configure'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_nodequeue_content_type_admin_title($subtype, $conf, $context) {
    return t('NodeQueue overviewnode_type_get_types');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_nodequeue_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Provides a list of all NodeQueues on the site.');
  return $block;
}

/**
 * Run-time rendering of the body of the block.
 */
function nodequeue_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdClass();

  // The title actually used in rendering
  $block->title = check_plain("NodeQueues");
  
  $items = array();
  
  // Get nodequeues.
  if (module_exists('nodequeue')) {
    module_load_include('module', 'nodequeue', 'nodequeue');
    
    $queues = nodequeue_load_queues(nodequeue_get_all_qids(25));
    foreach ($queues as $queue) {
      if (!nodequeue_queue_access($queue)) {
        unset($queues[$queue->qid]);
      }
    }

    if (empty($queues)) {
      $output = t('No nodequeues exist.');
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

    $output = theme('total_control_nodequeue', $header, $rows);
  }
  
  $block->content = $output;

  return $block;

}

/**
 * 'Edit form' callback for the content type.
 */
function nodequeue_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * 'Edit form' submit callback for the content type.
 */
function nodequeue_content_type_edit_form_submit($form, &$form_state) {
  foreach (array('item1', 'item2') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}