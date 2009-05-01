<?php

/*
 * These functions Return Pane Content
 */
 
// Admin Overview
function total_control_panels_render_admin_overview($something, $conf) {
  $item = total_control_panels_content_types_data('admin_overview');  
  $types = node_get_types('types');  
  $overview_cnt = array();
  $overview_usr = array();
    
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
      $overview_cnt[] = $line;
    } // if type
  } // foreach
  
  // User Overview
  if ($conf['user'] == 1){  // compare against user option on pane config
    $user_query = db_query("SELECT count(*) FROM {users}");
    $total['users_all'] = format_plural(db_result($user_query), '1 total user', '@count total users');
    $user_active_query = db_query("SELECT count(*) FROM {users} WHERE status = 1 AND login != 0");
    $total['users_active'] = format_plural(db_result($user_active_query), '1 active', '@count active');
    $user_block_query = db_query("SELECT count(*) FROM {users} WHERE status = 0");
    $total['users_block'] = format_plural(db_result($user_block_query), '1 blocked', '@count blocked');
    $overview_usr[] = $total['users_all'] . ' | ' . $total['users_active'] . ' | ' . $total['users_block'];
  }
  
  // Roles Overview
  $roles = user_roles(TRUE);
  $total['users_roles'] = '';
  foreach ($roles as $rid => $role){
    if (($conf['roles'][$rid]) && $rid != 2){  // compare against roles option on pane config
      $user_role_query = db_query("SELECT count(*) FROM {users} u INNER JOIN {users_roles} r on u.uid = r.uid WHERE r.rid = %d", $rid);
      $total['users_role_'.$rid] .= format_plural(db_result($user_role_query), '1 user', '@count users');
      $total['users_role_'.$rid] .= ' in role: '.$role;
      $overview_usr[] = $total['users_role_'.$rid];
    } // if not auth
  } // foreach
  
  $content = '<strong>'.t('Content').'</strong>';
  $content .= theme('item_list', $overview_cnt);
  $content .= '<strong>'.t('Users').'</strong>';
  $content .= theme('item_list', $overview_usr);
  return total_control_panels_render($item['title'], $content);
}

// User Overview
function total_control_panels_render_admin_users($something, $conf) {
  $item = total_control_panels_content_types_data('admin_users');  
  $types = node_get_types('types');  
  $overview = array();
  
  // User Overview
  if ($conf['user'] == 1){  // compare against user option on pane config
    $user_query = db_query("SELECT count(*) FROM {users}");
    $total['users_all'] = format_plural(db_result($user_query), '1 total user', '@count total users');
    $user_active_query = db_query("SELECT count(*) FROM {users} WHERE status = 1 AND login != 0");
    $total['users_active'] = format_plural(db_result($user_active_query), '1 active user', '@count active users');
    $user_block_query = db_query("SELECT count(*) FROM {users} WHERE status = 0");
    $total['users_block'] = format_plural(db_result($user_block_query), '1 blocked user', '@count blocked users');
    $overview[] = $total['users_all'];
    $overview[] = $total['users_active'];
    $overview[] = $total['users_block'];
  }
  
  // Roles Overview
  $roles = user_roles(TRUE);
  $total['users_roles'] = '';
  foreach ($roles as $rid => $role){
    if (($conf['roles'][$rid]) && $rid != 2){  // compare against roles option on pane config
      $user_role_query = db_query("SELECT count(*) FROM {users} u INNER JOIN {users_roles} r on u.uid = r.uid WHERE r.rid = %d", $rid);
      $total['users_role_'.$rid] .= format_plural(db_result($user_role_query), '1 user', '@count users');
      $total['users_role_'.$rid] .= ' in role: '.$role;
      $overview[] = $total['users_role_'.$rid];
    } // if not auth
  } // foreach
  
  $content = theme('item_list', $overview);
  return total_control_panels_render($item['title'], $content);
}

// Content Overview
function total_control_panels_render_admin_content($something, $conf) {
  $item = total_control_panels_content_types_data('admin_content');  
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
  
  $content = theme('item_list', $overview);
  return total_control_panels_render($item['title'], $content);
}

// Create Content
function total_control_panels_render_admin_create($something, $conf) {
  $item = total_control_panels_content_types_data('admin_create');
  $types = node_get_types('types');
  $create = array();  
  
  foreach ($types as $type => $object){
    if ($conf['types'][$type]){  // compare against type option on pane config
      $row = l(t('Add New '.$object->name),'node/add/'.$object->type);
      if (user_access('administer content types')){
        $row .= ' | ' . l(t('Configure'),'admin/content/node-type/'.$object->type);
      }
      $create[] = $row;
    } // end if type
  }
    
  $content = theme('item_list', $create);
  return total_control_panels_render($item['title'], $content);
}

// Manage Terms
function total_control_panels_render_admin_tags($something, $conf = array()) {
  $item = total_control_panels_content_types_data('admin_tags');
  $vocabs = taxonomy_get_vocabularies();
  $tags = array();  
    
  foreach ($vocabs as $vocab){
    if (in_array($vocab->vid, $conf['vids'])){  // compare against vocab option on pane config
      $term_query = db_query("SELECT count(*) FROM {term_data} WHERE vid = %d", $vocab->vid);
      $terms = format_plural(db_result($term_query), '1 tag', '@count tags');
      $tags[$vocab->vid] = $vocab->name . ': ' . $terms;
      if (user_access('administer taxonomy')){
        $tags[$vocab->vid] .= ' | '. l('list tags', 'admin/content/taxonomy/'.$vocab->vid);
        $tags[$vocab->vid] .= ' | '. l('add tag', 'admin/content/taxonomy/'.$vocab->vid.'/add/term');
      } // if access
    } // if vocab
  } // foreach
    
  $content = theme('item_list', $tags);
  return total_control_panels_render($item['title'], $content);
}