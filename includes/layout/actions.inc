<?php
define("ACTIONS_INC", true);
/*
Below the line 'I want to' in the Trail-Window are listed some actions.
The users' state in the directory defines which actions are listed.

In this file, you define the "look" 

This file gets the Strings used from <language>.inc                                 
*/
if (!defined("COMMON_PERMISSIONS_INC"))
 include("commonapi/common_permissions.inc");

if (!defined("SUBSCRIPTIONS_INC"))
 include("dbapi/subscriptions.inc");
 
if (!defined("COMMON_SUBSCRIPTIONS_INC"))
 include("commonapi/common_subscriptions.inc");


define("ACTION_PERMS", "\n".'<li><a href="[URL]" target="_self" class="hi" title="Change who may edit this trail">'.$ltrstr["Change Permissions"].'</a>');
define("ACTION_BACK",  "\n".'<li><a href="[URL]" class="hi" title="go back to the linktrail directory">'.$ltrstr["Go back to linktrail"].'</a>');

define("ACTION_CSUBS"    , "\n".'<li><a href="[URL]" target="_self" class="hi">'.$ltrstr["Change Subscription"].'</a>');
define("ACTION_SUBSCRIBE", "\n".'<li><a href="[URL]" target="_self" class="hi" title="Subscribe this Trail to find it again">'.$ltrstr["Subscribe"].'</a>');
//define("ACTION_CNOTI"    , "\n".'<li><a href="[URL]" target="_self" class="hi" title="Change how i would like the be notified about changes">'.$ltrstr["Change Notification"].'</a>');

define("ACTION_RECOM", "\n".'<li><a href="[URL]" target="_self" class="hi" title="Recommend this Trail to a friend">'.$ltrstr["Recommend this trail"].'</a>');
define("ACTION_ADD"  , "\n".'<li><a href="[URL]" target="_self" class="stat" title="Add a link to this trail.">'.$ltrstr["Add a link"].'</a>');
define("ACTION_MYPAG", "\n".'<li><a href="[URL]" class="hi">'.$ltrstr["Go to the Creators page"].'</a>');
define("ACTION_MYPAG_MY", "\n".'<li><a href="[URL]" class="hi">'.$ltrstr["Go to my Mypage"].'</a>');

define("ACTION_LOGIN", "\n".'<li><a href="[URL]" target="_self" class="hi">'.$ltrstr['Login'].'</a>');
define("ACTION_SHARE", "\n".'<li><a href="[URL]" target="_self" class="hi">'.$ltrstr["Share this trail"].'</a>');

define("ACTION_CONTACT", "\n".'<li><a href="[URL]" target="_self" class="hi">'.$ltrstr['Contact the author'].'</a>');


define("ACTION_GO", "\n".'<li>'.$ltrstr['Go to'].' [[DIRECTORY]][[CREATOR]][[MYPAGE]]');

define("GO_DIR", '<a href="[URL]" class="hi">'.$ltrstr['directory'].'</a>');
define("GO_CRE", '|<a href="[URL]" class="hi">'.$ltrstr['Creator'].'</a>');
define("GO_MYP", '|<a href="[URL]" class="hi">'.$ltrstr['mypage'].'</a>');


function build_action_list($nodeinfo, $perms, $username){
 global $auth, $sess;
 
 $actions = "";
 if (has_caps($perms, PERM_ADD))
  $actions.=str_replace("[URL]", "#addlinks", ACTION_ADD);
 if (has_caps($perms, PERM_CHANGE_PERMS) )
  $actions.=str_replace("[URL]", $sess->url('/trailedit/editperms.php'.build_good_url($nodeinfo['path'])), ACTION_PERMS);
 if ($auth->auth['uid'] != "nobody"){
  if (get_notification_method($username, $nodeinfo['id']) != -1)
   $actions.=str_replace("[URL]", $sess->url("/trailedit/editsubs.php".build_good_url($nodeinfo['path'])."?ft=".SFT_CHSUBS), ACTION_CSUBS);
  else
   $actions.=str_replace("[URL]", $sess->url("/trailedit/editsubs.php".build_good_url($nodeinfo['path'])."?ft=".SFT_SUBSCRIBE), ACTION_SUBSCRIBE);  
 }
 if ($auth->auth['uid'] == "nobody"){
  $actions .= str_replace("[URL]", $sess->url(build_good_url($nodeinfo['path'])."?dologin=1"), ACTION_LOGIN);
 }else{
  if ($auth->auth['uid'] != $nodeinfo['userid']){
   $str = sprintf(ACTION_CONTACT, $nodeinfo['username']);
   $actions .= str_replace("[URL]", $sess->url("/trailedit/lms_send.php".build_good_url($nodeinfo['path'])), $str);
  }
  $friends = get_friends($auth->auth['uid'], FT_ALL);
  if (is_array($friends))
   get_allowed_users($trail_id, $friends);
  else
   $friends = Array();
  if(count($friends) > 0)
   $actions .= str_replace("[URL]",  $sess->url("/trailedit/editshare.php".build_good_url($nodeinfo['path'])), ACTION_SHARE);
 }
 
 $actions .= create_go_action($username, $nodeinfo['username'], build_good_url(parent_path($nodeinfo['path'])));
 return $actions; 
}

function create_go_action($user, $owner, $parent_path){
 global $sess;
 
 $itsmine = ($user == $owner);
 $action['directory'] = str_replace("[URL]", $sess->url($parent_path), GO_DIR);
 if (!$itsmine)
  $action['creator'] = str_replace("[URL]", $sess->url('/Experts/'.rawurlencode($owner)), GO_CRE);
 else
  $action['creator'] = "";
 if ($user != "")
  $action['mypage'] = str_replace("[URL]", $sess->url("/Experts/".rawurlencode($user)), GO_MYP);
 else
  $action['mypage'] = "";
 $res = ACTION_GO;
 $res = str_replace("[[DIRECTORY]]", $action['directory'], $res);
 $res = str_replace("[[CREATOR]]", $action['creator'], $res);
 $res = str_replace("[[MYPAGE]]", $action['mypage'], $res);
 return $res;
}
?>