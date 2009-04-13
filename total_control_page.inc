<?php

/* Cut and paste below */

$page = new stdClass;
$page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
$page->name = 'dashboard';
$page->admin_title = 'Total Control Dashboard';
$page->path = 'admin/dashboard';
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
  'type' => 'normal',
  'title' => 'Dashboard',
  'name' => 'navigation',
  'weight' => '0',
  'parent' => array(
    'type' => 'none',
    'title' => '',
    'name' => 'navigation',
    'weight' => '0',
  ),
);
$page->arguments = array();
$page->default_handlers = array();
$handler = new stdClass;
$handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
$handler->name = 'page_dashboard_panel_context';
$handler->task = 'page';
$handler->subtask = 'dashboard';
$handler->handler = 'panel_context';
$handler->weight = 0;
$handler->conf = array(
  'autogenerate_title' => TRUE,
  'title' => 'Panel: Logged in user must have "have total control"',
  'no_blocks' => FALSE,
  'css_id' => '',
  'css' => '',
  'css_cache_file' => '',
  'contexts' => array(),
  'relationships' => array(),
  'access' => array(
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
  ),
  'did' => '24',
  'css_cache' => '',
);
$display = new panels_display;
$display->layout = 'threecol_33_34_33';
$display->layout_settings = array();
$display->panel_settings = array();
$display->cache = array();
$display->title = '';
$display->hide_title = FALSE;
$display->content = array();
$display->panels = array();
$handler->conf['display'] = $display;
$page->default_handlers[$handler->name] = $handler;

/* cut and paste above */