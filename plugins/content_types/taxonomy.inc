<?php

/**
 * Implementation of specially named hook_ctools_content_types()
 */
function total_control_taxonomy_ctools_content_types() {
  return array(
    'single' => TRUE,
    'icon' => 'icon_node_form.png',
    'no title override' => TRUE,
    'title' => t('Category Control'),
    'description' => t('Displays Categories, number of tags in each category, "view terms" link and "edit terms" link.'),
    'category' => t('Admin Dashboard'),
  ); 
}

function total_control_taxonomy_content_type_admin_title($subtype, $conf, $context) {
    return t('Category Control');
}

function total_control_taxonomy_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Displays Categories, number of tags in each category, "view terms" link and "edit terms" link.');
  return $block;
}

function total_control_taxonomy_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('total_control');
  
  $vids = ($conf['vids'])? $conf['vids']: array();
  
  $vocabs = taxonomy_get_vocabularies();
  $tags = array();  
  
  if (!empty($vocabs)){  
    foreach ($vocabs as $vocab){
      // compare against vocab option on pane config
      if ((in_array($vocab->vid, $vids)) || !array_key_exists('vids',$conf)){  
        $term_query = db_query("SELECT count(*) FROM {term_data} WHERE vid = %d", $vocab->vid);
        $terms = format_plural(db_result($term_query), '1 tag', '@count tags');
        $tags[$vocab->vid] = $vocab->name . ': ' . $terms;
        if (user_access('administer taxonomy')){
          $tags[$vocab->vid] .= ' | '. l('list tags', 'admin/content/taxonomy/'.$vocab->vid);
          $tags[$vocab->vid] .= ' | '. l('add tag', 'admin/content/taxonomy/'.$vocab->vid.'/add/term');
        } // if access
      } // if vocab
    } // foreach
  }
  
  $pane = total_control_taxonomy_ctools_content_types();
  
  if ($tags){
    $output = theme('item_list', $tags);
  }
  else {
    $output = t('You have no vocabularies yet. ');
    if (user_access('administer taxonomy')){
      $output .= '<div class="add-vocab">'.l(t('Add vocabulary'),'admin/content/taxonomy/add/vocabulary').'</div>';
    }
  }
  
  // assemble content
  $content =  '<div class="total-control-content-overview">';
  $content .= '  <h2 class="title">' . $pane['title'] . '</h2>';
  $content .= '  <div class="content">';
  $content .=      $output;
  $content .= '  </div>';
  $content .= '</div>';

  $block->content = $content;
  return $block;
}

function total_control_taxonomy_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
 
  $vocabs = taxonomy_get_vocabularies();
  if (!empty($vocabs)) {
    $vocab_options = array();
    $vocab_defaults = array();
    foreach ($vocabs as $vid => $vocab){
      $vocab_options[$vid] = $vocab->name;
      $vocab_defaults[] = $vid;
    }
  
    $form['vids'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Include Vocabularies'),
      '#multiple' => true,
      '#options' => $vocab_options,
      '#default_value' => ($form_state['op'] == 'add')? $vocab_defaults : $conf['vids'],
      );
  }
  
  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function total_control_taxonomy_content_type_edit_form_submit(&$form, &$form_state) {
  foreach ($form_state['values'] as $key => $data){
    $form_state['conf'][$key] = $form_state['values'][$key];
  } 
}
