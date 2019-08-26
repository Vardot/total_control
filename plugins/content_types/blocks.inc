<?php
/**
 * @file
 *
 * "Blocks" content type. It shows users with permissions statistics
 * and links to manage terms in vocabularies on the site.
 *
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Manage Blocks'),
  'defaults' => array('vids' => NULL),
  'icon' => 'cog.png',
  'description' => t('Provides links to manage blocks.'),
  'category' => t('Dashboard'),
  'edit text' => t('Configure'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_blocks_content_type_admin_title($subtype = NULL, $conf = NULL, $context = NULL) {
  return t('Manage Blocks');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_blocks_content_type_admin_info($subtype = NULL, $conf = NULL, $context = NULL) {
  $block = new stdClass();
  $block->title = t('Provides links to manage blocks.');

  return $block;
}

/**
 * Run-time rendering of the body of the block.
 */
function total_control_blocks_content_type_render($subtype, $conf, $panel_args, &$context) {
  if (!module_exists('block')) {
    return;
  }
  $block = new stdClass();
  $block->module = t('mtc_panels');
  $block->title = t('Manage Custom Blocks');

  $result = db_query("SELECT bid, info FROM {block_custom}")->fetchAll();

  $options = array('query' => array('destination' => 'admin/dashboard'));
  $header = array(
    array('data' => t('Custom Block')),
    array('data' => t('Operations'), 'colspan' => 3));
  $rows = array();

  foreach ($result as $record) {
    $data = array(t($record->info));

    if (user_access('administer blocks')) {
      $data[] = l(t('Configure'), 'admin/structure/block/manage/block/' . $record->bid . '/configure', $options);
    }

    $rows[] = array(
      'data' => $data,
    );
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('There are no custom blocks.'),
        'colspan' => 2,
      )
    );
  }

  if (user_access('administer blocks')) {
    $link = l(t('Block administration'), 'admin/structure/block', $options);
  }
  else {
    $link = '';
  }

  $block->content = theme('total_control_admin_table', array('header' => $header, 'rows' => $rows, 'link' => $link));

  return $block;
}
