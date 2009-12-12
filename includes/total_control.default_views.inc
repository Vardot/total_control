<?php

/**
 * Implementation of hook_views_default_views().
 */
function total_control_views_default_views() {
  $files = file_scan_directory(drupal_get_path('module', 'total_control'). '/views', '.view');
  foreach ($files as $filepath => $file) {
    require $filepath;
    
    // test for statistics module
    if (   ($file->name == 'control_activity' && !module_exists('statistics')) 
        || ($file->name == 'control_comments' && !module_exists('comment'))
        || ($file->name == 'control_terms' && !module_exists('taxonomy')) ){
      drupal_set_message(t('!feature features are being excluded because some needed modules are not enabled.'), array('!feature' => substr($file->name,8)));
    }
    else {
      if (isset($view)) {
        $views[$view->name] = $view;
      }
    }
    
  }
  return $views;
}

