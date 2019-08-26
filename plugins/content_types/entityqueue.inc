<?php
/**
 * @file
 *
 * "Nodequeue" content type. It shows users with permissions all of the
 * EntityQueues and provides links directly manage them.
 *
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Manage EntityQueues'),
  'defaults' => array(),
  'icon' => 'cog.png',
  'description' => t('Provides links to manage EntityQueues.'),
  'category' => t('Dashboard'),
  'edit text' => t('Configure'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_entityqueue_content_type_admin_title($subtype = NULL, $conf = NULL, $context = NULL) {
  return t('Manage EntityQueues');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_entityqueue_content_type_admin_info($subtype = NULL, $conf = NULL, $context = NULL) {
  $block = new stdClass();
  $block->title = t('Provides links to manage EntityQueues.');

  return $block;
}

/**
 * Run-time rendering of the body of the block.
 */
function total_control_entityqueue_content_type_render($subtype, $conf, $args, &$context) {
  // Check for entityqueue module.
  if (module_exists('entityqueue')) {
    // Get EntityQueues.
    module_load_include('module', 'entityqueue', 'entityqueue');
    $queues = entityqueue_queue_load_multiple();
    $subqueues = entityqueue_subqueue_load_multiple();

    $options = array('query' => array('destination' => 'admin/dashboard'));
    $header = array(
      array('data' => t('Title'),),
      array('data' => t('Min')),
      array('data' => t('Max')),
      array('data' => t('Operations'), 'colspan' => 2),
    );
    $rows = array();
    foreach ($subqueues as $queue) {
      $queue->queue_info = $queues[$queue->queue];

      $data = array();
      $data[] = array('class' => 'entityqueue-title', 'data' => check_plain($queue->queue_info->label));
      $data[] = array('class' => 'entityqueue-min', 'data' => $queue->queue_info->settings['min_size']);
      $data[] = array('class' => 'entityqueue-max', 'data' => $queue->queue_info->settings['max_size']);

      if (user_access('administer entityqueue')) {
        $data[] = array('data' => l(t('Configure'), 'admin/structure/entityqueue/list/' . $queue->queue_info->name . '/edit', $options));
      }
      if (user_access('manipulate entityqueues')) {
        $data[] = array('data' => l(t('Manage items'), 'admin/structure/entityqueue/list/' . $queue->queue_info->name . '/subqueues/' . $queue->subqueue_id . '/edit', $options));
      }

      $rows[] = $data;
    }

    $empty_text = t('There are no EntityQueues to display.');
    $link = '';
  }
  else {
    $header = array();
    $empty_text = t('Dashboard administration for the <a href="!href">entityqueue</a> module.', array('!href' => 'http://drupal.org/project/entityqueue'));
    $link = '';
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => $empty_text,
        'colspan' => 5,
      )
    );
  }

  $block = new stdClass();
  $block->module = t('mtc_panels');
  $block->title = t('Manage EntityQueues');
  $block->content = $block->content = theme('total_control_admin_table', array('header' => $header, 'rows' => $rows, 'link' => $link));

  return $block;
}
