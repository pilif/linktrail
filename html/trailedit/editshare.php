<?php
function form(){
 global $mytrail, $ltrstr, $auth;

print(msg_box($ltrstr['Share This Trail'], print_share_form($mytrail, $auth->auth['uname']), $mytrail, $mytrail['path'], 0, $ltrstr['Back without recommend']));
}

function send_trail($addresses, $message, $trail){
 global $HTTP_POST_VARS, $auth, $glob_userdata;
 if ($auth->auth['uid'] == "nobody"){
  $from_name    = ($HTTP_POST_VARS['field_name'] != "") ? $HTTP_POST_VARS['field_name'] : "Anonymous Linktrail User";
  $from_address = ($HTTP_POST_VARS['field_email']!= "") ? $HTTP_POST_VARS['field_email']: "info@linktrail.com";
 }else{
  $from_name    = ( ($glob_userdata['FirstName'] != "") or ($glob_userdata['LastName'] != "") ) ? $glob_userdata['FirstName']." ".$glob_userdata['LastName'] : $glob_userdata['Username'];
  $from_address = ($glob_userdata['Email'] != "") ? $glob_userdata['Email'] : "info@linktrail.com";
 }
 $message = "
This message must be extended...

A User on linktrail.com suggests you a LInktrail!

Trail: http://www.linktrail.com$trail

The user leaves a message:

$message
";
/*foreach($addresses as $address)
 mail($address, "Trail-Suggestion", $message,
     "From: $from_name <$from_address>\nX-Mailer: PHP/" . phpversion()); */
}

function doit(){
 global $sess, $HTTP_POST_VARS, $mytrail, $auth;
 
 if (is_array($HTTP_POST_VARS['friends'])){
  if (!defined("MSG_SUGGESTIONS_INC"))
   include("messages/suggestions.inc");
  //This stinks a bit since it causes one DB-Query per friend to be notified
  foreach($HTTP_POST_VARS['friends'] as $friend)
   suggest_trail($auth->auth['uname'], $friend, $mytrail['id'], $HTTP_POST_VARS['field_friendmsg']);
   //create_share_suggestion($mytrail['id'], $auth->auth['uname'], $friend);  
   
 }
 $HTTP_POST_VARS['field_email_addresses'] = str_replace('\r\n', '\n', $HTTP_POST_VARS['field_email_addresses']); //windows
 $HTTP_POST_VARS['field_email_addresses'] = str_replace('\r', '\n', $HTTP_POST_VARS['field_email_addresses']); //m„k
 $email_addresses = explode('\n', $HTTP_POST_VARS['field_email_addresses']); //uniks
 if (is_array($email_addresses))
  send_trail($email_addresses, $HTTP_POST_VARS['field_message'], $mytrail['path']);
header("Location: ".$sess->url($mytrail['path']));
}

$in_trail = 1;
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
if (!defined("PERMISSIONS_INC"))
 include("dbapi/permissions.inc");
if (!defined("SHARE_INC"))
 include("dbapi/share.inc");

page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm", "user" => "Linktrail_User"));
$nobody = ( ($auth->auth["uid"] == "nobody") or ($auth->auth["uid"] == "") or ($auth->auth["uid"] == "form"));

$mytrail = get_node_info($PATH_INFO);
if ($mytrail == -1)
 $mytrail = get_node_info($PATH_INFO."?");

if (!class_exists("Template")) 
 include("template.inc");

if ($REQUEST_METHOD == "POST"){
 doit();
}else{
 form();
}           

page_close();
?>
