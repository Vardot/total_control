<?php

/* Cut and paste below */

$page = new stdClass;
$page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
$page->name = 'dashboard';
$page->admin_title = 'Admin Dashboard';
$page->path = 'admin/dashboard';
$page->access = array(
  'logic' => 'and',
  'plugins' => array(
    '0' => array(
      'name' => 'perm',
      'settings' => array(
        'perm' => 'have total control',
      ),
      'context' => 'logged-in-user',
    ),
  ),
);
$page->multiple = FALSE;
$page->menu = array(
  'type' => 'normal',
  'title' => 'Admin Dashboard',
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

/* cut and paste above */