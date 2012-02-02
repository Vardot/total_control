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
  'title' => t('Admin - Menus'),
  'description' => t('Provides links to administer menus.'),
  // 'single' => TRUE means has no subtypes.
  'single' => TRUE,
  // Constructor.
  'content_types' => array('total_control_menus_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'total_control_menus_content_type_render',
  // The default context.
  'defaults' => array('menus' => array()),
  // This explicitly declares the config form. Without this line, the func would be
  // total_control_menus_content_type_edit_form.
  'edit form' => 'total_control_menus_content_type_edit_form',
  // Icon goes in the directory with the content type.
  'icon' => 'icon_node_form.png',
  'category' => t('Total Control'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_menus_content_type_admin_title($subtype, $conf, $context) {
  return t('Administer Menus');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_menus_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Provides links to administer menus.');
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
function total_control_menus_content_type_render($subtype, $conf, $args, $context) {
  $header = array(t('Page'), t('Operations'));
  $rows = array();

  $menus = menu_get_menus();
  $options = array('query' => array('destination' => 'admin/dashboard'));
  $header = array('page' => t('Menu'), 'options' => array('data' => t('Operations'), 'colspan' => 2));
  $rows = array();

  foreach ($menus as $menu_name => $menu) {
    if (!isset($conf['menus'][$menu_name]) || $conf['menus'][$menu_name] === $menu_name) {
      $rows[] = array(
        'data' => array(
          t($menu),
          l(t('Edit menu'), 'admin/build/menu-customize/' . $menu_name, $options),
          l(t('Add new link'), 'admin/build/menu-customize/' . $menu_name . '/add', $options),
        ),
      );
    }
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('There are no menus to display.'),
        'colspan' => 3,
      )
    );
  }

  // Build a link to the menu UI.
  if (user_access('administer menus')) {
    $link = l(t('Menu administration'), 'admin/build/menu');
  }

  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = t('Administer Menus');
  $block->content = theme('total_control_admin_table', $header, $rows, $link);

  return $block;

}

/**
 * 'Edit form' callback for the content type.
 */
function total_control_menus_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  $menus = menu_get_menus();
  foreach ($menus as $machine => $name) {
    if (!in_array($machine, array('management', 'user-menu', 'devel'))) {
      $menu_defaults[$machine] = $machine;
    }
  }
  $form['menus'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show links for the following menus on the dashboard'),
    '#options' => $menus,
    '#default_value' => ($conf['menus'])? $conf['menus'] : $menu_defaults,
  );

  return $form;
}

/**
 * 'Edit form' submit callback for the content type.
 */
function total_control_menus_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}