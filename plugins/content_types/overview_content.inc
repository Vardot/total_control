<?php
// $Id: overview_content.inc,v 1.12 2010/03/02 22:10:32 jenlampton Exp $

/**
 * @file
 * "Content overview" panels content type. Displays a summary of 
 * content statistics including: number pieces of each type of 
 * content, number of comments, number of blocked comments (spam)
 * and configuration links.
 *
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Content Overview'),
  'description' => t('Displays a summary of content statistics including: number pieces of each type of content, number of comments, number of blocked comments (spam), and if ').l(t('allowed'), 'admin/settings/total_control').t(', content-type configuration links.'),
  // 'single' => TRUE means has no subtypes.
  'single' => TRUE,
  // Constructor.
  'content_types' => array('total_control_overview_content_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'total_control_overview_content_content_type_render',
  // The default context.
  'defaults' => array('types' => NULL, 'comments' => NULL, 'spam' => 1),
  // This explicitly declares the config form. Without this line, the func would be
  // total_control_overview_content_content_type_edit_form.
  'edit form' => 'total_control_overview_content_content_type_edit_form',
  // Icon goes in the directory with the content type.
  'icon' => 'icon_node_form.png',
  'category' => t('Total Control'),
);

/**
 * 'Admin title' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_admin_title.
 *
 */
function total_control_overview_content_content_type_admin_title($subtype = NULL, $conf = NULL, $context = NULL) {
  return t('Content Overview');
}

/**
 * 'Admin info' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_admin_info.
 *
 */
function total_control_overview_content_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Displays a summary of content statistics including: number pieces of each type of content, number of comments, number of blocked comments (spam), and if ').l(t('allowed'), 'admin/settings/total_control').t(', content-type configuration links.');
  return $block;
}

/**
 * Run-time rendering of the body of the block.
 *
 * @param $subtype
 * @param $conf
 *   Configuration as done at admin time.
 * @param $args
 * @param $context
 *   Context - in this case we don't have any.
 *
 * @return
 *   An object with at least title and content members.
 */
function total_control_overview_content_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = total_control_overview_content_content_type_admin_title();

  $types = node_get_types('types');  
  $overview = array();
    
  // Content Overview
  foreach ($types as $type => $object){
    if (empty($conf['types']) || (isset($conf['types']) && $conf['types'][$type])){  // compare against type option on pane config
      $type_query = db_query("SELECT count(*) FROM {node} WHERE type = '%s' and status = 1", $type);
      $total[$type] = format_plural(db_result($type_query), '1 '.$object->name.' item', '@count '.$object->name.' items');
      
      $comment_setting = variable_get('comment_'. $type, 'comment');
      if ((module_exists('comment')) && (($comment_setting != 0) || (isset($conf['comments']) && $conf['comments'][$type]))){ // compare against comment option on pane config
        $comment_query = db_query("SELECT count(DISTINCT cid) FROM {comments} c INNER JOIN {node} n ON c.nid = n.nid WHERE n.type = '%s' and c.status = 1 AND n.status = 1", $type);
        $total[$type.'_comments'] =  format_plural(db_result($comment_query), '1 comment', '@count comments');
        
        if (isset($conf['spam']) && $conf['spam'] == 1){ // compare against comment option on pane config
          $spam_query = db_query("SELECT count(DISTINCT c.cid) FROM {comments} c INNER JOIN {node} n ON c.nid = n.nid WHERE n.type = '%s' and c.status = 0 AND n.status = 1", $type);
          $total[$type.'_comments_spam'] = format_plural(db_result($spam_query), '1 spam', '@count spam');
        } // if spam
      } // if comment
      
      $line =  $total[$type];
      $line .= (isset($total[$type.'_comments']))? ' with ' . $total[$type.'_comments'] : '' ;
      $line .= (isset($total[$type.'_comments_spam']))? ' (' . $total[$type.'_comments_spam'].')' : '' ;
      $overview[] = $line;
    } // if type
  } // foreach
  
  // assemble content
  $content =  '<div class="total-control-content-overview">';
  $content .= theme('item_list', $overview);
  $content .= '</div>';

  $block->content = $content;
  return $block;
}

/**
 * 'Edit form' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_edit_form.
 *
 */
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
    '#default_value' => $type_defaults,
    );
  if (module_exists('comments')){
    $form['comments'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Include Comment Stats for Content Types'),
      '#options' => $type_options,
      '#default_value' => $comment_defaults,
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
  }

  return $form;
}


/**
 * 'Edit form' submit callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_edit_form_submit.
 * The submit form stores the data in $conf.
 *
 */
function total_control_overview_content_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  } 
}
