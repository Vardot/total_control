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
  $block->title = t('Take Total Control.');
  
  // Building the content
  $content =  '<div class="total-control-site-overview">';
  $content .= '  <p>'.t('Welcome to your administrative dashboard.  !panel to add more content panes here, or configure those provided by default. Views content panes can be created for you automatically according to your !settings.  Make this dashboard the perfect hub for all your administrative needs.', array('!panel' => l(t('Edit this panel'), 'admin/build/pages/edit/dashboard'), '!settings' => l(t('Total Control Settings'), 'admin/settings/control'))).'</p>';
  $content .= '</div>';

  $block->content = $content;
  return $block;
}
