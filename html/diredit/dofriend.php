<?php
if (!defined("COMMON_PERMISSIONS_INC"))
 include("commonapi/common_permissions.inc");
if (!defined("PERMISSIONS_INC"))
 include("dbapi/permissions.inc");
if (!defined("USER_INC"))
 include("dbapi/user.inc");
if (!defined("MSG_FRIENDSHIPS_INC"))
  include("messages/friendships.inc"); 
if (!defined("MESSAGES_INC"))
  include("messages/friendships.inc"); 

page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm", "user" => "Linktrail_User"));

//Some funny guy with good eyes might try to call this file directely with his own
//userid (I'm asking me, where he should get it) and the userid of his friend (I am even
//more asking me how to get it) for comfirming the request/cancelin a frienddship.
//Let's kick him back!
$capabilities = get_caps($perm, "/");
if (($HTTP_GET_VARS['expert'] != $auth->auth['uid']) and (has_caps($capabilities, CAP_SUPERUSER)) ){
 page_close();
 Header("Location: ".$sess->url("/Experts/".rawurlencode($expert)));
 exit;
}

if ( ($HTTP_GET_VARS['expert'] == "") or ($HTTP_GET_VARS['target'] == "")){
 page_close();
 Header("Location: ".$sess->url("/Experts/".rawurlencode($expert)));
 exit;
} 

$uname    = uid2name($HTTP_GET_VARS['expert']);
$hisname = uid2name($HTTP_GET_VARS['target']);

/*foreach($HTTP_GET_VARS as $key => $value)
 echo("$key = $value<br>");
 
die("test");*/
        
switch ($HTTP_GET_VARS['action']) {
    case "comfirm":
        //echo("blepp");
        delete_friendship_request($HTTP_GET_VARS['expert'], $HTTP_GET_VARS['target']);
        create_friendship($HTTP_GET_VARS['expert'], $HTTP_GET_VARS['target']);
        friendship_notification($uname, $HTTP_GET_VARS['target'], "accepted");
        friendship_notification($uname, $HTTP_GET_VARS['expert'], "accepted_self", $hisname);
        break;
    case "decline":
        delete_friendship_request($HTTP_GET_VARS['expert'], $HTTP_GET_VARS['target']);
        friendship_notification($uname, $HTTP_GET_VARS['target'], "ignore");
        friendship_notification($uname, $HTTP_GET_VARS['expert'], "ignore_self", $hisname);
        break;
    case "quit":
        remove_friendship($HTTP_GET_VARS['expert'], $HTTP_GET_VARS['target']);
        friendship_notification($uname, $HTTP_GET_VARS['target'], "term");
        friendship_notification($uname, $HTTP_GET_VARS['expert'], "term_self", $hisname);
        break;
    case "cancel":
        delete_friendship_request($HTTP_GET_VARS['expert'], $HTTP_GET_VARS['target']);
        friendship_notification($uname, $HTTP_GET_VARS['target'], "cancel");
        friendship_notification($uname, $HTTP_GET_VARS['expert'], "cancel_self", $hisname);
        break;
}

 
page_close();
Header("Location: ".$sess->url("/Experts/".rawurlencode($uname)));
?>
