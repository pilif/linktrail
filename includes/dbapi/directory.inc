<?php
 define("DIRECTORY_INC", true);
/*
  directory.inc; 1.01
  
  Contains DataBase-Functions used to build the directory-tree
  Documentation is inline and in directory.inc.html when ready
  
  Written by Pilif and Jody
  
  Created:  Feb, 20th 2000
  Modified: Mar,  5th 2000
 
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

function get_symlinks_to($path){
 global $myDB;
 
 $query = FormatSQL(SQL_GET_SYMLINKS_TO_DESTINATION, array(addslashes($path)));
 $myDB->query($query);
 if ($myDB->num_rows() <= 0)
  return -1;
  
 while ($myDB->next_record()){
  $hsh['path']    = $myDB->f('Name');
  $hsh['id']      = $myDB->f('Node_ID');
  $hsh['linkto']  = $myDB->f('LinkTo');
  $arr[] = $hsh; 
 }
 return $arr;
} 

 
function get_maxlevel(){
 global $myDB;
 
 $res = 0;
 $query = SQL_MAXLEVEL;
 if ( $myDB->query($query) ){
  $myDB->next_record();
  $res = $myDB->f("Level");
 }
 return $res;
}

//function get_subnodes_ex($node: string): array of hash

function get_subnodes_ex($node_id=1){
 global $myDB, $glob_language;
 $node = addslashes($node);
 $query = FormatSQL(SQL_SUBNODES_EX, array($node_id, $glob_language));
//die("<pre>$query</pre>");
 $myDB->query($query);
 if ($myDB->num_rows() <= 0) {
  return -1;
 }else{
  $idx = 0;
  $done = Array();
  $subcount = 0;
  $chidx = false;
  while ($myDB->next_record()){
   if (!$done[$myDB->f("name")]){
    $subcount = 0;
    $done[$myDB->f("name")] = 1;
    $arr[$idx]['name']   =  $myDB->f("name");
    $arr[$idx]['linkto'] =  $myDB->f("LinkTo");
    $indexes[$myDB->f("name")] = $idx;
    $idx++;
   }
   if ( ($myDB->f("subname") != "") and ($subcount < CAT_UP_COUNT_HI) ){
    $index = $indexes[$myDB->f("name")];
    $arr[$index]['subs'][$subcount]['name'] = $myDB->f("subname"); 
    $arr[$index]['subs'][$subcount]['linkto'] = $myDB->f("sub_link_to"); 
    $subcount++;
   }
  }
  reset($arr);
  return $arr;
 }
} 
 
/*
  functon get_subnodes($node: string, tree: integer): array of string;
  
  Retreives all subnodes of current node. $node is the path of the
  object.
  
  If $node is an integer, you can avoid an extra query for getting
  the path's Object_ID.
  
  If $direct is false, the function will return all categories 
  
  If $climb is false, the funtion will return an array of paths
  to the subnodes.
  
  ************************
  * Warning: The following is currentely untested!!! *
                              ************************
  
  If $climb is true, the function will return an array of the following
  hash:
    
  $hsh['path'] = "path/to/the/subnode/"
  $hsh['sub']  = array("paths/to/subsubnodes/with/enough/points")
  
   "enough points" is defined in config.inc
  
  Returns -1 if no Sub-Items found
*/
 
function get_subnodes($node, $climb=false){
 global $myDB;
 
 if (is_integer($node)){
  $path = id2path($node);
  if ($path == -1) 
   return -1;
 }else
  $path = $node;
 $path = addslashes($path); 
 $query = FormatSQL(SQL_SUBNODE, (array)$path); 
 die($query);
 $myDB->query($query);
 if ($myDB->num_rows() <= 0) {
  return -1;
 }else{
  $idx = 0;
  while ($myDB->next_record()){
   $arr[$idx] = $myDB->f("Name");
   $idx++;
  }
  reset($arr);
  if (!$climb)
   return $arr;
  else{
   for($x=0; $x<count($arr); $x++){
    $nrs[0] = level_count($arr[$x]);
    $nrs[1] = $nrs[0]; 
    $query = FormatSQL(SQL_CATUP, $nrs); 
    echo("$query<br>");
    $myDB->query($query);
    $tmphash['path'] = $arr[$x];
    $idx = 0;
    while ($myDB->next_record()){
     $tmparr[$idx] = $myDB->f("Name");
     $idx++;
    }
    $tmphash['sub'] = $tmparr;
    $newarr[$x] = $tmphash; 
   }
   return $newarr;
  } 
 }
}


