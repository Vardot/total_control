<?php

/**
 * @file taxonomy.inc
 *
 * "Taxonomy" content type. It shows users with permissions statistics
 * and links to manage terms in vocabularies on the site.
 *
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Categories'),
  'defaults' => array('vids' => NULL),
  'icon' => 'cog.png',
  'description' => t('Displays Categories, number of terms in each category, "view terms" link and "edit terms" link.'),
  'category' => t('Total control'),
  'edit text' => t('Configure'),
);

/**
 * 'Admin title' callback for the content type.
 */
function total_control_taxonomy_content_type_admin_title($subtype = NULL, $conf = NULL, $context = NULL) {
  return t('Taxonomy overview');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_taxonomy_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Displays category information: number of terms in each vocabulary with links to see the list or add a new one.');

  return $block;
}

/**
 * Run-time rendering of the body of the block.
 */
function total_control_taxonomy_content_type_render($subtype, $conf, $panel_args, $context) {
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
        $term_count = db_query("SELECT count(*) FROM {taxonomy_term_data} WHERE vid = :vid", array(':vid' => $vocab->vid))->fetchField();

        $terms = format_plural($term_count, '1 term', '@count terms');
        $rows[$vocab->vid] = $vocab->name . ': ' . $terms;
        if (user_access('administer taxonomy')){
          $rows[$vocab->vid] .= ' | '. l('list terms', 'admin/content/taxonomy/'.$vocab->vid);
          $rows[$vocab->vid] .= ' | '. l('add term', 'admin/content/taxonomy/'.$vocab->vid.'/add/term');
        } // if access
      } // if vocab
    } // foreach
  }

  if ($rows){
    $output = theme('total_control_taxonomy', array('taxonomy' => $rows));
  }
  else {
    $output = t('You have no vocabularies yet. ');
    if (user_access('administer taxonomy')){
      $output .= '<div class="add-vocab">'.l(t('Add vocabulary'),'admin/content/taxonomy/add/vocabulary').'</div>';
    }
  }

  $block->content = $output;

  return $block;
}

/**
 * 'Edit form' callback for the content type.
 */
function total_control_taxonomy_content_type_edit_form($form, &$form_state) {
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
 */
function total_control_taxonomy_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  } 
}
