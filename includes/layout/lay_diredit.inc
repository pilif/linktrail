<?php
define("LAY_DIREDIT_INC", true);
/*
  edit_directory.inc; 1.00
  
  Contains DataBase-Functions used to build the directory-tree
  Documentation is inline and in directory.inc.html when ready
  
  Written by Pilif and Jody
  
  Created:  Apr, 9th 2000
 
  © 2000 by Linktrail. All rights reserved  
 */
 
if (!defined("DIRECTORY_INC"))
 include("dbapi/directory.inc");

if (!defined("LAY_DIRECTORY_INC"))
 include("layout/lay_directory.inc");
 
if (!defined("BUTTONS_INC"))
 include("layout/buttons.inc");
 
if (!defined("DEFEXP_INC"))
  include("dbapi/defexp.inc");

function print_editform_add($node){
 global $sess, $PHP_SELF, $thisnode, $glob_language;
 
 $tpl = new Template(APPLICATION_HOME."/templates/directory/edit", "keep");
 if ($thisnode['language'] == 1){
  $tpl->set_file(array("main" => "nodeedit_add.html"));
 }else{
  $tpl->set_file(array("main" => "nodeedit_add_int.html"));
 } 
 $tpl->set_var("ACTION_ADD", $sess->url("/diredit/editdir.php"));
 $tpl->set_var("HIDDENS_ADD", '<input type="hidden" name="action" value="add">');
 $tpl->set_var("THISNODE", friendlyname($thisnode['path'])); 

 $max_level=get_maxlevel()+1;
 $options = "";
 for ($x=0; $x < $max_level+1; $x++){
  if ($x != $thisnode['level']+1)
   $options .= '<option value="'.$x.'"'.">$x\n";
  else
   $options .= '<option selected value="'.$x.'"'.">$x\n"; 
 }
 $tpl->set_var("LEVELOPTIONS_ADD", $options); 
 $tpl->set_var("BUTTONS_ADD", print_trail_editbuttons("addform"));
 $tpl->parse("main", "main");
 return $tpl->get("main"); 
}


function create_cat_move_url($id){
  global $REQUEST_URI, $HTTP_GET_VARS, $QUERY_STRING;
  
  $url = $REQUEST_URI;
  if ($QUERY_STRING != 'movenode='.$HTTP_GET_VARS['movenode'])
    $url .=  (strstr($url, "?")) ? "&" : "?";
  if (strstr($url, 'movenode='))
   $url = str_replace('movenode='.$HTTP_GET_VARS['movenode'], 'movenode='.$id, $url);
  else
   $url .= 'movenode='.$id;

 $url = str_replace('editmode=1', 'editmode=0', $url);
 return $url;
}

function print_one_symlink($mininode){
 global $sess;
 
 $tpl = new Template(APPLICATION_HOME."/templates/directory/edit", "keep");
 $tpl->set_file(array("main" => "edit_symlink.html"));
 $tpl->set_var('SOURCE_NODE_URL', $sess->url(parent_path($mininode['path'])));
 $tpl->set_var('SOURCE_NODE_NAME', friendlyname(parent_path($mininode['path'])));
 $tpl->set_var("NAME", friendlyname($mininode['path']));
 $tpl->set_var("SYMLINK_ID", $mininode['id']);
 $tpl->parse("main", "main");
 return $tpl->get("main"); 

}

function print_editform_symlink($kat){
 global $sess, $PHP_SELF, $thisnode, $REQUEST_URI;
$symlinks = get_symlinks_to($kat);
 if ($symlinks == -1)
  return "";

 
 $tpl = new Template(APPLICATION_HOME."/templates/directory/edit", "keep");
 $tpl->set_file(array("main" => "nodeedit_symlinks.html"));
 $tpl->set_var("THISNODE", friendlyname($kat)); 
 $tpl->set_var("ACTION_SYMLINK", $sess->url("/diredit/editdir.php"));
 $tpl->set_var("HIDDENS_SYMLINK", '<input type="hidden" name="action" value="symlink">');

 $str = "";
 for($x=0; $x < count($symlinks); $x++)
   $str .= print_one_symlink($symlinks[$x]);
 $tpl->set_var("SYMLINKS", $str);

 $tpl->parse("main", "main");
 return $tpl->get("main"); 
}


