<?php

/**
 * @file taxonomy.inc
 *
 * "Taxonomy" content type. It shows users with permissions statistics
 * and links to manage terms in vocabularies on the site.
 *
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Admin - Taxonomy'),
  'description' => t('Provides links to administer taxonomy.'),
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
 */
function total_control_taxonomy_content_type_admin_title($subtype = NULL, $conf = NULL, $context = NULL) {
  return t('Administer Taxonomy');
}

/**
 * 'Admin info' callback for the content type.
 */
function total_control_taxonomy_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Provides links to administer taxonomy.');

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
  if (!module_exists('taxonomy')) {
    return;
  }

  $vids = (isset($conf['vids']))? $conf['vids']: array();

  $vocabs = taxonomy_get_vocabularies();
  $access = user_access('administer taxonomy');
  $options = array('query' => array('destination' => 'admin/dashboard'));
  $header = array('page' => t('Vocabulary'), 'options' => array('data' => t('Operations'), 'colspan' => 2));
  $rows = array();  

  if (!empty($vocabs)) {  
    foreach ($vocabs as $vocab) {
      // compare against vocab option on pane config
      if ((in_array($vocab->vid, $vids)) || !array_key_exists('vids', $conf)) {  
        $term_query = db_query("SELECT count(*) FROM {term_data} WHERE vid = %d", $vocab->vid);
        $terms = format_plural(db_result($term_query), '1 term', '@count terms');
        if ($access) {
          $rows[] = array(
            'data' => array(
              t($vocab->name . ': ' . $terms),
              l('List terms', 'admin/content/taxonomy/' . $vocab->vid, $options),
              l('Add terms', 'admin/content/taxonomy/' . $vocab->vid . '/add/term', $options),
            ),
          );
        }
      }
    }
  }

  if ($access) {
    $link = l(t('Taxonomy administration'), 'admin/content/taxonomy');
  }

  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => t('There are no vocabularies to display.'),
        'colspan' => 3,
      )
    );
  }

  $block = new stdClass();
  $block->module = t('total_control');
  $block->title = t('Administer Taxonomy');
  $block->content = theme('total_control_admin_table', $header, $rows, $link);

  return $block;
}

/**
 * 'Edit form' callback for the content type.
 */
function total_control_taxonomy_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  $vocabs = taxonomy_get_vocabularies();
  if (!empty($vocabs)) {
    $vocab_options = array();
    $vocab_defaults = array();
    foreach ($vocabs as $vid => $vocab) {
      $vocab_options[$vid] = $vocab->name;
      $vocab_defaults[$vid] = $vid;
    }

    if (isset($conf['vids'])) {
      $vocab_defaults = $conf['vids'];
    }

    $form['vids'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Include Vocabularies'),
      '#multiple' => TRUE,
      '#options' => $vocab_options,
      '#default_value' => $vocab_defaults,
      );
  }

  return $form;
}

/**
 * 'Edit form' submit callback for the content type.
 */
function total_control_taxonomy_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  } 
}
