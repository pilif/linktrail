<?php

function addform($predef="", $errfile=""){
 global $sess, $nodeinfo, $HTTP_GET_VARS, $kat;
 $tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
 $tpl->set_file(array("simpleframe" => "simpleframe.html"));
 $tpl->set_var("CONTENT", print_trail_addform($kat, $predef, $errfile));
 $tpl->parse("simpleframe", "simpleframe");
 print $tpl->get("simpleframe");
}

function editform($predef="", $errfile=""){
 global $sess, $mytrail, $auth, $PATH_INFO, $perm;
 
 $capabilities = get_caps($perm, parent_path($mytrail['path']));
 //$mytrail is a complete nodeinfo-structure that comes from
 //trail.php
 
 $tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
 $tpl->set_file(array("simpleframe" => "simpleframe.html"));
 $tpl->set_var("CONTENT", print_trail_editform($mytrail, $predef, has_caps($capabilities,CAP_SUPERUSER), $errfile));
 
 $tpl->parse("simpleframe", "simpleframe");
 print $tpl->get("simpleframe");
}


function doit(){
global $sess, $kat, $mytrail, $create, $txt_title, $txt_description, $ok, $cancel, $HTTP_POST_VARS;

if (isset($cancel)){
 cancel();
 exit;
}elseif (isset($kat)){
 create($kat, $txt_title, $txt_description);
 exit;
}elseif (isset($ok)){
 edit($mytrail, $txt_description, $txt_title, false, isset($HTTP_POST_VARS['field_admin_do'])); 
 exit;
}elseif( ($HTTP_POST_VARS['ok'] == "") and ($HTTP_POST_VARS['cancel'] == "") and ($HTTP_POST_VARS['create'] == "")){
 del($mytrail['id'], isset($HTTP_POST_VARS['field_admin_do']));
 exit;
}

}

function edit(&$nodeinfo, $description, $title, $pass_lang_check = false, $act_as_admin=false){
 global $sess, $auth, $glob_language_name, $glob_language;
 $testnode = get_node_info((string)parent_path($nodeinfo['path']).$title);
 if (!defined("SPELLING_INC"))
  include("commonapi/spelling.inc");
 if (!defined("DIRECTORY_NOTIFICATION_INC"));
  include("messages/directory_notification.inc"); 

 $predef['title']       = $title;
 $title = str_replace(' ', '_', $title);
 $predef['description'] = $description; 
 
 if (strstr($title,'/')){
  editform($predef, "err_invalidslash.html");
  exit;
 }elseif($title == ""){
  editform($predef, "err_notitle.html");
  exit;
 }elseif($description == ""){
  editform($predef, "err_nodesc.html");
  exit;
 }elseif( ( $testnode != -1) and ($nodeinfo['id'] != $testnode['id']) ){
  editform($predef, "err_object_exists.html");
  exit;
 }elseif(!check_language($description, $glob_language_name) and !$pass_lang_check){
  lang_query($kat, $title, $description, $nodeinfo['id'], $act_as_admin);
 }else{ 
 $obj['name']         = parent_path($nodeinfo['path']).$title; 
 $obj['description']  = $description;
 $obj['useraccess']   = $nodeinfo['useraccess'];
 $obj['friendaccess'] = $nodeinfo['friendaccess'];
 $obj['userid']       = $nodeinfo['userid'];
 $obj['level']        = level_count($obj['name']);
 $obj['language']     = $glob_language;
 edit_object($nodeinfo['id'], $obj);
 $obj['id'] = $nodeinfo['id'];
 if ($act_as_admin)
  send_admin_notification($obj, LTMSG_TRAILCHG); //TODO: add reason
 //else 
 // send_notification($obj, $auth->auth['uname'], LTMSG_TRAILCHG, !($auth->auth['uid'] == $obj['owner'])); 
  //don't send if I delete my own trail
 Header("Location: ".$sess->url(build_good_url($obj['name'])."?reloadparent=1"));
 
 exit;
 }
}

function lang_query($kat, $title, $description, $trail_id=-1, $act_as_admin=false){
 $errtrail['kat']         = $kat;
 $errtrail['title']       = $title;
 $errtrail['description'] = $description;
 $errtrail['trail_id']    = $trail_id;
 $errtrail['act_as_admin']= $act_as_admin;
 
 $str = base64_encode(serialize($errtrail));
 
 $tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
 $tpl->set_file(array("simpleframe" => "simpleframe.html"));
 $tpl->set_var("CONTENT",  print_badlang($str));
 $tpl->parse("simpleframe", "simpleframe");
 print $tpl->get("simpleframe");
 page_close();
 exit;
}