function get_trail_count($node){
 global $myDB;
 $node = addslashes($node);
 $query = FormatSQL(SQL_TRAILCOUNT, array($node));
 $myDB->query($query);
 if ($myDB->num_rows() == 0)
  return 0;
 $myDB->next_record();
 return $myDB->f("c");
}
/*
  functon get_trails($node: string[, from: integer]): array of hash;
*/
 
function get_trails($node, $from=0, $level=0){
 global $myDB;
 
 // get the Object_ID
 if (is_integer($node)){
  $path = id2path($node);
  if ($path == -1) 
   return -1;
 }else
  $path = $node;  
 
 $path = addslashes($path); 
 if ($from != -1)
  $query = FormatSQL(SQL_DIRSTRAILL, array($path, $from));
 else
  $query = FormatSQL(SQL_DIRSTRAILU, (array)$path);
$myDB->query($query);
if ($myDB->num_rows() <= 0) {
 return -1;
}else{
 $idx = 0;
 while ($myDB->next_record()){
  $hsh['path']         = $myDB->f("Name");
  $hsh['id']           = $myDB->f("Node_ID");
  $hsh['description']  = $myDB->f("Description");
  $hsh['userid']       = $myDB->f("Owner");
  $hsh['username']     = $myDB->f("username");
  $hsh['useraccess']   = $myDB->f("UserAccess");
  $hsh['friendaccess'] = $myDB->f("FriendAccess");
  $hsh['changedate']   = $myDB->f("ChangeDate");
  $hsh['adddate']      = $myDB->f("AddDate");
  $hsh['hasimage']     = $myDB->f("HasImage");
  $hsh['from']         = $myDB->f("Parent");
  $hsh['sponsored']    = $myDB->f("SponsoredBy");
  $hsh['ownernotification'] = $myDB->f("OwnerNotification");
  if (!$direct){
   $hsh['points']       = $myDB->f("ExtraLong")*REM_POINTS;
   $hsh['up']           = true;
  }else{
   $hsh['points']       = $myDB->f("ExtraLong");      
   $hsh['up']           = false; 
  }
  $arr[$idx] = $hsh;
  $idx++;
 }
 reset($arr);
 return $arr; 
}
 
}

function get_trails_by_date($id, $fdate=""){
 global $myDB, $glob_language;
  
 if ($id == "")
  $query = FormatSQL(SQL_NEWTRAILSG, array($glob_language));
 else
  $query = FormatSQL(SQL_NEWTRAILSL, array($id, $glob_language));
$myDB->query($query);
if ($myDB->num_rows() <= 0) {
 return -1;
}else{
 $idx = 0;
 while ($myDB->next_record()){
  $hsh['path']         = $myDB->f("Name");
  $hsh['id']           = $myDB->f("Node_ID");
  $hsh['changedate']   = $myDB->f("ChangeDate");
  $hsh['adddate']      = $myDB->f("AddDate");
  $arr[] = $hsh;
 }
 reset($arr);
 return $arr; 
}
 
}


