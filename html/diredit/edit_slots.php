<?php
page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm", "user" => "Linktrail_User"));
if (!defined("COMMON_PERMISSIONS_INC"))
 include("commonapi/common_permissions.inc");
if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");
if (!defined("LAY_DIRECTORY_INC"))
 include("layout/lay_directory.inc");
if (!defined("LAY_SLOTS_INC"))
 include("layout/lay_slots.inc");
if (!class_exists("Template"))
 include("template.inc");
$capabilities = get_caps($perm, $kat); 
 
$url    = str_replace ('//', '/', $PATH_INFO);
$pos    = strrpos($url, '/')+1;
$method = substr($url, $pos);
$kat    = substr($url, 0, $pos);
if (!has_caps($capabilities, CAP_EDIT_SLOTS)){
 Header("Location: ".$sess->url($kat));
 exit;
}
if ($method == "") $method = "Add_Slot";

//die("Method: $method || Path: $kat || pathinfo: $PATH_INFO");
$nodeinfo = get_node_info($kat);

switch ($method) {
    case 'Add_Slot':
        if ($REQUEST_METHOD == "GET")
         add_slot_form();
        else
         if (isset($del))
          go_back();
         else
          add_slot();
        break;
    case 'Edit_Slot':
        if ($REQUEST_METHOD == "GET")
         edit_slot_form();
        else
         if (isset($del))
          delete_slot();
         else
          edit_slot();
        break;
    default:
        error('Unknown method', 'The method passed is not valid. Do not call me directely', 8, __FILE__);
        break;
}


function print_page_begin(){
 global $kat, $method;
 
 $pathlist = $kat.'Slots/'.$method;
 $pl  = build_pathlist($pathlist, false);
 $plf = build_pathlist($pathlist, true);
 $restriction_list = build_restriction_list($kat);
 $username = $auth->auth['uname']; 
 $in_login = false;
 include("commonheader2.html");
}

function print_page_end(){
 global $glob_language_name;
 include("commonfooter2.html");
}


//echo("qs: $QUERY_STRING");

function add_slot_form(){
 global $kat, $capabilities, $nodeinfo;
print_page_begin();
$tpl = new Template(APPLICATION_HOME."/templates/directory", "keep");
$tpl->set_file(array("main" => "main.html"));

$tpl->set_var("SUBNODES", print_slot_form($nodeinfo));
$tpl->set_var("TRAILS", "");
$tpl->set_var("IWANTTO", print_iwantto($kat, $capabilities));
$tpl->set_var("FEATURED", print_featured($kat, $capabilities));
$tpl->set_var("TOPEX", print_topex($kat));
$tpl->set_var("NEWTRAILS", print_newtrails($activekat));
$tpl->set_var("DIDYOUKNOW", "");
$tpl->set_var("OPENTRAIL", "");
$tpl->parse("main", "main");
$tpl->p("main");
print_page_end();
}

function validate_and_set(&$http_vars){
 global $nodeinfo;
 
 $errors = Array();
 
 if ( ($http_vars['field_title_1'] == "") or ($http_vars['field_trail_1'] == "") )
  $errors[] = "You must fill out Trail 1";

 if (count($errors) == 0){ 
  $trail1id = path2id( urldecode($http_vars['field_trail_1']) );
  if ($trail1id == -1)
   $errors[] = "Trail 1 (".$http_vars['field_trail_1'].") was not found in the database. Please correct the path";
 }  

 if ($http_vars['field_trail_2'] != ""){
  $trail2id = path2id( urldecode($http_vars['field_trail_2']) );
  if ($trail2id == -1)
   $errors[] = "Trail 2 was not found in the database. Please correct the path";
 }else
  $trail2id = "";

 if (count($errors) != 0)
  slot_edit_error($errors);
  
 $slot['id']           = $http_vars['id'];
 $slot['node']         = $nodeinfo['id'];
 $slot['trail_1_id']   = $trail1id;
 $slot['trail_1_text'] = $http_vars['field_title_1'];
 $slot['trail_2_id']   = $trail2id;
 $slot['trail_2_text'] = $http_vars['field_title_2'];
 $slot['description']  = $http_vars['field_description'];
 $slot['next']         = "";
 $slot['islive']       = (isset($http_vars['field_slot_live'])) ? "y" : "n";

 return $slot;
}

