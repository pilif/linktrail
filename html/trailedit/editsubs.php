<?php
function form($reloadparent = false){
 global $mytrail, $ltrstr, $ft, $notification, $sess;
 
 //die($notification); 
 
 switch ($ft) {
     case SFT_CHSUBS:
         $str      = $ltrstr['CHANGE SUBSCRIPTION'];
         $backtext = $ltrstr['Back without unsub'];
         break;
     case SFT_SUBSCRIBE:
         $str      = $ltrstr['SUBSCRIBE'];
         $backtext = $ltrstr['Back without sub'];
         break;
 }
 print(msg_box($str,  print_subscription_form($notification, $ft, $reloadparent), $mytrail, $mytrail['path'], 0, $backtext));
}

function doit(){
 global $HTTP_POST_VARS, $mytrail, $auth, $sess;


 


if ( get_notification_method($auth->auth['uname'], $mytrail['id']) != -1)
 change_subscription($auth->auth['uname'], $mytrail['id'], $newn);
else{
 subscribe($auth->auth['uname'], $mytrail['id'], $newn); 
}
$str="";
if ($HTTP_POST_VARS['reloadparent'] == "1")
$str = "?reloadparent=1";
header("Location: ".$sess->url($mytrail['path'].$str));
}



if (!defined("LAY_TRAIL_INC"))
 include("layout/lay_trail.inc");
if (!defined("EDIT_LINKS_INC"))
 include("dbapi/edit_links.inc");
if (!defined("TRAILFUNCS_INC"))
 include("commonapi/trailfuncs.inc");

if (!defined("COMMON_SUBSCRIPTIONS_INC"))
 include("commonapi/common_subscriptions.inc");
if (!defined("SUBSCRIPTIONS_INC"))
 include("dbapi/subscriptions.inc");
if(!defined("MSG_SUGGESTIONS_INC"))
 include("messages/suggestions.inc");
     
if (!class_exists("Template"))
 include("template.inc");

$in_trail = 3;
page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm", "user" => "Linktrail_User"));
$auth->login_if( $auth->auth["uid"] == "nobody" );

$mytrail = get_node_info($PATH_INFO);
if ($mytrail == -1)
 $mytrail = get_node_info($PATH_INFO."?");
 

if ( get_notification_method($auth->auth['uname'], $mytrail['id']) != -1)
 $ft=SFT_CHSUBS;
else
 $ft=SFT_SUBSCRIBE;  

if ($action == "exec"){
 if (isset($unsubscribe)){
  unsubscribe($auth->auth['uname'], $mytrail['id']);
  $str="";
  if ($HTTP_POST_VARS['reloadparent'] == "1")
  $str = "?reloadparent=1";
 // die("Location: ".$sess->url($mytrail['path'].$str));

  header("Location: ".$sess->url($mytrail['path'].$str));
 }
 else
  doit();
}else{
 form($HTTP_GET_VARS['reload_parent'] == "1");
}           
page_close();
?>
