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
    'category' => t('Admin Dashboard'),
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
  
  // Building the content
  $content =  '<div class="total-control-site-overview">';
  $content .= '  <h2 class="title">' . t('Congratulations, You are now in Total Control!') . '</h2>';
  $content .= '  <div class="content">';
  $content .=      '<p>'.t('Welcome to the total control admin dashboard.  Some admin panes are provided here for you by default, but more are available!  Go to Admin > Site building > Pages.  Locate the "Total Control Dashboard" on the page.  Under "Operations" at right, click "Panel Content" to check out your options. Make this dashboard your own!').'</p>';
  $content .= '  </div>';
  $content .= '</div>';

  $block->content = $content;
  return $block;
}