function create($kat, $title, $description, $pass_language_check=false){
 global $sess, $auth, $glob_language_name, $glob_language;
 if (!defined("SPELLING_INC"))
 include("commonapi/spelling.inc");
 
 if (!strstr($kat, '/'))
  $kat = base64_decode($kat);

 $predef['kat']         = $kat;
 $predef['title']       = $title;
 $predef['description'] = $description; 
 $title = str_replace(' ', '_', $title); 
 $exists = get_node_info($kat.$title);
 $exists = ($exists != -1);
 if (strstr($title,'/')){
  $errfile = "err_invalidslash.html";
  addform($predef, $errfile);
  exit;
 }elseif($title == ""){
  $errfile ="err_notitle.html";
  addform($predef, $errfile);
  exit;
 }elseif($description == ""){
  $errfile = "err_nodesc.html";
  addform($predef, $errfile);
  exit;
 }elseif( $exists ){
  $errfile ="err_object_exists.html";
  addform($predef, $errfile);
  exit;
 }elseif((!check_language($description, $glob_language_name)) and (!$pass_language_check)){
  lang_query($kat, $title, $description);
 }else{ 
 $title = str_replace(' ', '_', $title); 
 $obj['name']         = $kat.$title;
 $obj['parent']       = path2id($kat); 
 $obj['level']        = level_count($kat);
 $obj['objecttype']   = "trail";
 $obj['description']  = $description;
 $obj['useraccess']   = 4;
 $obj['friendaccess'] = 12;
 $obj['owner']        = $auth->auth["uid"];
 $obj['language']     = $glob_language;
 add_object($obj, true); 

 Header("Location: ".$sess->url(build_good_url($obj['name'])."?reloadparent=1"));
 exit;
// echo("Debug: dieses Fenster schliessen");
 }
}

function cancel(){
 global $uniq;
 
 $time = stop_timer(2);
 //global $sess, $mytrail;
  $time = stop_timer(2);
 page_close();
  $time = stop_timer(2);

 Header("Location: ".$GLOBALS['sess']->url(build_good_url($GLOBALS['mytrail']['path'])));
 exit;
}

function del($id, $act_as_admin = false){  
 global $sess, $mytrail, $auth;

 if (!defined("DIRECTORY_NOTIFICATION_INC"));
  include("messages/directory_notification.inc"); 

 rm_object((integer)$id, true);

 if ($act_as_admin)
  send_admin_notification($mytrail, LTMSG_TRAILDEL); //TODO: add reason
 
 include("directory/edit/del-confirmation.html");
}

function lang_answer($trailinfo){
 global $HTTP_POST_VARS, $mytrail;
 
 $trailinfo = unserialize(base64_decode($trailinfo));
 if ($HTTP_POST_VARS['yes']){
  if ($trailinfo['trail_id'] != -1)
   edit($mytrail, $trailinfo['description'], $trailinfo['title'], true, $trailinfo['act_as_admin']);
  else
   create($trailinfo['kat'], $trailinfo['title'], $trailinfo['description'], true);
 }else{
  if ($trailinfo['trail_id'] != -1)
   editform($trailinfo);
  else
   addform($trailinfo);
 }
}

//die($QUERY_STRING);
if (!defined("LAY_DIRECTORY_INC"))
 include("layout/lay_directory.inc");
if (!defined("EDIT_DIRECTORY_INC"))
 include("dbapi/edit_directory.inc");
if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");
 
//die($PATH_INFO); 
$in_trail = 1;
page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm", "user" => "Linktrail_User"));

if (isset($action)){
 $sess->unregister("action");
 $sess->unregister("kat");
}else
 $action=($HTTP_POST_VARS['action'] != "") ? $HTTP_POST_VARS['action'] : $HTTP_GET_VARS['action']; 
 
$auth->login_if($auth->auth["uid"] == "nobody");
if (($action != "addform") and (is_trail($PATH_INFO) )){
 $caps    = get_caps($perm); 
 $mytrail = get_node_info($PATH_INFO);
 if ( ($auth->auth['uid'] != $mytrail['userid']) and (!has_caps($caps, CAP_SUPERUSER))){
  die(sprintf("keine berechtigung.<br>uid ist: %s<br>trail geh÷rt: %s", $auth->auth['uid'], $mytrail['userid']) );
  page_close();
  Header("Location: ".$sess->url(build_good_url($PATH_INFO)));
  exit;
 }
}else{
 $kat = $PATH_INFO;
} 

include("template.inc");

switch ($action) {
  case "":
          editform();
          break;
  case "addform":
          $mytrail = "";
          addform();
          break;
  case "doit":
          doit();
          $sess->unregister("kat");
          break;
  case "langanswer":
          lang_answer($trailinfo);
          break;
		  
}
 

page_close();
?>
