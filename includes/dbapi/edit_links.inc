<?php
 define("EDIT_LINKS_INC", true);
/*
  edit_links.inc; 1.00
    
  Written by Pilif and Jody
  
  Created:  Apr, 10th 2000
 
  © 2000 by Linktrail. All rights reserved  
 */

 /*
   The following is common to all DBAPI-Units. The DB-Connection 
   will only be opened once
 */

 global $myDB;

 
 if (!is_object($myDB)) {
  $myDB = new DB_Linktrail;
  include("dbapi/sql_strs.inc");
  include("dbapi/sql_util.inc");
 }
 
 if (!defined("COMUTILS_INC"))
  include("dbapi/comutils.inc");
 if (!defined("TRAILS_INC"))
  include("dbapi/trails.inc");
 if (!defined("EDIT_DIRECTORY_INC"))
  include("dbapi/edit_directory.inc");
 if (!defined("TRAILFUNCS_INC"))
  include("commonapi/trailfuncs.inc");
 if (!defined("MESSAGES_INC"))
  include("dbapi/messages.inc");
 if (!defined("SUBSCRIPTIONS_INC"))
  include("dbapi/subscriptions.inc");

function edit_link($obj_id, $new_obj){
 global $myDB;
 
 foreach($new_obj as $key => $val){ $new_obj[$key] = cleanup_string($new_obj[$key]); };
 $old_obj = get_link_info($obj_id);
 
 if (!strstr($new_obj['url'], "//"))
  $new_obj['url'] = 'http://'.$new_obj['url'];

 if ($new_obj['title'] == "")
  $new_obj['title'] = ($old_obj['title'] == "") ? "" : $old_obj['title'];
 if ($new_obj['description'] == "")
  $new_obj['description'] = ($old_obj['description'] == "") ? "" : $old_obj['description'];
 if ($new_obj['url'] == "")
  $new_obj['url'] = ($old_obj['url'] == "") ? "" : $old_obj['url'];
 if ( ($new_obj['url'] != "") and ($new_obj['position'] != "") )
  $sql = FormatSQL(SQL_EDIT_LINK_WP, array($new_obj['title'], $new_obj['description'], $new_obj['url'], $new_obj['position'], $new_obj['owner'], $obj_id) );
 elseif ( ($new_obj['url'] == "") and ($new_obj['position'] != "") )
  $sql = FormatSQL(SQL_EDIT_LINK_OP, array($new_obj['position'], $obj_id) );
 elseif ( ($new_obj['url'] != "") and ($new_obj['position'] == "") )
  $sql = FormatSQL(SQL_EDIT_LINK, array($new_obj['title'], $new_obj['description'], $new_obj['url'], $new_obj['owner'], $obj_id) );
 
// die("<pre>$sql</pre>");
 update_dir_changedate($new_obj['trail']); 
 return $myDB->query($sql);
} 

  
function add_link($obj, $now=false){
 global $myDB;
 
 foreach($obj as $key => $val) $obj[$key] = cleanup_string($obj[$key]);
 if ( !strstr($obj['url'], "://"))
  $obj['url'] = 'http://'.$obj['url'];
 if (!$now){
  $query=FormatSQL(SQL_MD_L_I, array($obj['title'], 'add', 'Trail='.$obj['trail']."&Description=".$obj['description']."&Url=".$obj['url']."&Position=".$obj['position'], $obj['owner']));
  return $myDB->query($query);
 }else{
  if ($obj['position'] != "")
   $query = FormatSQL(SQL_INSLINKWP, array($obj['trail'], $obj['title'], $obj['description'], $obj['url'], $obj['position'], $obj['owner'], 'NOW()'));
  else
   $query = FormatSQL(SQL_INSLINK, array($obj['trail'], $obj['title'], $obj['description'], $obj['url'], $obj['owner'], 'NOW()'));  
  update_dir_changedate($obj['trail']); 
  return $myDB->query($query);
 }
}

