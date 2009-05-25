<?php

/* Cut and paste below */

$page = new stdClass;
$page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
$page->name = 'dashboard';
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
    'weight' => '0',
  ),
);
$page->arguments = array();

/* cut and paste above */