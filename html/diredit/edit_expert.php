<?php
page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm", "user" => "Linktrail_User"));
if (!defined("COMMON_PERMISSIONS_INC"))
 include("commonapi/common_permissions.inc");
if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");

if (!defined("LAY_DIRECTORY_INC"))
 include("layout/lay_directory.inc");

if (!defined("LAY_DIREDIT_INC"))
 include("layout/lay_diredit.inc");

if (!defined("LAY_SLOTS_INC"))
 include("layout/lay_slots.inc");

if (!defined("DISPLAY_DIRECTORY_INC"))
 include("application/display_directory.inc"); 

$capabilities = get_caps($perm, $kat); 
$kat = $PATH_INFO;
$nodeinfo = get_node_info($kat);

//push back evil guys...
if ( (!has_caps($capabilities, CAP_EDIT_EXPERTS)) or ($REQUEST_METHOD != "POST") or ($nodeinfo == -1) )
 go_back();



if (isset($cancel))
 go_back();
else
 doit();

function go_back(){
 global $sess, $PATH_INFO;

 page_close();
 Header("Location: ".$sess->url($PATH_INFO));
 exit;
} 

function print_error($errstr, $predef){
 $exp_error['predef']   = $predef;
 $exp_error['errorstr'] = $errstr;
 display_directory($exp_error);
 page_close();
 exit;
}

function doit(){
 global $HTTP_POST_VARS, $nodeinfo;
 
 if (!defined("RED_NOTIFICATION_INC"));
  include("messages/red_notification.inc"); 
 if (!defined("USER_INC"));
  include("dbapi/user.inc"); 
  
 
 $errstr = '<ul>';
 if ($HTTP_POST_VARS['expert_type'] == 'auto'){
  delete_expert($nodeinfo['id']);
  if ($HTTP_POST_VARS['old_expert'] != ""){
   $user = get_user_from_name($HTTP_POST_VARS['old_expert']);
   if ($user != -1)
    if (isset($HTTP_POST_VARS['field_notify_user']))
     send_red_notification(LTMSG_FETERMN, $user['User_ID'], $nodeinfo['id']);
  }
 }else{
  if ($HTTP_POST_VARS['field_expert'] == ""){
   $errstr .= '<li>You have to enter an expert';
  }else{
   if (!is_valid_expert($HTTP_POST_VARS['field_expert']))
    $errstr .= '<li>The expert you specified could not be found in the database.';    
  }
  if($HTTP_POST_VARS['field_description'] == "")
    $errstr .= '<li>Please enter a descripiton for your expert.';     

  if ($errstr != '<ul>'){
   $predef['Name']  = $HTTP_POST_VARS['field_expert'];
   $predef['About'] = $HTTP_POST_VARS['field_description'];
   print_error($errstr.'</ul>', $predef);
  }

  $currexp = get_def_expert($nodeinfo['id']);
  $newexp_id = get_user_from_name($HTTP_POST_VARS['field_expert']);
  $newexp_id = $newexp_id['User_ID'];
  
  if (is_array($currexp)){
   edit_expert($nodeinfo['id'], $HTTP_POST_VARS['field_expert'], $HTTP_POST_VARS['field_description']);
   
   if ($HTTP_POST_VARS['old_expert'] != ""){
    $user = get_user_from_name($HTTP_POST_VARS['old_expert']);
    if ($user != -1)
     if (isset($HTTP_POST_VARS['field_notify_user']))
      send_red_notification(LTMSG_FETERMN, $user['User_ID'], $nodeinfo['id']);
   }
   if (isset($HTTP_POST_VARS['field_notify_user']))
    send_red_notification(LTMSG_FEELECT, $newexp_id, $nodeinfo['id']);
  }else{
   add_expert($nodeinfo['id'], $HTTP_POST_VARS['field_expert'], $HTTP_POST_VARS['field_description']);
   if (isset($HTTP_POST_VARS['field_notify_user']))
    send_red_notification(LTMSG_FEELECT, $newexp_id, $nodeinfo['id']);
  }
 }
 go_back();
}

?>