function copy_file($slot_id){
 global $nodeinfo, $field_image_file, $field_image_file_name;
 if (!file_exists($field_image_file)) return;
 $ext = extract_file_ext($field_image_file_name);
 if ($ext == ""){
  $exts[] = ".gif";
  $exts[] = ".jpg";
  $exts[] = ".png";
  $arr = getimagesize ($field_image_file);
  $ext = $exts[$arr[2]-1];
 }
 remove_file($slot_id); //or else extension-changes would not get notified..
 copy ($field_image_file, APPLICATION_HOME."/html/img/slots/$slot_id".$ext);
}

function remove_file($id){
 $file = "";
 $valid_exts=Array(".gif", ".jpg", ".png", "");
 foreach($valid_exts as $ext){
  $probe = APPLICATION_HOME.'/html/img/slots/'.$id.$ext;
  if (file_exists($probe)){
   $file = APPLICATION_HOME.'/html/img/slots/'.$id.$ext;
   break;
  }
 }
 if ($file != "")
  unlink($file);
}

function add_slot(){
 global $nodeinfo, $HTTP_POST_VARS, $field_image_file, $field_image_file_name;

 if (!defined("RED_NOTIFICATION_INC"));
  include("messages/red_notification.inc"); 
 
 $slot = validate_and_set($HTTP_POST_VARS);
 $slot_id = add_slot_ex($slot);
 
 copy_file($slot_id);
 
 if (isset($HTTP_POST_VARS['field_notify_users'])){
  $trail1 = get_node_info($slot['trail_1_id']);
  $trail2 = ($slot['trail_2_id']) ?  get_node_info($slot['trail_2_id']) : -1;
 
  send_red_notification(LTMSG_TRAILSLT, $trail1['userid'], $trail1['id'], $slot['node']);
  if ($trail2 != -1)
   send_red_notification(LTMSG_TRAILSLT, $trail2['userid'], $trail2['id'], $slot['node']);
 }
 Header("Location: ".$nodeinfo['path']);
 exit;
}

function go_back(){
 global $kat;
 header("Location: $kat");
}

function edit_slot_form(){
 global $kat, $capabilities, $nodeinfo, $id;
 print_page_begin();
 $tpl = new Template(APPLICATION_HOME."/templates/directory", "keep");
 $tpl->set_file(array("main" => "main.html"));

 $tpl->set_var("SUBNODES", print_slot_form($nodeinfo, $id));
 $tpl->set_var("TRAILS", "");
 $tpl->set_var("IWANTTO", print_iwantto($kat, $capabilities));
 $tpl->set_var("FEATURED", print_featured($kat));
 $tpl->set_var("TOPEX", print_topex($kat));
 $tpl->set_var("NEWTRAILS", print_newtrails($activekat));
 $tpl->set_var("DIDYOUKNOW", "");
 $tpl->set_var("OPENTRAIL", "");
 $tpl->parse("main", "main");
 $tpl->p("main");
 print_page_end();
}

function delete_slot(){
 global $nodeinfo, $HTTP_POST_VARS, $field_image_file, $field_image_file_name, $sess;

 if (!defined("RED_NOTIFICATION_INC"));
  include("messages/red_notification.inc"); 

 
 $slotobj = get_slot_info($HTTP_POST_VARS['id']);
 rm_slot_ex($slotobj, true);
 remove_file($HTTP_POST_VARS['id']);

 //Notify users only if god (admin) wants them to be notified (the default)
 if (isset($HTTP_POST_VARS['field_notify_users'])){
 
  $trail1 = get_node_info($slotobj['trail_1_id']);
  $trail2 = ($slotobj['trail_2_id']) ?  get_node_info($slotobj['trail_2_id']) : -1;
 
  send_red_notification(LTMSG_SLOTTERM, $trail1['userid'], $trail1['id'], $slotobj['node']);
  if ($trail2 != -1)
   send_red_notification(LTMSG_SLOTTERM, $trail2['userid'], $trail2['id'], $slotobj['node']);
 } 
 
 Header("Location: ".$sess->url($nodeinfo['path']));
}

