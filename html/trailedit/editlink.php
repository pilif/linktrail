<?php
function form($predef=""){
 global $mytrail, $i, $trailperms, $caps;
 if (is_array($predef)){
  print_trail_direct(array(), $mytrail, $trailperms, -1, $predef, has_caps($caps, CAP_SUPERUSER));
 }else{
  $links = get_links($mytrail['path']);
  if (is_array($links)){
   print_trail_direct($links, $mytrail, $trailperms, $i, "", has_caps($caps, CAP_SUPERUSER));
   //the output is printed directly for usability reasons when
   //hanling big trails under slow response time conditions
  }
 } 
}

function edit($act_as_admin){
 global $del_x, $cancel, $sess, $ok_edit;

 if (isset($ok_edit))
  proceed($act_as_admin);
 elseif (isset($cancel))
  cancel();
 else
  del($act_as_admin);
 
/*$sess->unregister("mytrail");
$sess->unregister("action"); 
$sess->unregister("thislink"); */
   
}

function cancel(){
 global $sess, $mytrail;
// die("cancel");
 Header("Location: ".$sess->url($mytrail['path']));
}

function del($act_as_admin=false){ 
 global $sess, $thislink, $mytrail, $auth;
 
 $thislink['trail'] = $mytrail['id'];
 rm_link_ex($thislink, true);
 if (!defined("DIRECTORY_NOTIFICATION_INC"));
  include("messages/directory_notification.inc"); 

 if ($act_as_admin)
  send_admin_notification($mytrail, LTMSG_TRAILCHG); //TODO: add reason
 else{ //will not send message if user kicks his own trail
  $hsh['url']   = $thislink['url'];
  $hsh['title'] = $thislink['title'];
  send_link_notification($mytrail, $hsh, $auth->auth['uname'], LTMSG_LINKDELE, !($auth->auth['uid'] == $mytrail['owner']));
 } 

 Header("Location: ".$sess->url($mytrail['path']));
}

function print_bad_lang($title, $url, $description, $reloadparent, $this_link = -1, $row="", $act_as_admin = false){
 global $mytrail, $ltrstr;
 
 $linkdata['title']        = $title;
 $linkdata['url']          = $url;
 $linkdata['description']  = $description;
 $linkdata['reloadparent'] = $reloadparent;
 $linkdata['this_link']    = $this_link;
 $linkdata['row']          = $row;
 $linkdata['act_as_admin'] = $act_as_admin;
 $str = base64_encode(serialize($linkdata));
 print(msg_box($ltrstr['Bad language'], print_badlang_link($str), $mytrail, "", 0, -1));
 exit;
}


function add($pass_lang_check = false, $act_as_admin = false){
 global $sess, $title, $url, $description, $mytrail, $auth, $reloadparent, $glob_language_name;
 
 if (!defined("SPELLING_INC"))
  include("commonapi/spelling.inc");
 if (!defined("DIRECTORY_NOTIFICATION_INC"));
  include("messages/directory_notification.inc"); 

 
 if(!check_language($description, $glob_language_name) and !$pass_lang_check){
  print_bad_lang($title, $url, $description, $reloadparent);
  exit;
 }
// die("Blepp: ".$mytrail['id']);
 $sess->unregister("formdata");  
 $obj['trail']       = $mytrail['id'];
 $obj['title']       = $title;
 $obj['description'] = $description;
 $obj['url']         = $url;
 $obj['owner']       = $auth->auth["uid"];
 
 add_link_ex($obj, true);

 if ($act_as_admin)
  send_admin_notification($mytrail, LTMSG_TRAILCHG); //TODO: add reason
 else{ //will not send message if user kicks his own trail
  $hsh['url']   = $url;
  $hsh['title'] = $title;
  send_link_notification($mytrail, $hsh, $auth->auth['uname'], LTMSG_LINKADD, !($auth->auth['uid'] == $mytrail['owner']));
 } 

 if ($reloadparent == 1)
  Header("Location: ".$sess->url($mytrail['path']."?reloadparent=1")); 
 else
 Header("Location: ".$sess->url($mytrail['path']));
 
}

