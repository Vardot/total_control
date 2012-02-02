<?php

/**
 * @file
 *
 * "Nodequeue" content type. It shows users with permissions all of the 
 * nodequeues and provides links directly manage them.
 *
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Admin - NodeQueues'),
  'defaults' => array(),
  'icon' => 'cog.png',
  'description' => t('Provides links to administer NodeQueues.'),
  'category' => t('Total control'),
  'edit text' => t('Configure'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_nodequeue_content_type_admin_title($subtype = NULL, $conf = NULL, $context = NULL) {
  return t('Administer NodeQueues');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_nodequeue_content_type_admin_info($subtype = NULL, $conf = NULL, $context = NULL) {
  $block = new stdClass();
  $block->title = t('Provides links to administer NodeQueues.');

  return $block;
}

/**
 * Run-time rendering of the body of the block.
 */
function total_control_nodequeue_content_type_render($subtype, $conf, $args, &$context) {
  // Check for nodequeue module. 
  if (module_exists('nodequeue')) {
    // Get nodequeues.
    module_load_include('module', 'nodequeue', 'nodequeue');
    $queues = nodequeue_load_queues(nodequeue_get_all_qids(25));
    foreach ($queues as $queue) {
      if (!nodequeue_queue_access($queue)) {
        unset($queues[$queue->qid]);
      }
    }

    $options = array('query' => array('destination' => 'admin/dashboard'));
    $header = array(
      array('data' => t('Title'),),
      array('data' => t('Size')),
      array('data' => t('Operations'), 'colspan' => 3),
    );
    $rows = array();
    foreach ($queues as $queue) {
      $rows[] = array(
        array('class' => 'nodequeue-title', 'data' => check_plain($queue->title)),
        array('class' => 'nodequeue-max-nodes', 'data' => $queue->size == 0 ? t('Infinite') : $queue->size),
        array('data' => l(t('Configure'), 'admin/content/nodequeue/' . $queue->qid . '/edit', $options)),
        array('data' => l(t('Manage items'), 'admin/content/nodequeue/' . $queue->qid . '/view', $options)),
      );
    }
  }
  
  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('There are no nodequeues to display.'),
        'colspan' => 5,
      )
    );
  }

  if (user_access('administer nodequeue')) {
    $link = l(t('NodeQueue administration'), 'admin/structure/nodequeue');
  }

  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = t('Administer NodeQueues');
  $block->content = $block->content = theme('total_control_admin_table', array('header' => $header, 'rows' => $rows, 'link' => $link));

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
