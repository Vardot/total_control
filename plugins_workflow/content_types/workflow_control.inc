<?php

/**
 * Implementation of specially named hook_ctools_content_types()
 */
function total_control_workflow_workflow_control_ctools_content_types() {
  return array(
    'single' => TRUE,
    'icon' => 'icon_node_form.png',
    'no title override' => TRUE,
    'title' => t('Workflow Control'),
    'description' => t('Displays workflow information and links, and workflow state information.'),
    'category' => t('Admin Dashboard'),
  ); 
}

function total_control_workflow_workflow_control_content_type_admin_title($subtype, $conf, $context) {
    return t('Workflow Control');
}

function total_control_workflow_workflow_control_content_type_admin_info($subtype, $conf, $context) {
  $block = new stdClass();
  $block->title = t('Displays workflow and state information.');
  return $block;
}

function total_control_workflow_workflow_control_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = t('total_control_workflow');
  
  $workflows = workflow_get_all();
  $rows = array();
  
  foreach ($workflows as $wid => $name) {
    if (in_array($wid, $conf['wids'])){  // compare against vocab option on pane config
      $add_state = l(t('Add state'),"admin/build/workflow/state/$wid");
      $actions = l(t('Actions'), "admin/build/trigger/workflow/$wid");
      $edit = l(t('Edit'), "admin/build/workflow/edit/$wid");
      $delete = l(t('Delete'), "admin/build/workflow/delete/$wid");
      
      $row = '<div class="workflow"><strong>'.$name.'</strong></div>';
      $row .= '<div class="workflow-links">'.$add_state.' | '.$actions.' | '.$edit.' | '.$delete.'</div>';
      
      $states = workflow_get_states($wid);
      
      foreach ($states as $sid => $state_name) {
        if ($state_name != '(creation)'){
          if (!workflow_is_system_state(t($state_name))) {
            $state_edit = l(t('Edit'), "admin/build/workflow/state/$wid/$sid");
            $state_delete = l(t('Delete'), "admin/build/workflow/state/delete/$wid/$sid");
          }
          $row .= '<div class="state-info"><span class="state">'.$state_name.':</span> '.$state_edit.' | '.$state_delete.'</div>';
        }
      }
      $rows[] = $row;
    }
  }
  
  $pane = total_control_workflow_workflow_control_ctools_content_types();
  
  // assemble content
  $content =  '<div class="total-control-content-overview">';
  $content .= '  <h2 class="title">' . $pane['title'] . '</h2>';
  $content .= '  <div class="content">';
  $content .=      theme('item_list', $rows);
  $content .= '  </div>';
  $content .= '</div>';

  $block->content = $content;
  return $block;
}

function total_control_workflow_workflow_control_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  
  $workflows = workflow_get_all();
  $workflow_options = array();
  $workflow_defaults = array();
  foreach ($workflows as $wid => $name){
    $workflow_options[$wid] = $name;
    $workflow_defaults[] = $wid;
  }
 
  $form['wids'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Include Workflows'),
    '#multiple' => true,
    '#options' => $workflow_options,
    '#default_value' => ($form_state['op'] == 'add')? $workflow_defaults : $conf['wids'],
    );
    /* TODO get states
    $form['states'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Include Workflows'),
      '#multiple' => true,
      '#options' => $state_options,
      '#default_value' => ($form_state['op'] == 'add')? $state_defaults : $conf['states'],
      );
      */
  
  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function total_control_workflow_workflow_control_content_type_edit_form_submit(&$form, &$form_state) {
  foreach ($form_state['values'] as $key => $data){
    $form_state['conf'][$key] = $form_state['values'][$key];
  } 
}
