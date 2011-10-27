<?php
// $Id: welcome.inc,v 1.8 2010/03/02 22:10:32 jenlampton Exp $

/**
 * @file
 * "Welcome" panels content type. Displays a summary of 
 * content statistics including: number pieces of each type of 
 * content, number of comments, number of blocked comments (spam)
 * and configuration links.
 *
 */
 
/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Welcome'),
  'description' => t('Intro to Total Control'),
  // 'single' => TRUE means has no subtypes.
  'single' => TRUE,
  // Constructor.
  'content_types' => array('total_control_welcome_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'total_control_welcome_content_type_render',
  // The default context.
  'defaults' => array('types' => NULL, 'comments' => NULL, 'spam' => 1),
  // This explicitly declares the config form. Without this line, the func would be
  // total_control_welcome_content_type_edit_form.
  'edit form' => 'total_control_welcome_content_type_edit_form',
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
function total_control_welcome_content_type_admin_title($subtype, $conf, $context) {
    return t('Welcome');
}

/**
 * 'Admin info' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_admin_info.
 *
 */
function total_control_welcome_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Intro to Total Control');
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
function total_control_welcome_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = t('Take Total Control.');
  
  // Building the content
  $content =  '<div class="total-control-site-overview">';
  $content .= '  <p>'.t('Welcome to your administrative dashboard.  !panel to add more content panes here, or configure those provided by default. Views content panes can be created for you automatically according to your !settings.  Make this dashboard the perfect hub for all your administrative needs.', array('!panel' => l(t('Edit this panel'), 'admin/build/pages/edit/dashboard'), '!settings' => l(t('Total Control Settings'), 'admin/settings/control'))).'</p>';
  $content .= '</div>';

  $block->content = $content;
  return $block;
}
