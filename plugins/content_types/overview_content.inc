<?php

/**
 * Implementation of specially named hook_ctools_content_types()
 */
function total_control_overview_content_ctools_content_types() {
  return array(
    'single' => TRUE,
    'icon' => 'icon_node_form.png',
    'no title override' => TRUE,
    'title' => t('Content Overview'),
    'description' => t('Displays a summary of content statistics including: number pieces of each type of content, number of comments, number of blocked comments (spam), and if ').l(t('allowed'), 'admin/settings/total_control').t(', content-type configuration links.'),
    'category' => t('Admin Dashboard'),
  );
}

function total_control_overview_content_content_type_admin_title($subtype, $conf, $context) {
  return t('Content Overview');
}

function total_control_overview_content_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Displays a summary of content statistics including: number pieces of each type of content, number of comments, number of blocked comments (spam), and if ').l(t('allowed'), 'admin/settings/total_control').t(', content-type configuration links.');
  return $block;
}

function total_control_overview_content_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('total_control');

  $types = node_get_types('types');  
  $overview = array();
    
  // Content Overview
  foreach ($types as $type => $object){
    if ($conf['types'][$type]){  // compare against type option on pane config
      $type_query = db_query("SELECT count(*) FROM {node} WHERE type = '%s' and status = 1", $type);
      $total[$type] = format_plural(db_result($type_query), '1 '.$type.' item', '@count '.$type.' items');
          
      if ($conf['comments'][$type]){ // compare against comment option on pane config
        $comment_query = db_query("SELECT count(DISTINCT cid) FROM {comments} c INNER JOIN {node} n ON c.nid = n.nid WHERE n.type = '%s' and c.status = 1 AND n.status = 1", $type);
        $total[$type.'_comments'] =  format_plural(db_result($comment_query), '1 comment', '@count comments');
        
        if ($conf['spam'] == 1){ // compare against comment option on pane config
          $spam_query = db_query("SELECT count(DISTINCT c.cid) FROM {comments} c INNER JOIN {node} n ON c.nid = n.nid WHERE n.type = '%s' and c.status = 0 AND n.status = 1", $type);
          $total[$type.'_comments_spam'] = format_plural(db_result($spam_query), '1 spam', '@count spam');
        } // if spam
      } // if comment
      
      $line =  $total[$type];
      $line .= ($total[$type.'_comments'])? ' with ' . $total[$type.'_comments'] : '' ;
      $line .= ($total[$type.'_comments_spam'])? ' (' . $total[$type.'_comments_spam'].')' : '' ;
      $overview[] = $line;
    } // if type
  } // foreach

  $pane = total_control_overview_content_ctools_content_types();
  
  // assemble content
  $content =  '<div class="total-control-content-overview">';
  $content .= '  <h2 class="title">' . $pane['title'] . '</h2>';
  $content .= '  <div class="content">';
  $content .=     theme('item_list', $overview);
  $content .= '  </div>';
  $content .= '</div>';

  $block->content = $content;
  return $block;
}

function total_control_overview_content_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  
  $types = node_get_types('types');
  $type_options = array();
  $type_defaults = array();
  $comment_defaults = array();
  foreach ($types as $type => $object){
    $type_options[$type] = $object->name;
    $type_defaults[] = $type;
    if ($type == 'blog' || $type == 'forum topic'){
      $comment_defaults[] = $type;
    }
  }
  $form['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Include Stats on Content Types'),
    '#options' => $type_options,
    '#default_value' => ($form_state['op'] == 'add')? $type_defaults : $conf['types'],
    );
  $form['comments'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Include Comment Stats for Content Types'),
    '#options' => $type_options,
    '#default_value' => ($form_state['op'] == 'add')? $comment_defaults : $conf['comments'],
    );
  $spam_options = array(
    0 => t('no'), 
    1 => t('Include Spam Comment count'));
  $form['spam'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include Spam Comment count'),
    '#options' => $spam_options,
    '#default_value' => ($form_state['op'] == 'add')? TRUE : $conf['spam'],
    );

  return $form;
}


/**
 * The submit form stores the data in $conf.
 */
function total_control_overview_content_content_type_edit_form_submit(&$form, &$form_state) {
  foreach ($form_state['values'] as $key => $data){
    $form_state['conf'][$key] = $form_state['values'][$key];
  } 
}
