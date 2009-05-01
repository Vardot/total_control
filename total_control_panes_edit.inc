<?php

/*
 * These functions Define Pane Config Form items
 */

function total_control_panels_edit_admin_overview(){
  $form = array();
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
    '#default_value' => TRUE,
    );
  $user_options = array(
    0 => t('no'), 
    1 => t('Show User Stats'));
  $form['user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include User Counts'),
    '#options' => $user_options,
    '#default_value' => TRUE,
    );
  $roles = user_roles(TRUE);
  foreach ($roles as $rid => $role){
    if ($rid != 2){
      $roles_options[$rid] = $role;
      $roles_defaults[] = $rid;
    }
  }
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Include User counts in Roles'),
    '#options' => $roles_options,
    '#default_value' => $roles_defaults,
    );
  return $form;
} 

function total_control_panels_edit_admin_users(){
  $form = array();
  $user_options = array(
    0 => t('no'), 
    1 => t('Show User Stats'));
  $form['user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include User Counts'),
    '#options' => $user_options,
    '#default_value' => TRUE,
    );
  $roles = user_roles(TRUE);
  $roles_options = array();
  $roles_defaults = array();
  foreach ($roles as $rid => $role){
    if ($rid != 2){
      $roles_options[$rid] = $role;
      $roles_defaults[] = $rid;
    }
  }
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Include User counts in Roles'),
    '#options' => $roles_options,
    '#default_value' => $roles_defaults,
    );
  return $form;
} 

function total_control_panels_edit_admin_content(){
  $form = array();
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
    '#default_value' => TRUE,
    );
  return $form;
} 


function total_control_panels_edit_admin_create(){
  $form = array();
  $types = node_get_types('types');
  $type_options = array();
  $type_defaults = array();
  foreach ($types as $type => $object){
    $type_options[$type] = $object->name;
    $type_defaults[] = $type;
  }
  $form['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Include Create links for Content Types'),
    '#options' => $type_options,
    '#default_value' => $type_defaults,
    );
  return $form;
}

function total_control_panels_edit_admin_tags() {
  $form = array();
  $vocabs = taxonomy_get_vocabularies();
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
    '#default_value' => $vocab_defaults,
    );
  return $form;
}