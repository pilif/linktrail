<?php
 define("EDIT_DIRECTORY_INC", true);
/*
  edit_directory.inc; 1.00
  
  Contains DataBase-Functions used to build the directory-tree
  Documentation is inline and in directory.inc.html when ready
  
  Written by Pilif and Jody
  
  Created:  Mar, 21th 2000
 
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

 if (!defined("SCORING_INC"))
  include("dbapi/scoring.inc");

function update_targets($oldname, $newname){
 global $myDB;
 
 $query = FormatSQL(SQL_UPDATE_TARGETS, array(addslashes($newname), addslashes($oldname)));
 $myDB->query($query);
// die("<pre>$query</pre>");
}  
  
function create_symlink($newname, $linksto){
 global $myDB, $glob_language;
 $level = level_count($newname);
 $parent = path2id(parent_path($newname));
 $query = FormatSQL(SQL_CREATE_SYMLINK, array($newname, $linksto, $parent, $level));
 $myDB->query($query);
 remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path($newname)).'.'.$glob_language.'.html');
 remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path(parent_path($newname))).'.'.$glob_language.'.html');

 update_dir_changedate($parent);
 return true;
} 

function rename_symlink($id, $newpath){
 global $myDB, $glob_language;
 
 $query = FormatSQL(SQL_RENAME_SYMLINK, array($newpath, $id));
// echo("<pre>$query</pre>");
 $myDB->query($query);
 update_dir_changedate(path2id(parent_path($newpath)));
 remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path($newpath)).'.'.$glob_language.'.html');
 remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path(parent_path($newpath))).'.'.$glob_language.'.html');
 update_dir_changedate($id);
}

function delete_symlink($id){
 global $myDB, $glob_language;

 $query = FormatSQL(SQL_RM_D1, array($id));
 $name = id2path($id);
 $myDB->query($query);

  remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path($name)).'.'.$glob_language.'.html');
  remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path(parent_path($name))).'.'.$glob_language.'.html');
 update_dir_changedate($id);
} 
//Some events that are connected to some Node will not alter it's ChangeDate  
//namely: creating/adding/editing childs. The following function updates the
//date:

function update_dir_changedate($node_id){
 global $myDB;
 if ($node_id == "") return;
 $query = FormatSQL(SQL_UPDATE_DIR_CHANGE_DATE, array($node_id));
 $myDB->query($query);
}  
  
function sub_rename($oldname, $newname){
 global $myDB, $glob_language;
 
 $query = FormatSQL(SQL_GET_CHILDREN_RENAME, array(addslashes($oldname), $glob_language));
 $myDB->query($query);
 $x = 0;
 while ($myDB->next_record()){
  $ids[$x]['id']   = $myDB->f("Node_ID");
  $ids[$x]['path'] = $myDB->f("Name");
  $x++;
 }
 if (is_array($ids)){
  foreach($ids as $id){
   $newn     = str_replace($oldname, $newname, $id['path']);
   $newlevel = level_count($newn);
   $query = FormatSQL(SQL_UPDATE_CHILD, array(addslashes($newn), $newlevel, $id['id']));
   $myDB->query($query);
   update_dir_changedate(path2id(addslashes($oldname))); 
   update_dir_changedate(path2id(addslashes($newn))); 
   remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path($newn)).'.'.$glob_language.'.html');
   remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path(parent_path($newn))).'.'.$glob_language.'.html');
   remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path($oldname)).'.'.$glob_language.'.html');
   remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path(parent_path($oldname))).'.'.$glob_language.'.html');
   if (!is_trail($oldname))
    update_targets($oldname, $newn);
  }
 }
 
 return true;
}

/*function get_usermod(){
 global $myDB;
 
 $query = SQL_DUSERC; //no formatSQL since we have no where-clause
 $myDB->query($query);
 if ($myDB->num_rows() <= 0) 
  return -1; //no changes
 $idx = 0;
 while ($myDB->next_record()){
  $tmphash['type'] = $myDB->f("Type"); 
  parse_str($myDB->f('ChangeString')); //this will create global vars...
  switch ($tmphash['type']) {
     case 'add':
         $tmphash['name']         = $myDB->f('Object_Name');
         $tmphash['parent']       = $parent;
         $tmphash['level']        = $level;
         $tmphash['objecttype']   = $objecttype;
         $tmphash['description']  = $description;
         $tmphash['useraccess']   = $useraccess;
         $tmphash['friendaccess'] = $friendaccess;
         $tmphash['owner']        = $owner;
         break;
     case 'move':
         $tmphash['name']         = $myDB->f('Object_Name');
         $tmphash['newpath']      = $NewPath;
         break;
     case 'delete':
         $tmphash['name']         = $myDB->f('Object_Name');
         break;
  }//swithch
  $retarray[$idx] = $tmphash;
  $idx++;
 } //while
 return $retarray;
}*/
  
