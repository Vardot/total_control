<?php

/* Cut and paste below */

$page = new stdClass;
$page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
$page->api_version = 1;
$page->name = 'dashboard';
$page->task = 'panel_page';
$page->admin_title = 'Total Control Dashboard';
$page->path = 'admin/dashboard/main';
$page->access = array(
  'plugins' => array(
    '0' => array(
      'name' => 'perm',
      'settings' => array(
        'perm' => 'have total control',
      ),
      'context' => 'logged-in-user',
    ),
  ),
  'logic' => 'and',
);
$page->multiple = FALSE;
$page->menu = array(
  'type' => 'default tab',
  'title' => 'Dashboard',
  'name' => 'navigation',
  'weight' => '-50',
  'parent' => array(
    'type' => 'normal',
    'title' => 'Dashboard',
    'name' => 'navigation',
    'weight' => '50',
  ),
);
$page->arguments = array();
$page->conf = array(
  'no_blocks' => 1,
  'css_id' => '',
  'css' => '',
);
$display = new panels_display;
$display->layout = 'threecol_33_34_33_stacked';
$display->layout_settings = array();
$display->panel_settings = array();
$display->cache = array();
$display->title = '';
$display->hide_title = FALSE;
$display->content = array();
$display->panels = array();
  $pane = new stdClass;
  $pane->pid = 'new-1';
  $pane->panel = 'left';
  $pane->type = 'overview_content';
  $pane->subtype = 'overview_content';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'op' => 'Finish',
    'return' => 'Finish',
    'types' => array(
      'page' => 'page',
      'story' => 'story',
    ),
    'comments' => array(
      'page' => 0,
      'story' => 0,
    ),
    'spam' => 1,
    'form_build_id' => 'form-5bb226a1fd1597365a69218de7de24ea',
    'form_token' => 'a60f57a6f43735af56d0ae26511e9385',
    'form_id' => 'total_control_overview_content_content_type_edit_form',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-1'] = $pane;
  $display->panels['left'][0] = 'new-1';
  $pane = new stdClass;
  $pane->pid = 'new-2';
  $pane->panel = 'left';
  $pane->type = 'create';
  $pane->subtype = 'create';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'op' => 'Finish',
    'return' => 'Finish',
    'types' => array(
      'page' => 'page',
      'story' => 'story',
    ),
    'form_build_id' => 'form-552ef1135296bc07546460d868fca156',
    'form_token' => 'd9c54d1e870b06b3268c11a1f3191a81',
    'form_id' => 'total_control_create_content_type_edit_form',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $display->content['new-2'] = $pane;
  $display->panels['left'][1] = 'new-2';
  $pane = new stdClass;
  $pane->pid = 'new-3';
  $pane->panel = 'left';
  $pane->type = 'taxonomy';
  $pane->subtype = 'taxonomy';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'op' => 'Finish',
    'return' => 'Finish',
    'form_build_id' => 'form-4774df4dfa948203b28a8f4ea42f390b',
    'form_token' => 'b0b2398e34dd11e194d41a7d2c7ec8ac',
    'form_id' => 'total_control_taxonomy_content_type_edit_form',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $display->content['new-3'] = $pane;
  $display->panels['left'][2] = 'new-3';
  $pane = new stdClass;
  $pane->pid = 'new-4';
  $pane->panel = 'middle';
  $pane->type = 'views_panes';
  $pane->subtype = 'control_content-panel_pane_1';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-4'] = $pane;
  $display->panels['middle'][0] = 'new-4';
  $pane = new stdClass;
  $pane->pid = 'new-5';
  $pane->panel = 'middle';
  $pane->type = 'views_panes';
  $pane->subtype = 'control_content-panel_pane_2';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $display->content['new-5'] = $pane;
  $display->panels['middle'][1] = 'new-5';
  $pane = new stdClass;
  $pane->pid = 'new-6';
  $pane->panel = 'middle';
  $pane->type = 'views_panes';
  $pane->subtype = 'control_comments-panel_pane_1';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $display->content['new-6'] = $pane;
  $display->panels['middle'][2] = 'new-6';
  $pane = new stdClass;
  $pane->pid = 'new-7';
  $pane->panel = 'right';
  $pane->type = 'overview_user';
  $pane->subtype = 'overview_user';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'op' => 'Finish',
    'return' => 'Finish',
    'user' => 1,
    'form_build_id' => 'form-ee14a2700830a04df70708b1ab02eaa5',
    'form_token' => '8da5fd3b86f35ce682fff5c1663b5eaf',
    'form_id' => 'total_control_overview_user_content_type_edit_form',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-7'] = $pane;
  $display->panels['right'][0] = 'new-7';
  $pane = new stdClass;
  $pane->pid = 'new-8';
  $pane->panel = 'right';
  $pane->type = 'views_panes';
  $pane->subtype = 'control_users-panel_pane_1';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $display->content['new-8'] = $pane;
  $display->panels['right'][1] = 'new-8';
  $pane = new stdClass;
  $pane->pid = 'new-9';
  $pane->panel = 'top';
  $pane->type = 'welcome';
  $pane->subtype = 'welcome';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array();
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-9'] = $pane;
  $display->panels['top'][0] = 'new-9';
$page->conf['display'] = $display;


/* cut and paste above */