<?php

/**
 * @file overview_content.inc
 *
 * "Content overview" panels content type. Displays a summary of content 
 * statistics including: number pieces of each type of content, number of
 * comments, number of blocked comments (spam) and configuration links.
 *
 */
 
$plugin = array(
  'single' => TRUE,
  'title' => t('Content overview'),
  'no title override' => TRUE,
  'defaults' => array('types' => array(), 'comments' => array(), 'spam' => 1),
  'icon' => 'cog.png',
  'description' => t('Displays a summary of content statistics including: 
    number pieces of each type of content, number of comments, number of 
    blocked comments (spam), and if ').l(t('allowed'), 
    'admin/settings/total_control').t(', content-type configuration links.'),
  'category' => t('Total control'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_overview_content_content_type_admin_title($subtype = NULL, $conf = NULL, $context = NULL) {
  return t('Content overview');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_overview_content_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Displays a summary of content statistics including: 
    number pieces of each type of content, number of comments, number of 
    blocked comments (spam), and if ').l(t('allowed'), 
    'admin/config/control') . t(', content-type configuration links.');

  return $block;
}

/**
 * Run-time rendering of the body of the block.
 */
function total_control_overview_content_content_type_render($subtype, $conf, $panel_args, $context) {
  $types = node_type_get_types();  
  $overview = array();

  foreach ($types as $type => $object){
    if (empty($conf['types']) || (isset($conf['types']) && $conf['types'][$type])){  // compare against type option on pane config
      $type_count = db_query("SELECT count(*) FROM {node} WHERE type = :type and status = 1", array(':type' => $type))->fetchField();
      $total[$type] = format_plural($type_count, '1 ' . $object->name . ' item', '@count ' . $object->name . ' items');

      // compare against comment option on pane config.
      $comment_setting = variable_get('comment_' . $type, 'comment');

      if ((module_exists('comment')) && (($comment_setting != 0) || (isset($conf['comments']) && $conf['comments'][$type]))){
        $comment_count = db_query("SELECT count(DISTINCT cid) FROM {comment} c INNER JOIN {node} n ON c.nid = n.nid WHERE n.type = :type and c.status = 1 AND n.status = 1", array(':type' => $type))->fetchField();
        $total[$type . '_comments'] =  format_plural($comment_count, '1 comment', '@count comments');

        if (isset($conf['spam']) && $conf['spam'] == 1){ // compare against comment option on pane config
          $spam_count = db_query("SELECT count(DISTINCT c.cid) FROM {comment} c INNER JOIN {node} n ON c.nid = n.nid WHERE n.type = :type and c.status = 0 AND n.status = 1", array(':type' => $type))->fetchField();
          $total[$type.'_comments_spam'] = format_plural($spam_count, '1 spam', '@count spam');
        }
      }

      $line =  $total[$type];
      $line .= (isset($total[$type.'_comments']))? ' with ' . $total[$type.'_comments'] : '' ;
      $line .= (isset($total[$type.'_comments_spam']))? ' (' . $total[$type.'_comments_spam'].')' : '' ;
      $overview[] = $line;
    }
  }

  $block = new stdClass();
  $block->title = total_control_overview_content_content_type_admin_title();
  $block->content = theme('total_control_overview_content', array('overview' => $overview));

  return $block;
}

/**
 * 'Edit form' callback for the content type.
 */
function total_control_overview_content_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $types = node_type_get_types();
  $type_options = array();
  $type_defaults = array();
  $comment_defaults = array();

  foreach ($types as $type => $object) {
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
    '#default_value' => ($conf['types'])? $conf['types'] : $type_defaults,
  );

  if (module_exists('comment')) {
    $form['comments'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Include Comment Stats for Content Types'),
      '#options' => $type_options,
      '#default_value' => ($conf['comments'])? $conf['comments'] : $comment_defaults,
    );
    $spam_options = array(
      0 => t('no'), 
      1 => t('Include Spam Comment count')
    );
    $form['spam'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include Spam Comment count'),
      '#options' => $spam_options,
      '#default_value' => ($form_state['op'] == 'add')? 1 : $conf['spam'],
    );
  }

  return $form;
}

/**
 * 'Edit form' submit callback for the content type.
 */
function total_control_overview_content_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  } 
}
