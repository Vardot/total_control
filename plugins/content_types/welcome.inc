<?php

/**
 * Implementation of specially named hook_ctools_content_types()
 */
function total_control_welcome_ctools_content_types() {
  return array(
    'single' => TRUE,
    'icon' => 'icon_node_form.png',
    'no title override' => TRUE,
    'title' => t('Welcome'),
    'description' => t('Intro to Total Control'),
    'category' => t('Total Control'),
  );
}

function total_control_welcome_content_type_admin_title($subtype, $conf, $context) {
    return t('Welcome');
}

function total_control_welcome_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Intro to Total Control');
  return $block;
}

function total_control_welcome_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = t('Congratulations! You are now in Total Control.');
  
  // Building the content
  $content =  '<div class="total-control-site-overview">';
  $content .= '  <p>'.t('Welcome to your Total Control admin dashboard.  Please start customizing to make this dashboard the perfect hub for all your administrative needs.').'</p>';
  $content .= '</div>';

  $block->content = $content;
  return $block;
}
