<?php

/**
* Implementation of hook_views_default_views().
**/
function total_control_workflow_views_default_views() {
  $files = file_scan_directory(drupal_get_path('module', 'total_control'). '/views_workflow', '.view');
  foreach ($files as $filepath => $file) {
    require $filepath;
    if (isset($view)) {
      $views[$view->name] = $view;
    }
  }
  return $views;
}