function mv_object($old_path, $new_path){
 global $myDB, $glob_language;
  
 $new['level']     =  level_count($new_path);
 $new['parent_id'] =  path2id(addslashes(parent_path($new_path)));
 $query = FormatSQL(SQL_MV_D, array(addslashes($new_path), $new['level'], $new['parent_id'], addslashes($old_path)));
 update_dir_changedate(path2id(parent_path(addslashes($old_path)))); 
 update_dir_changedate(path2id(parent_path(addslashes($new_path)))); 
 remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path($old_path)).'.'.$glob_language.'.html');
 remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path(parent_path($old_path))).'.'.$glob_language.'.html');
 remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path($new_path)).'.'.$glob_language.'.html');
 remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path(parent_path($new_path))).'.'.$glob_language.'.html');

 if (!is_trail($old_path))
  update_targets(addslashes($old_path), addslashes($new_path));
// echo("<pre>$query</pre><p>");
 return $myDB->query($query);
}

function rm_object($path, $now=false){
global $myDB, $glob_language;

if (!is_integer($path)){
 $pathid   = path2id($path);
 $pathname = $path;
}else{
 $pathid   = $path;
 $pathname = id2path($path);
}
if ($now){
 $res=false;
// die("delete object");
 $query=FormatSQL(SQL_RM_D1, (array)$pathid);
 if ($myDB->query($query)){
  $query = FormatSQL(SQL_RM_D2, (array)$pathid);
  $res = $myDB->query($query);
  if ($res){
  if (is_trail($path))
   $query = FormatSQL(SQL_RM_D3, (array)$pathid);
   $res = $myDB->query($query);  
   if ($res){
   $query = FormatSQL(SQL_RM_D5, (array)$pathid);
   $res = $myDB->query($query);  
   }
   if ($res){
   $query = FormatSQL(SQL_RM_D6, (array)$pathid);
   $res = $myDB->query($query);  
   }
  }else{
//   $query = FormatSQL(SQL_RM_D3, (array)$pathid); STUUUUSSS!
//   $res = $myDB->query($query);  
  }
 }
}else{
 $query=FormatSQL(SQL_MD_O_I, array($pathname, 'delete', ''));
 $res = $myDB->query($query);
}
update_dir_changedate(path2id(parent_path($path))); 
remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path($path)).'.'.$glob_language.'.html');
remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path(parent_path($path))).'.'.$glob_language.'.html');

return $res; 
}

/* 
 The followibng function takes a Name or an ID of a node to delete. It deletes the
 node and all subobjects (Nodes, Trails and Links) it contains.
 
 To achieve this, there are two possible ways: One forward which menas recursing
 the Directory to get all the affected entries and one backward which means to delete
 only the node and then look for Objects without parents and deleting them.
 
 The second way is much easier to implement and helps keeping the directory clean.
 
 For the sake of possible future enhancements (optimisations), the cleaning-up-functionality
 is in another function...
*/ 

