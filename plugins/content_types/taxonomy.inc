<?php

/**
 * Implementation of specially named hook_ctools_content_types()
 */
function total_control_taxonomy_ctools_content_types() {
  return array(
    'single' => TRUE,
    'icon' => 'icon_node_form.png',
    'no title override' => TRUE,
    'title' => t('Categories and Terms'),
    'description' => t('Displays Categories, number of terms in each category, "view terms" link and "edit terms" link.'),
    'category' => t('Total Control'),
    'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
    'defaults' => array('vids' => NULL),
  ); 
}

function total_control_taxonomy_content_type_admin_title($subtype, $conf, $context) {
    return t('Categories and Terms');
}

function total_control_taxonomy_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Displays Categories, number of terms in each category, "view terms" link and "edit terms" link.');
  return $block;
}

function total_control_taxonomy_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('total_control');
  
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
  
  $pane = total_control_taxonomy_ctools_content_types();
  
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
  // For each part of the form defined in the 'defaults' array set when you
  // defined the content type, copy the value from the form into the array
  // of items to be saved. We don't ever want to use
  // $form_state['conf'] = $form_state['values'] because values contains
  // buttons, form id and other items we don't want stored. CTools will handle
  // the actual form submission.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  } 
}
