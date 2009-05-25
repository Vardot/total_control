<?php

/* Cut and paste below */

$page = new stdClass;
$page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
$page->api_version = 1;
$page->name = 'dashboard';
$page->task = 'page';
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
$page->conf = FALSE;


/* cut and paste above */