function rm_node($path){
 global $myDB, $glob_language;

 if (!is_integer($path)){
 $pathid   = path2id($path);
 $pathname = $path;
 }else{
 $pathid   = $path;
 $pathname = id2path($path);
 }
 
 $query=FormatSQL(SQL_RM_D1, (array)$pathid);    //delete the node from ltrDirectory
 if ($myDB->query($query)){
  $query = FormatSQL(SQL_RM_D2, (array)$pathid); //delete the node from ltrDirectoryData
  $res = $myDB->query($query);
 }
 update_dir_changedate(path2id(parent_path($pathname))); 
 remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path($pathname)).'.'.$glob_language.'.html');
 remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path(parent_path($pathname))).'.'.$glob_language.'.html');
 while (cleanup_directory() > 0) //now: Cleanup the parentless objects and their childs (4 Queries!)
  cleanup_directory();
}

function cleanup_directory(){
 global $myDB, $glob_language;

$query = SQL_GET_PARENTLESS;
$myDB->query($query);
$str = "";
$found = 0;
while ($myDB->next_record()){
 $found++; //counter of found objects
 $bimbo = $myDB->f("Node_ID");

 remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path($myDB->f("Name"))).'.'.$glob_language.'.html');
 remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path(parent_path($myDB->f("Name")))).'.'.$glob_language.'.html');

 if ($str == "")
  $str = $bimbo;
 else
  $str = $str.",".$bimbo;
}
if ($found == 0) return 0;

$query = SQL_CLEANUP_DATA . "($str)";
$myDB->query($query);

$query = SQL_CLEANUP_LINKS . "($str)";
$myDB->query($query);
$query = SQL_CLEANUP_DIR . "($str)";
$myDB->query($query);
}

function edit_object($id, $obj){
 global $myDB, $glob_language;
 
 foreach($obj as $key => $val){ $obj[$key] = cleanup_string($obj[$key]); addslashes($obj[$key]);}
 $obj['intnode'] = ($obj['intnode'] == "") ? "NULL" : $obj['intnode'];
 
 $sql1 = FormatSQL(SQL_OEDT_1, array($obj['name'], $obj['level'], $obj['language'], $obj['intnode'], $id));
 if ( $myDB->query($sql1) ){
  $sql2 = FormatSQL(SQL_OEDT_2, array($obj['description'], $obj['useraccess'], $obj['friendaccess'], $obj['userid'], $id)); 
  remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path($obj['name'])).'.'.$glob_language.'.html');
  remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path(parent_path($obj['name']))).'.'.$glob_language.'.html');
  return $myDB->query($sql2);
 }else
  return false;
}

function add_object($obj, $now=false){
 global $myDB, $glob_language;
 
 foreach($obj as $key => $val){ $obj[$key] = cleanup_string($obj[$key]); }
 $obj['name']    = str_replace(" ", "_", $obj['name']);
 $obj['intnode'] = ($obj['intnode'] == "") ? "NULL" : $obj['intnode'];
 $query = FormatSQL(SQL_OADD_1, array($obj['name'], $obj['parent'], $obj['level'], $obj['objecttype'], $obj['language'], $obj['intnode']));
 if ($myDB->query($query)){
  $query = FormatSQL(SQL_OADD_a, (array)$obj['name']);
  $myDB->query($query);
  if ($myDB->num_rows() > 0){
   $myDB->next_record();
   $nodeid = $myDB->f("Node_ID");
   $avg = is_trail($obj['name']) ? get_avg_points($obj['parent']) : 0;
   $query = FormatSQL(SQL_OADD_2, array($nodeid, $obj['description'], $obj['useraccess'], $obj['friendaccess'], $obj['owner'], $avg));
   update_dir_changedate(path2id(parent_path($obj['name']))); 
   remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path($obj['name'])).'.'.$glob_language.'.html');
   remove_from_cache(CACHE_DIR.'/directory/subnodes/'.base64_encode(parent_path(parent_path($obj['name']))).'.'.$glob_language.'.html');
   return $myDB->query($query);
  }
 }
 return false; 
}
