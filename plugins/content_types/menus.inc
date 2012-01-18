<?php

/**
 * @file
 *
 * "Panel Pages" panels content pane. It shows users with permissions the
 * panel pages on the site, and provides links directly to the "content" tab.
 *
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Menus'),
  'defaults' => array(),
  'icon' => 'cog.png',
  'description' => t('A list of menus on the site.'),
  'category' => t('Total control'),
  'edit text' => t('Configure'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_menus_content_type_admin_title($subtype = NULL, $conf = NULL, $context = NULL) {
  return t('Menu overview');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_menus_content_type_admin_info($subtype = NULL, $conf = NULL, $context = NULL) {
  $block = new stdClass();
  $block->title = t('Displays a list of Menus, with access to edit links within.');

  return $block;
}

/**
 * Run-time rendering of the body of the block.
 */
function total_control_menus_content_type_render($subtype, $conf, $args, &$context) {
  $items = array();

  // Get panels pages.
  $menus = menu_get_menus();

  $count = 0;
  $header = array(); // TODO
  $rows = array();

  foreach ($menus as $menu_name => $menu) {
    // Don't show ever show Management, or Devel.
    if (!in_array($menu_name, array('management', 'devel'))) {
      $rows[] = array(
        'data' => array(
          t($menu),
          l(t('Edit links'), 'admin/structure/menu/manage/' . $menu_name),
        ),
      );
      // Only show 10. // TODO setting.
      if (++$count >= 10) {
        break;
      }
    }
  }

  // Build a link to the page manager UI.
  if (user_access('administer menu')) {
    $link = l(t('Menu administration'), 'admin/structure/menus');
  }

  $block = new stdClass();
  $block->title = t('Menus');
  $block->module = t('total_control');
  // TODO
  $block->content = theme('total_control_menus', array('header' => $header, 'rows' => $rows, 'link' => $link));

  return $block;

}

/**
 * 'Edit form' callback for the content type.
 */
function total_control_menus_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * 'Edit form' submit callback for the content type.
 */
function total_control_menus_content_type_edit_form_submit($form, &$form_state) {
  foreach (array('item1', 'item2') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