function print_editform_edit($kat){
 global $sess, $PHP_SELF, $thisnode, $REQUEST_URI;

 $tpl = new Template(APPLICATION_HOME."/templates/directory/edit", "keep");
 if ($thisnode['language'] == 1){
  $tpl->set_file(array("main" => "nodeedit_edit.html"));
 }else{
  $tpl->set_file(array("main" => "nodeedit_edit_int.html"));
  if ($thisnode['intnode'] != "")
   $tpl->set_var("PREDEF_INTNODE", id2path($thisnode['intnode']));
  else
   $tpl->set_var("PREDEF_INTNODE", "");
 } 
 $tpl->set_var("THISNODE", friendlyname($kat)); 
 $tpl->set_var("ACTION_EDIT", $sess->url("/diredit/editdir.php"));
 $tpl->set_var("HIDDENS_EDIT", '<input type="hidden" name="action" value="edit">');
 $tpl->set_var("PREDEF_TITLE", friendlyname($kat));
 $max_level=get_maxlevel()+1;
 $options = "";
 for ($x=0; $x < $max_level+1; $x++){
  if ($x != $thisnode['level'])
   $options_s .= '<option value="'.$x.'"'.">$x\n";
  else
   $options_s .= '<option selected value="'.$x.'"'.">$x\n"; 
 }
 $tpl->set_var("LEVELOPTIONS_EDIT", $options_s);
 $tpl->set_var("PREDEF_DESCRIPTION", $thisnode['description']); 
 $tpl->set_var("DELBUTTON_EDIT", get_button(0, "", "middle", "", 'onClick="if (confirm(\'Deleting this Node will remove all Trails it contains with all their links!!!\nProceed?\')) document.editform.submit(); else return false;"'));
 $tpl->set_var("BUTTONS_EDIT", print_trail_editbuttons("editform"));
 $tpl->set_var("MOVEURL", create_cat_move_url($thisnode['id']));
 $REQUEST_URI = alter_url($REQUEST_URI, 'editmode', 0);
 $tpl->set_var("LINKURL", alter_url($REQUEST_URI, 'linknode', $thisnode['id']));

 $tpl->parse("main", "main");
 return $tpl->get("main"); 
}
 
 
function print_editform($kat){
 global $sess, $PHP_SELF, $thisnode;
 
 $thisnode = get_node_info($kat);

 $sess->register("thisnode");
 
 $res  = print_editform_add($thisnode);
 if ($kat != "/")
  $res .= print_editform_edit($kat);
 $res.= print_editform_symlink($kat);
 return $res;
} 

function print_defexp_form($nodeinfo, $exp_error=""){
 global $sess;
 
 if (is_array($exp_error)){
  $predef   = $exp_error['predef'];
  $errorstr = $exp_error['errorstr'];
 }

 $tpl = new Template(APPLICATION_HOME."/templates/directory/edit", "keep");
 $tpl->set_file(array("main" => "edit_defexp.html"));
 $tpl->set_var("THISNODE", friendlyname($nodeinfo['path']));
 $tpl->set_var("ACTION", $sess->url("/diredit/edit_expert.php".build_good_url($nodeinfo['path'])));
 $tpl->set_var("ERRORS", $errorstr);
 $tpl->set_var("FULLPATH", $nodeinfo['path']);

 $exp = "";
 if (is_array($predef))
  $exp = $predef;
 else
  $exp = get_def_expert($nodeinfo['id']);

$tpl->set_var("HIDDENS", '<input type="hidden" name="action" value="expert"><input type="hidden" name="old_expert" value="'.$exp['Name'].'">');
  
  
 if ($exp == ""){
  $tpl->set_var("CHECKED_AUTO", 'checked'); 
  $tpl->set_var("CHECKED_DEFINE", ''); 
  $tpl->set_var("EXPERT", "(not set)");
  $tpl->set_var("DESCRIPTION", "(not set)");
 }else{
  $tpl->set_var("CHECKED_AUTO", ''); 
  $tpl->set_var("CHECKED_DEFINE", 'checked'); 
  $tpl->set_var("EXPERT", $exp['Name']);
  $tpl->set_var("DESCRIPTION", $exp['About']);
 } 
 $tpl->parse("main", "main");
 return $tpl->get("main"); 

}

?>