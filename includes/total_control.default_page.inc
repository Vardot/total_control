<?php

/*  IMPORT CODE BELOW  */

$handler = new stdClass;
$handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
$handler->api_version = 1;
$handler->name = 'total_control_panel_context';
$handler->task = 'dashboard';
$handler->subtask = '';
$handler->handler = 'panel_context';
$handler->weight = 0;
$handler->conf = array(
  'title' => 'Total Control Admin Dashboard',
  'no_blocks' => FALSE,
  'css_id' => '',
  'css' => '',
  'contexts' => array(),
  'relationships' => array(),
);
$display = new panels_display;
$display->layout = 'twocol_stacked';
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
$pane->type = 'create';
$pane->subtype = 'create';
$pane->shown = TRUE;
$pane->access = array();
$pane->configuration = array(
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
$pane->type = 'taxonomy';
$pane->subtype = 'taxonomy';
$pane->shown = TRUE;
$pane->access = array();
$pane->configuration = array(
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
$pane->type = 'overview_content';
$pane->subtype = 'overview_content';
$pane->shown = TRUE;
$pane->access = array();
$pane->configuration = array(
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
$pane->panel = 'right';
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
$display->panels['right'][0] = 'new-4';
$pane = new stdClass;
$pane->pid = 'new-5';
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
$display->content['new-5'] = $pane;
$display->panels['right'][1] = 'new-5';
$pane = new stdClass;
$pane->pid = 'new-6';
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
$display->content['new-6'] = $pane;
$display->panels['top'][0] = 'new-6';
$handler->conf['display'] = $display;

/*  IMPORT CODE ABOVE  */

$handlers[$handler->name] = $handler;