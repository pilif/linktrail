<?php
function add(){
 global $sess, $title, $level, $description, $int_node, $thisnode, $glob_language;
 
 if (is_trail($title))
  $title = $title."/"; 
 
 if (level_count($title) > 0){
  include("directory/edit/err_invalidslash.html");
 }else{ 
 $title = str_replace(' ', '_', $title); 
 $obj['name']         = $thisnode['path'].$title;
 $obj['name'] = str_replace(' ', '_', $obj['name']);
 $obj['parent']       = path2id($thisnode['path']);
 $obj['level']        = $level;
 $obj['objecttype']   = 'node';
 $obj['description']  = $description;
 $obj['useraccess']   = 0;
 $obj['friendaccess'] = 0;
 $obj['owner']        = '';
 $obj['language']     = $glob_language;
 
 $intid          = path2id_ex($int_node, 1);
 $obj['intnode'] = ($intid != -1) ? $intid : "";
 
 add_object($obj, true);
 header("Location: ".$sess->url($thisnode['path'])); 
 }
}

function edit(){
 global $sess, $title, $level, $description, $int_node, $thisnode, $glob_language;
 
 if (is_trail($title))
  $title = $title."/"; 
 
 if (level_count($title) > 0){
  include("directory/edit/err_invalidslash.html");
 }else{ 
 $obj['name']         = parent_path($thisnode['path']).$title;
 $obj['name'] = str_replace(' ', '_', $obj['name']);
 $obj['level']        = $level;
 $obj['description']  = $description;
 $obj['useraccess']   = 0;
 $obj['friendaccess'] = 0;
 $obj['owner']        = '';
 $obj['language']     = $glob_language;

 $intid          = path2id_ex($int_node, 1);

 $obj['intnode'] = ($intid != -1) ? $intid : "";

 if ($obj['name'] != $thisnode['path'])
  sub_rename($thisnode['path'], $obj['name']);

 edit_object($thisnode['id'], $obj);
// die("Fertig: ".$obj['name']);
 header("Location: ".$sess->url($obj['name']));
 }
}

function cancel(){
 global $thisnode;
 
// die("gehe zu: "."Location: ".$thisnode['path']);
 header("Location: ".$thisnode['path']);
}

function del(){
  global $thisnode, $sess;
  
  rm_node($thisnode['path']);
  $parent = parent_path($thisnode['path']);
 
 Header("Location: ".$sess->url($parent));

}

function ltrsymlink(){
 global $HTTP_POST_VARS, $thisnode, $sess;
 foreach($HTTP_POST_VARS as $key=>$value){
  if(preg_match('/(.*)?_([0-9]+)/', $key, $erg))
   if ( ($erg[1] == "rename") or ($erg[1] == "delete") ){
    $method=$erg[1];
    $id=$erg[2];
   } 
 }
 if (($id != "") and ($method != "")){
  if ($method == "delete"){
   delete_symlink($id);
  }elseif($method == "rename"){
   if ($HTTP_POST_VARS["field_name-$id"] != ""){
    if (level_count($HTTP_POST_VARS["field_name-$id"]) > 0){
     include("directory/edit/err_invalidslash.html");
     exit;
    }
    $fullpath = id2path($id);
    rename_symlink($id,  parent_path($fullpath).$HTTP_POST_VARS["field_name-$id"].'/');
   }
  }
 }
//echo("Location: ".$thisnode['path']."?editmode=1");
 header("Location: ".$sess->url($thisnode['path']."?editmode=1"));
exit;
}

if (!defined("LAY_DIRECTORY_INC"))
 include("layout/lay_directory.inc");
if (!defined("EDIT_DIRECTORY_INC"))
 include("dbapi/edit_directory.inc");
if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");
include("template.inc");

page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm", "user" => "Linktrail_User"));
$sess->unregister("thisnode");         

if (empty($action))
 $action = "";

if (isset($cancel))
 $action = "cancel";
elseif (isset($del_x))
 $action = "del";
switch ($action) {
  case "add":
          add();
          break;
  case "symlink":
          ltrsymlink();
          break;
  case "edit":
		  edit();
          break;
  case "cancel":
          cancel();
          break;
  case "del":
          del();
          break;
        
}
 

page_close();
echo("done");
exit
?>