function edit_slot(){
 global $nodeinfo, $HTTP_POST_VARS, $field_image_file, $field_image_file_name, $sess;

 if (!defined("RED_NOTIFICATION_INC"));
  include("messages/red_notification.inc"); 
  
 $slot    = validate_and_set($HTTP_POST_VARS);
 $oldslot = get_slot_info($slot['id']);
 
 edit_slot_ex($slot['id'], $slot);
 if ( isset($HTTP_POST_VARS['row']) and ($HTTP_POST_VARS['row'] != "") )
  reposition_slot($slot, $HTTP_POST_VARS['row']);
 copy_file($slot['id']);

 //Notify users only if god (admin) wants them to be notified (the default)
 if (isset($HTTP_POST_VARS['field_notify_users'])){
 
  $trail1_new = get_node_info($slot['trail_1_id']);
  $trail2_new = ($slot['trail_2_id']) ?  get_node_info($slot['trail_2_id']) : -1;
 
  $trail1_old = get_node_info($oldslot['trail_1_id']);
  $trail2_old = ($oldslot['trail_2_id']) ?  get_node_info($oldslot['trail_2_id']) : -1;

  send_red_notification(LTMSG_SLOTTERM, $trail1_old['userid'], $trail1_old['id'], $slot['node']);
  if ($trail2 != -1)
   send_red_notification(LTMSG_SLOTTERM, $trail2_old['userid'], $trail2_old['id'], $slot['node']);

  send_red_notification(LTMSG_SLOTTERM, $trail1_new['userid'], $trail1_new['id'], $slot['node']);
  if ($trail2 != -1)
   send_red_notification(LTMSG_SLOTTERM, $trail2_new['userid'], $trail2_new['id'], $slot['node']);
 } 
 

 Header("Location: ".$sess->url($nodeinfo['path']));
 exit;
}

function slot_edit_error($errors){
 global $kat, $HTTP_POST_VARS, $capabilities, $nodeinfo, $id;
 
 $slot['id']           = $HTTP_POST_VARS['id'];
 $slot['node']         = $nodeinfo['id'];
 $slot['trail_1_path'] = $HTTP_POST_VARS['field_trail_1'];
 $slot['trail_1_text'] = $HTTP_POST_VARS['field_title_1'];
 $slot['trail_2_path'] = $HTTP_POST_VARS['field_trail_2'];
 $slot['trail_2_text'] = $HTTP_POST_VARS['field_title_2'];
 $slot['description']  = $HTTP_POST_VARS['field_description'];
 
 $errstrs = "<font size=\"2\" face=\"verdana, arial, helvetica\">The following errors occured:<ul>"; 
 foreach($errors as $errstr)
  $errstrs .= "<li>$errstr";
 $errstrs .= "</ul></font>"; 

 print_page_begin();
 $tpl = new Template(APPLICATION_HOME."/templates/directory", "keep");
 $tpl->set_file(array("main" => "main.html"));

 $tpl->set_var("SUBNODES", print_slot_form($nodeinfo, $slot, $errstrs));
 $tpl->set_var("TRAILS", "");
 $tpl->set_var("IWANTTO", print_iwantto($kat, $capabilities));
 $tpl->set_var("FEATURED", print_featured($kat));
 $tpl->set_var("TOPEX", print_topex($kat));
 $tpl->set_var("NEWTRAILS", print_newtrails($activekat));
 $tpl->set_var("DIDYOUKNOW", "");
 $tpl->set_var("OPENTRAIL", "");
 $tpl->parse("main", "main");
 $tpl->p("main");
 print_page_end();
 exit;
}

page_close();
?>