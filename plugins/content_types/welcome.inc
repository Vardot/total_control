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
  
  // Building the content
  $content =  '<div class="total-control-site-overview">';
  $content .= '  <h2 class="title">' . t('Congratulations, You are now in Total Control.') . '</h2>';
  $content .= '  <div class="content">';
  $content .= '    <p>'.t('Welcome to the total control admin dashboard.  Please review the tips below for how to make this dashboard the perfect hub for all your administration needs.').'</p>';
  $content .=      '<p>'.t('A few panes are provided here for you by default, but there are ').'<strong>'.t('other panes available').'</strong>'.t('.  Go to Admin > Site building > Pages.  Locate the "Total Control Admin Dashboard" in the list.  "Edit" that panel and click on "Content". On the panel content page, click the cog wheel at the top left of any panel and select "Add content" from the drop-down.  Your dashboard options are provided under "Total Control" on the left. Add as many or as few as you like, and make this dashboard your own!').'</p>';
  $content .= '<p>'.t('The tabs on this page are default views provided as part of Total Control.  I guessed at what you might like to see there, but feel free to ').'<strong>'.t('override each view').'</strong>'.t(' as necessary at Admin > Site Building > Views.').'</p>';
  $content .= '  </div>';
  $content .= '</div>';

  $block->content = $content;
  return $block;
}