function rm_link($obj, $now=false){
 global $myDB;
 
 if (is_array($obj))
  foreach($obj as $key => $val) $obj[$key] = cleanup_string($obj[$key]);
 if (!$now){
  $query=FormatSQL(SQL_MD_L_I, array($obj['title'], 'delete', 'Link_ID='.$obj['id'], $obj['owner']));
  return $myDB->query($query);
 }else{
  $query = FormatSQL(SQL_LINKDEL, array($obj['id']));
  update_dir_changedate($obj['trail']); 
  return $myDB->query($query);
 }
}

function rm_link_ex($obj){
 global $myDB;

 $current_links = get_links((integer)$obj['trail']);
 $object = new user_sorted_list($current_links);
 $object->delete_element($obj['id']);
 $sql_strs = $object->get_sql_diff($current_links);
 lock_table('ltrLinks');
 foreach($sql_strs as $sql)
  $myDB->query($sql);

 
 $query = FormatSQL(SQL_LINKDEL, array($obj['id']));
 $myDB->query($query);
 unlock_table('ltrLinks');
 update_dir_changedate($obj['trail']); 
 return true;
}


function add_link_ex($obj){
 global $myDB;
 if (is_array($obj))
  foreach($obj as $key => $val) $obj[$key] = cleanup_string($obj[$key]);
 
 if ( !strstr($obj['url'], "://"))
  $obj['url'] = 'http://'.$obj['url'];

 $current_links = get_links((integer)$obj['trail']);
 $object = new user_sorted_list($current_links);
 
// lock_table('ltrLinks');
 $query = FormatSQL(SQL_INSLINK_EX, array($obj['trail'], $obj['title'], $obj['description'],
                                          $obj['url'], $obj['owner']));
 //die("<pre>$query</pre>");
 $myDB->query($query);
 $id = mysql_insert_id();
 $obj['id'] = $id;
 $object->add_element($obj);
 $sql_strs = $object->get_sql_diff($current_links);

 foreach($sql_strs as $sql)
  $myDB->query("$sql");
// unlock_table('ltrLinks');
 
 update_dir_changedate($obj['trail']); 
 return true;
}

function edit_link_ex($id, $obj){
 global $myDB;
 
 if (is_array($obj))
  foreach($obj as $key => $val) $obj[$key] = cleanup_string($obj[$key]);


 $query = FormatSQL(SQL_EDIT_LINK_EX, array($obj['title'], $obj['description'],
                                          $obj['url'], $obj['owner'], $id));
 $myDB->query($query);
 //echo("<pre>$query</pre>");
 update_dir_changedate($obj['trail']); 
 return true;
}

function reposition_link($obj, $newpos){
 global $myDB;
// die($newpos);
// lock_table('ltrLinks WRITE, auth_user READ, ltrDirectory READ, ltrDirectory AS lookup');
// echo("Test");
 $current_links = get_links((integer)$obj['trail']);
 $linkid = $obj['id'];
 $object = new user_sorted_list($current_links);
 /*foreach($object->arr as $element)
  printf('<tt><font color="teal">(%s) ich bin %s;'."\t".'nach mir kommt: %s</font></tt><br>', $element['title'], $element['id'], $element['next']);
 echo("<p>"); 
 echo("verschiebe $linkid nach $newpos...");
 echo("<p>"); */
  $object->reposition($linkid, $newpos);
/* foreach($object->arr as $element)
  printf('<tt><font color="red">(%s) bin %s;'."\t".'nach mir kommt: %s</font></tt><br>', $element['title'], $element['id'], $element['next']);
 echo("<p>"); */
 $object->sort_array();
 /*foreach($object->arr as $element)
  printf('<tt><font color="blue">(%s) ich bin %s;'."\t".'nach mir kommt: %s</font></tt><br>', $element['title'], $element['id'], $element['next']);
 echo("<p>"); */
 $sql_strs = $object->get_sql_diff($current_links);
 foreach($sql_strs as $sql){
  // echo("query: <tt>$sql</tt><br>");
  $myDB->query($sql);
 }
//die("Ende des Tests. Nicht wurde ver„ndert"); 
// unlock_table();

}