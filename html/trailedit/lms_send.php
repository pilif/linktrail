<?php
function form($error = false){
 global $mytrail, $ltrstr, $sess;

print(msg_box($ltrstr['Send Message'], print_send_message_form($error), $mytrail, $mytrail['path'], 0, $ltrstr['Go Back Message']));
}

function doit(){
 global $sess, $HTTP_POST_VARS, $mytrail, $auth;
 
 if ($HTTP_POST_VARS['field_subject'] == ""){
  form(true);
  page_close();
  exit;
 }
 
 if(!defined("TALK_INC"))
  include("messages/talk.inc");
  
talk($auth->auth['uname'], $mytrail['userid'], $HTTP_POST_VARS['field_subject'], $HTTP_POST_VARS['field_message'], $mytrail['id']);
 
header("Location: ".$sess->url($mytrail['path']));
}


if (!defined("LAY_TRAIL_INC"))
 include("layout/lay_trail.inc");
/*
 First I read the permissions of our user. 
*/

page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm", "user" => "Linktrail_User"));

$mytrail = get_node_info($PATH_INFO);
if ($mytrail == -1)
 $mytrail = get_node_info($PATH_INFO."?");

include("template.inc");

if ($REQUEST_METHOD == "POST"){
 doit();
}else{
 form();
}

page_close();
?>