function get_node_info($node){
 global $myDB, $glob_language;
 
 // get the Object_ID
 if (!strstr($node, '/')){
  $path = id2path($node);
  if ($path == -1) 
   return -1;
 }else
  $path = $node;  
  
  $path = addslashes($path);
  
  if (is_trail($path) )
   $query = FormatSQL(SQL_OBJINFO, array($path, $glob_language));
  else 
   $query = FormatSQL(SQL_OBJINFO_NODE, array($path, $glob_language));

//die("<pre>$query</pre>");
  $myDB->query($query);
  if ($myDB->num_rows() <= 0) {
    return -1;
  }else{
   $myDB->next_record();
   $hsh['path']         = $myDB->f("Name");
   $hsh['id']           = $myDB->f("Node_ID");
   $hsh['description']  = $myDB->f("Description");
   $hsh['level']        = $myDB->f("Level");
   $hsh['linkto']       = $myDB->f("LinkTo");
   $hsh['userid']       = $myDB->f("Owner");
   $hsh['username']     = $myDB->f("username");
   $hsh['useraccess']   = $myDB->f("UserAccess");
   $hsh['friendaccess'] = $myDB->f("FriendAccess");
   $hsh['changedate']   = $myDB->f("ChangeDate");
   $hsh['adddate']      = $myDB->f("AddDate");
   $hsh['hasimage']     = $myDB->f("HasImage");
   $hsh['language']     = $myDB->f("Language");
   $hsh['intnode']      = $myDB->f("IntNode");
   $hsh['from']         = $myDB->f("Parent");
   $hsh['sponsored']    = $myDB->f("SponsoredBy");
   $hsh['ownernotification'] = $myDB->f("OwnerNotification");
   return $hsh; 
   }
 }
 

function count_trails_mypage($uname, $foreign=false){
 global $myDB;
 $str = $foreign ? "FOREIGN_" : "";
 eval('$query = SQL_'.$str.'MYPAGE_COUNT;');
 $uname = addslashes($uname);
 $query = FormatSQL($query, array($uname));
 $myDB->query($query);
 if ($myDB->num_rows() <= 0) 
  return 0;
 $myDB->next_record();
 return $myDB->f("c");
}

/*
 Returns all trails that belong to or are subscribed from the
 given user.
*/
function get_trails_by_uname($uname, $uid, $current_index, $order_by, $order_type="ASC", $foreign=false){
 global $myDB;
  $str = $foreign ? "FOREIGN_" : "";
  eval('$queryl = SQL_'.$str.'MYPAGEL;');
  eval('$queryu = SQL_'.$str.'MYPAGE;');
 if ($current_index == -1)
  $query = FormatSQL($queryu, array($uname, $order_by, $order_type));
 else
  $query = FormatSQL($queryl, array($uname, $order_by, $order_type, $current_index));
 $myDB->query($query);
 if ($myDB->num_rows() <= 0) {
  return -1;
 }else{
  $idx = 0;
  while ($myDB->next_record()){
   $subscribed =  ($myDB->f("SubscribedToTrail") != "");
/*   echo($myDB->f("friendlyname_trail")." | ");
   echo($myDB->f("friendlyname_parent")."<br>");
*/
   $hsh['path']         = $myDB->f("Name");
   $hsh['id']           = $myDB->f("Node_ID");
   $hsh['description']  = $myDB->f("Description");
   $hsh['userid']       = $myDB->f("Owner");
   $hsh['username']     = $myDB->f("username");
   $hsh['useraccess']   = $myDB->f("UserAccess");
   $hsh['friendaccess'] = $myDB->f("FriendAccess");
   $hsh['changedate']   = $myDB->f("ChangeDate");
   $hsh['adddate']      = $myDB->f("AddDate");
   $hsh['hasimage']     = $myDB->f("HasImage");
   $hsh['from']         = $myDB->f("Parent");
   $hsh['sponsored']    = $myDB->f("SponsoredBy");
   $hsh['points']       = $myDB->f("ExtraLong"); 
   $hsh['language']     = $myDB->f("Language"); 
   $hsh['ownernotification'] = $myDB->f("OwnerNotification");
   $hsh['subscribed']   = $subscribed;
   $hsh['up']           = false;
   $hsh['suggested_from'] = $myDB->f("suggested_from");
   $arr[$idx] = $hsh;
   $idx++;
  }
  reset($arr);
  return $arr; 
 } 
} 
 
?>