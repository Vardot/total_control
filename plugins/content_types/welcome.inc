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
  $content .= '    <p>'.t('Welcome to the total control admin dashboard.  Please review the tips below for how to make this dashboard the perfect hub for all your administration needs.').'</p>';
  $content .=      '<p>'.t('A few panes are provided here for you by default, but there are ').'<strong>'.t('other panes available').'</strong>'.t('.  Go to Admin > Site building > Pages.  Locate the "Admin Dashboard" in the list.  Under "Operations" at right, click "Panel Content". On the panel layout page, click the plus sign at the top left of any panel and select "Add content" from the drop-down.  Your Total Control options are provided under "Admin Dashboard" on the left. Add as many or as few as you like, and make this dashboard your own!').'</p>';
    $content .=      '<p>'.t('You can also ').'<strong>'.t('change information shown').'</strong>'.t(' on many of these panes. You can exclude a specific content type or vocabulary from the lists provided by changing the settings for each pane. Go to Admin > Site building > Pages.  Locate the "Admin Dashboard" in the list.  Under "Operations" at right, click "Panel Content".  Of the panes already added, click on the cog-wheel at the top right, and select "Settings" from the drop-down.  Customize as you see fit. ').'</p>';
  $content .= '<p>'.t('The tabs on this page are default views provided as part of Total Control.  I guessed at what you might like to see there, but please ').'<strong>'.t('override each view').'</strong>'.t(' as necessary at Admin > Site Building > Views.').'</p>';
  $content .= '<p>'.t('If you want something else here, or think your overrides should be included by default, please ').l(t('post an issue'),'http://drupal.org/project/issues/total_control', array('absolute' => TRUE, 'target'=>'_blank')).t(' and let me know.').'</p>';
  $content .= '  </div>';
  $content .= '</div>';

  $block->content = $content;
  return $block;
}