function proceed($act_as_admin = false){
 global $thislink, $sess, $mytrail, $auth, $txt_linktitle, $txt_linkurl, $txt_linkdescription, $row, $HTTP_POST_VARS;
 
 if (!defined("DIRECTORY_NOTIFICATION_INC"));
  include("messages/directory_notification.inc"); 

/*foreach($HTTP_POST_VARS as $key => $value)
  echo("key=$key; value=$value<br>");
 if ( isset($txt_linktitle) )
  die("blupp");
 die("efertig");*/
 if (isset($txt_linktitle)) $new_obj['title'] = $txt_linktitle; else $new_obj['title'] = $thislink['title'];
 if (isset($txt_linkdescription)) $new_obj['description'] = $txt_linkdescription; else $new_obj['description'] = $thislink['description'];        
 if (isset($txt_linkurl)) $new_obj['url'] = $txt_linkurl; else $new_obj['url'] = $thislink['url'];        
 $new_obj['owner'] = $thislink['uid'];
 $new_obj['trail'] = $mytrail['id'];
 $new_obj['id'] = $thislink['id'];
 
 edit_link_ex($thislink['id'], $new_obj);
 if ($act_as_admin)
  send_admin_notification($mytrail, LTMSG_TRAILCHG); //TODO: add reason
 else{ //will not send message if user kicks his own trail
  $hsh['url']   = $txt_linkurl;
  $hsh['title'] = $txt_linktitle;
  send_link_notification($mytrail, $hsh, $auth->auth['uname'], LTMSG_LINKCHGD, !($auth->auth['uid'] == $mytrail['owner']));
 } 

 if ( isset($row) and ($row != "") ){
  reposition_link($new_obj, $row);
  if ($act_as_admin)
   send_admin_notification($mytrail, LTMSG_TRAILCHG); //TODO: add reason
  else{ //will not send message if user kicks his own trail
   $hsh['url']   = $thislink['url'];
   $hsh['title'] = $thislink['title'];
   send_link_notification($mytrail, $hsh, $auth->auth['uname'], LTMSG_LINKMOVD, !($auth->auth['uid'] == $mytrail['owner']));
  } 
 }
 
 // die("fertig");
 Header("Location: ".$sess->url($mytrail['path']));
}

function perm_check($type, $perms, $thislink){
 global $auth;
 
 $res = false;
 
 switch ($type) {
  case "add":
    $res = has_caps($perms, PERM_ADD);
  break;
  case "edit":
    $res = (has_caps($perms, PERM_EDIT) or has_caps($perms, PERM_MOVE) or ($thislink['uid'] == $auth->auth['uid']));
  break;
  case "move":
    $res = has_caps($perms, PERM_MOVE);
  break;
  case "del":
    $res = (has_caps($perms, PERM_DEL) or has_caps($perms, PERM_MOVE) or ($thislink['uid'] == $auth->auth['uid']));
  break;
  case "pass":
   $res = true;
  break;

 }
return $res;
}

if (!defined("LAY_TRAIL_INC"))
 include("layout/lay_trail.inc");
if (!defined("EDIT_LINKS_INC"))
 include("dbapi/edit_links.inc");
if (!defined("TRAILFUNCS_INC"))
 include("commonapi/trailfuncs.inc");
/*
 First I read the permissions of our user. 
*/
if (!defined("COMMON_PERMISSIONS_INC"))
 include("commonapi/common_permissions.inc");

//die(class_exists("Template"));
if (!class_exists("Template"))
 include("template.inc");

$in_trail = 1;
page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm", "user" => "Linktrail_User"));
if (isset($action)){
 $sess->unregister("action");
}else
 $action=($HTTP_POST_VARS['action'] != "") ? $HTTP_POST_VARS['action'] : $HTTP_GET_VARS['action']; 
 
if (isset($formdata['action'])){
 $action = $formdata['action'];
 $title = $formdata['title'];
 $description = $formdata['description'];
 $url = $formdata['url'];
}

$caps       = get_caps($perm); //used to read the superuser-capability of users with perm->have-perm("admin");
$mytrail = get_node_info($PATH_INFO);
if (isset($i))
 $thislink = get_link_info($i);

if ($mytrail == -1)
 $mytrail = get_node_info($PATH_INFO."?");
$trailperms = relevant_perms($mytrail, $auth->auth["uid"], $caps);
if ($action == "langanswer")
 $what = "pass";
if ($action == "add")
 $what = "add";
if ($action == "")
 $what = "pass";
if ($action == "edit"){
 if (isset($ok_edit))
  $what = "edit";
 elseif (isset($cancel))
  $what = "pass";
 else
  $what = "del";
} 

if ( ($auth->auth['uid'] != "nobody") and !perm_check($what, $trailperms, $thislink) ){
 page_close();
 Header("Location: ".$sess->url($PATH_INFO));
 exit;
} 
 

if ($action == "edit")
 edit(isset($HTTP_POST_VARS['field_act_as_admin']));
elseif ($action == "add"){
 $formdata['title'] = $title;
 $formdata['description'] = $description;
 $formdata['url'] = $url;
 $formdata['action'] = "add";
 $sess->register("formdata");
 $in_trail = 2;
 $auth->login_if($auth->auth["uid"] == "nobody");
 add(isset($HTTP_POST_VARS['field_act_as_admin']));
}elseif($action == "langanswer"){
 $linkdata    = unserialize(base64_decode($linkdata));
 if (isset($yes)){
  $title        = $linkdata['title'];
  $url          = $linkdata['url'];
  $description  = $linkdata['description'];
  $reloadparent = $linkdata['reloadparent'];
  $act_as_admin = $linkdata['act_as_admin'];
  add(true, $act_as_admin);
 }else{
  form($linkdata);
 }
}else{
 //die("Mache form");
 form();
}           

page_close();
?>
