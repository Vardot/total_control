<?php
// $Id: taxonomy.inc,v 1.11 2010/03/02 22:10:32 jenlampton Exp $

/**
 * @file
 * "Taxonomy" content type. It shows users with permissions statistics
 * and links to manage terms in vocabularies on the site.
 *
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Categories'),
  'description' => t('Displays Categories, number of terms in each category, "view terms" link and "edit terms" link.'),
  // 'single' => TRUE means has no subtypes.
  'single' => TRUE,
  // Constructor.
  'content_types' => array('total_control_taxonomy_content_type'),
  // Name of a function which will render the block.
  'render callback' => 'total_control_taxonomy_content_type_render',
  // The default context.
  'defaults' => array('vids' => NULL),
  // This explicitly declares the config form. Without this line, the func would be
  // total_control_taxonomy_content_type_edit_form.
  'edit form' => 'total_control_taxonomy_content_type_edit_form',
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
function total_control_taxonomy_content_type_admin_title($subtype = NULL, $conf = NULL, $context = NULL) {
    return t('Categories and Terms');
}

/**
 * 'Admin info' callback for the content type.
 *
 * ctools_plugin_example_total_control_panel_pages_content_type_admin_info.
 *
 */
function total_control_taxonomy_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Displays Categories, number of terms in each category, "view terms" link and "edit terms" link.');
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
function total_control_taxonomy_content_type_render($subtype, $conf, $panel_args, &$context) {
  if (!module_exists('taxonomy')){
    return;
  }
  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = total_control_taxonomy_content_type_admin_title();
  
  $vids = (isset($conf['vids']))? $conf['vids']: array();
  
  $vocabs = taxonomy_get_vocabularies();
  $rows = array();  
  
  if (!empty($vocabs)){  
    foreach ($vocabs as $vocab){
      // compare against vocab option on pane config
      if ((in_array($vocab->vid, $vids)) || !array_key_exists('vids',$conf)){  
        $term_query = db_query("SELECT count(*) FROM {term_data} WHERE vid = %d", $vocab->vid);
        $terms = format_plural(db_result($term_query), '1 term', '@count terms');
        $rows[$vocab->vid] = $vocab->name . ': ' . $terms;
        if (user_access('administer taxonomy')){
          $rows[$vocab->vid] .= ' | '. l('list terms', 'admin/content/taxonomy/'.$vocab->vid);
          $rows[$vocab->vid] .= ' | '. l('add term', 'admin/content/taxonomy/'.$vocab->vid.'/add/term');
        } // if access
      } // if vocab
    } // foreach
  }
  
  if ($rows){
    $output = theme('item_list', $rows);
  }
  else {
    $output = t('You have no vocabularies yet. ');
    if (user_access('administer taxonomy')){
      $output .= '<div class="add-vocab">'.l(t('Add vocabulary'),'admin/content/taxonomy/add/vocabulary').'</div>';
    }
  }
  
  // assemble content
  $content =  '<div class="total-control-content-overview">';
  $content .= $output;
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
function total_control_taxonomy_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
 
  $vocabs = taxonomy_get_vocabularies();
  if (!empty($vocabs)) {
    $vocab_options = array();
    $vocab_defaults = array();
    foreach ($vocabs as $vid => $vocab){
      $vocab_options[$vid] = $vocab->name;
      $vocab_defaults[$vid] = $vid;
    }
    
    if (isset($conf['vids'])){
      $vocab_defaults = $conf['vids'];
    }
  
    $form['vids'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Include Vocabularies'),
      '#multiple' => true,
      '#options' => $vocab_options,
      '#default_value' => $vocab_defaults,
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
function total_control_taxonomy_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  } 
}
