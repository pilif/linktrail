<?php
define("LAY_MYPAGE_INC", true);

if (!defined("DIRECTORY_INC"))
 include("dbapi/directory.inc");
 
if (!defined("BUTTONS_INC"))
 include("layout/buttons.inc");

if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");

 if (!defined("LAY_DIRECTORY_INC"))
 include("layout/lay_directory.inc");

 
if (!defined("COMMON_PERMISSIONS_INC"))
 include("commonapi/common_permissions.inc"); 

if (!defined("PERMISSIONS_INC"))
 include("dbapi/permissions.inc"); 

function print_exp_error($expert){
 $tpl = new Template(APPLICATION_HOME."/templates/directory", "keep");
 $tpl->set_file(array("error" => "err_noexp.html"));
 $tpl->set_var("EXPERT", $expert);
 $tpl->parse("main", "error");
 return $tpl->get("main");
}

 
function print_mypage_trail($nodeinfo, $expert, $color=BACK_COLOR1, $itsme=false, $updated=false){
 global $HTTP_GET_VARS, $glob_userdata, $ltrstr, $sess, $randval, $glob_language, $language_host;
 
 $dh = friendlydate($nodeinfo['adddate']);
 $date = $dh['month'].'/'.$dh['day'].'/'.$dh['year']; 

 $tpl = new Template(APPLICATION_HOME."/templates/mypage", "keep");

 if ($nodeinfo['suggested_from'] == ""){
//  die("Test: ".$nodeinfo['suggested_from']);
  $tpl->set_file(array("main" => "trail.html"));
  if ($nodeinfo['subscribed'])
   $tpl->set_var("FLAG_SUBSCRIBED", print_flag_subscribed($nodeinfo['path'], $itsme));
  else
   $tpl->set_var("FLAG_SUBSCRIBED","&nbsp;");
 }else{
  if (!$itsme)
   return "";
  $tpl->set_file(array("main" => "trail_suggested.html"));
  $tpl->set_var("FRIENDURL", $sess->url('/Experts/'.rawurlencode($nodeinfo['suggested_from'])));
  $tpl->set_var("FRIENDNAME", $nodeinfo['suggested_from']);
  $actionstring= "action=subscribe&trail=".$nodeinfo['id']."&expert=$expert&ob=".$HTTP_GET_VARS['ob']."&ci=".$HTTP_GET_VARS['ci'];
  $actionstring= base64_encode($actionstring);
  $tpl->set_var("YESURL", $sess->url("/diredit/dosuggestion.php?actionstring=".$actionstring));
  $actionstring= "action=cancel&trail=".$nodeinfo['id']."&expert=$expert&ob=".$HTTP_GET_VARS['ob']."&ci=".$HTTP_GET_VARS['ci'];
  $actionstring= base64_encode($actionstring);
  $tpl->set_var("NOURL", $sess->url("/diredit/dosuggestion.php?actionstring=".$actionstring));
 } 
 if ($glob_language != $nodeinfo['language']){
  $tpl->set_var("LMARK", $ltrstr['Languages Short'][$nodeinfo['language']]);
  $url = 'http://'.$language_host[$nodeinfo['language']].$sess->url(build_good_url($nodeinfo['path']));
 }else{
  $url = $sess->url(build_good_url($nodeinfo['path']));
  $tpl->set_var("LMARK", "");
 }
  
 $tpl->set_var("TRAILURL", $url);
 $tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));
 $tpl->set_var("COLOR", $color);
// $tpl->set_var("TRAILURL", "JavaScript: openIt('".addslashes(build_good_url($nodeinfo['path']))."'); return false;" );
 $tpl->set_var("TRAILNAME", str_replace("_", " ", friendlyname($nodeinfo['path'])));
 $tpl->set_var("NODENAME", str_replace("_", " ", friendlyname(parent_path($nodeinfo['path']))));
 $tpl->set_var("NODEURL", $sess->url(parent_path($nodeinfo['path'])));
 $tpl->set_var("CHANGEDATE", $date);
 
 $cdate = unixdate(friendlydate($nodeinfo['changedate']));
 $adate = unixdate(friendlydate($nodeinfo['adddate']));
 $udate = unixdate(friendlydate($glob_userdata['LastOnline']));
 $str = "";
 if ($udate != -1){
  if ($cdate > $udate)
   $str = $ltrstr['Updated'];
  if ($adate >= $udate) //added has priority before changed
   $str = $ltrstr['New'];
  if ($str != "")
   $str = str_replace('[FLAG]', $str, DFLAG_FORMAT);
 }
 if ($str == "") $str = "&nbsp;";
 $tpl->set_var("FLAG_UPDATED", $str);

 $tpl->parse("main", "main");
 return $tpl->get("main");
}

function print_friendlist_awaiting($friends, $uid, $expert_data){
 global $auth, $sess;
 
 if (!is_array($friends))
  return "";
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/friendlist", "keep");
 $tpl->set_file(array("awaiting" => "awaiting.html"));

 $tpl->set_block("awaiting", "waitingfriend", "waitingfriends");
 # define NUM and BIGNUM, then append "row" to "rows"...

 for($x=0; $x < count($friends); $x++) {
   $tpl->set_var("WFRIENDURL", $sess->url("/Experts/".rawurlencode($friends[$x]['username'])));
   $tpl->set_var("WFRIENDNAME", $friends[$x]['username']);
   $tpl->set_var("YESURL", $sess->url(sprintf("/diredit/dofriend.php?expert=%s&target=%s&action=comfirm", urlencode($expert_data['User_ID']), urlencode($friends[$x]['userid']))));
   $tpl->set_var("NOURL", $sess->url(sprintf("/diredit/dofriend.php?expert=%s&target=%s&action=decline", urlencode($expert_data['User_ID']), urlencode($friends[$x]['userid']))));
   $tpl->parse("waitingfriends", "waitingfriend", true);
 }
 $tpl->parse("main", "awaiting");
 return $tpl->get("main");
}

function print_friendlist_awaited($friends, $myuid, $expert_data){
 global $sess;
 if (!is_array($friends))
  return "";

 $tpl = new Template(APPLICATION_HOME."/templates/mypage/friendlist", "keep");
 $tpl->set_file(array("awaited" => "awaited.html"));

 $tpl->set_block("awaited", "afriend", "awaitedfriends");
 # define NUM and BIGNUM, then append "row" to "rows"...
 for($x=0; $x < count($friends); $x++) {
   $tpl->set_var("AFRIENDURL", "/Experts/".rawurlencode($friends[$x]['username']));
   $tpl->set_var("AFRIENDNAME", $friends[$x]['username']);
   $tpl->set_var("DELURL", $sess->url(sprintf("/diredit/dofriend.php?expert=%s&target=%s&action=cancel", urlencode($expert_data['User_ID']), urlencode($friends[$x]['userid'])))); //$sess->url("/diredit/dofriend.php?actionstring=$actionstring"));
   $tpl->parse("awaitedfriends", "afriend", true);
 }
 $tpl->parse("main", "awaited");
 return $tpl->get("main");
}

function print_friendlist_all($friends, $myuid, $myfriends, $expert_data){
 global $auth, $sess;
 if (!is_array($friends))
  return "";

 $tpl = new Template(APPLICATION_HOME."/templates/mypage/friendlist", "keep");
 if ($myfriends)
  $tpl->set_file(array("all" => "my_friends.html"));
 else
  $tpl->set_file(array("all" => "friends.html"));

 $tpl->set_block("all", "friend", "friends");
 # define NUM and BIGNUM, then append "row" to "rows"...
 for($x=0; $x < count($friends); $x++) {
   $tpl->set_var("FRIENDURL", $sess->url("/Experts/". rawurlencode($friends[$x]['username'])));
   $tpl->set_var("FRIENDNAME", $friends[$x]['username']);
   if ($myfriends){
    $actionstring = sprintf("uid1=%s&uid2=%s&expert=%s&action=delete",$myuid, $friends[$x]['userid'], $expert);
    $actionstring = base64_encode($actionstring);
    $tpl->set_var("DELURL", $sess->url(sprintf("/diredit/dofriend.php?expert=%s&target=%s&action=quit", urlencode($expert_data['User_ID']), urlencode($friends[$x]['userid']))));
   }
   $tpl->parse("friends", "friend", true);
 }
 $tpl->parse("main", "all");
 return $tpl->get("main");
}



function print_mypage_friendlist($userid, $expert_data, $itsme){
 global $sess;
 $tpl = new Template(APPLICATION_HOME."/templates/mypage", "keep");
 
 $tpl->set_file(array("friendlist" => "friendlist.html"));
 if($itsme){
    $friends = get_friends($userid, FT_WAITING);
    $tpl->set_var("AWAITING", print_friendlist_awaiting($friends, $userid, $expert_data));
    $friends = get_friends($userid, FT_AWAITED);
     
    $tpl->set_var("AWAITED", print_friendlist_awaited($friends, $userid, $expert_data));
 }else{
    $tpl->set_var("AWAITING", "");
    $tpl->set_var("AWAITED", "");
 }
 $friends = get_friends($userid, FT_ALL);
 $tpl->set_var("FRIENDS", print_friendlist_all($friends, $userid, $itsme, $expert_data));
 $tpl->parse("main", "friendlist");
 return $tpl->get("main");
}

function print_flag_subscribed($path, $itsme){
 global $sess, $randval, $glob_userdata;
 
 $tpl = new Template(APPLICATION_HOME."/templates/mypage", "keep");
 if ($itsme){
  $tpl->set_file(array("flag" => "flag_subscribed_my.html"));
  $tpl->set_var("SUBSCRIBEDURL",$sess->url( "/trailedit/editsubs.php".build_good_url($path)."?reload_parent=1"));
  $tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));
  $tpl->set_var("FULLPATH", $path);
 }else{
  $tpl->set_file(array("flag" => "flag_subscribed.html"));
 }
   
 $tpl->parse("main", "flag");
 return $tpl->get("main");
}

function print_mypage_t4u($t4u){
 global $sess, $glob_userdata;
 
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/", "keep");
 $tpl->set_file(array("name" => "t4u.html"));
 $tpl->set_var("T4U_URL", $sess->url(build_good_url($t4u['path'])));
 $tpl->set_var("T4U_TITLE", friendlyname($t4u['path']));
 $tpl->set_var("T4U_DESC", $t4u['description']);
 $tpl->set_var("T4U_TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222)); 
 $tpl->parse("main", "name");
 return $tpl->get("main");
}
function print_mypage_name($first, $last){
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/profile", "keep");
 $tpl->set_file(array("name" => "name.html"));
 $tpl->set_var("FIRSTNAME", $first);
 $tpl->set_var("LASTNAME", $last);
 $tpl->parse("main", "name");
 return $tpl->get("main");
}

/*function print_mypage_msn($name, $provider){
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/profile", "keep");
 $tpl->set_file(array("msn" => "msn.html"));
 $tpl->set_var("MSN_ID", $name);
 $tpl->set_var("MSN_SERVICE", $provider);
 $tpl->parse("main", "msn");
 return $tpl->get("main");
}*/

function print_mypage_gender($gender){
 global $ltrstr;
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/profile", "keep");
 $tpl->set_file(array("gender" => "gender.html"));
 $tpl->set_var("GENDER", $ltrstr["gender_".$gender]);
 $tpl->parse("main", "gender");
 return $tpl->get("main");
}

function print_mypage_common($filename, $template_name, $content){
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/profile", "keep");
 $tpl->set_file(array("common" => "$filename"));
 $tpl->set_var($template_name, $content);
 $tpl->parse("main", "common");
 return $tpl->get("main");
}

/*function print_mypage_favourites($url_array){
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/profile", "keep");
 $tpl->set_file(array("favourites" => "favourites.html"));
 $str = "";
 foreach($url_array as $url)
  if ($url != "")
   $str .= '<a href="'.$url.'">'.$url.'</a>'."<br>\n";
 $tpl->set_var("URLS", $str);
 $tpl->parse("main", "favourites");
 return $tpl->get("main");
}*/

/*function print_iwantto_changeprofile($expert){
 global $sess;
 
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/iwantto", "keep");
 $tpl->set_file(array("iwantto" => "edit_profile.html"));

 $tpl->set_var("ACTIONURL", $sess->url("/Experts/".rawurlencode($expert)."/Change_Profile"));
 
 $tpl->parse("iwantto", "iwantto");
 return $tpl->get("iwantto");
}*/

function print_iwantto_contactauthor($expert){
 global $sess;
 
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/iwantto", "keep");
 $tpl->set_file(array("iwantto" => "contact_author.html"));

 $tpl->set_var("ACTIONURL", $sess->url("/Experts/".rawurlencode($expert)."/Send_Message"));
 $tpl->set_var("USERNAME", $expert);
 $tpl->parse("iwantto", "iwantto");
 return $tpl->get("iwantto");
}


/*function print_iwantto_changepwd($expert){
 global $sess;
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/iwantto", "keep");
 $tpl->set_file(array("iwantto" => "edit_pass.html"));

 $tpl->set_var("ACTIONURL", $sess->url("/Experts/".rawurlencode($expert)."/Change_Security"));
 
 $tpl->parse("iwantto", "iwantto");
 return $tpl->get("iwantto");
}*/

function print_iwantto_changeprefs($expert){
 global $sess;
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/iwantto", "keep");
 $tpl->set_file(array("iwantto" => "edit_prefs.html"));

 $tpl->set_var("ACTIONURL", $sess->url("/Experts/".rawurlencode($expert)."/Settings/"));
 
 $tpl->parse("iwantto", "iwantto");
 return $tpl->get("iwantto");
}


function print_iwantto_friendstuff($target_user, $target_uid, $type){
 global $auth, $sess;
 
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/iwantto", "keep");
 $tpl->set_file(array("iwantto" => $type."_friendship.html"));
 $tpl->set_var("USERNAME", $target_user);
 if ($type == "make")
  $str = $sess->url(build_good_url("/Experts/$target_user/Make_friendship"));
 else{
  $str = $sess->url(sprintf("/diredit/dofriend.php?expert=%s&target=%s&action=%s", urlencode($auth->auth['uid']), urlencode($target_uid), $type));
 }
 $tpl->set_var("ACTIONURL", $str);
 $tpl->parse("iwantto", "iwantto");
 return $tpl->get("iwantto"); 
}

/*
function print_iwantto_make_friendship($uid, $expert){
 global $auth, $sess;
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/iwantto", "keep");
 $tpl->set_file(array("iwantto" => "make_friendship.html"));
 $tpl->set_var("ACTIONURL", $sess->url(build_good_url("/Experts/$expert/Make_friendship")));
 $tpl->parse("iwantto", "iwantto");
 return $tpl->get("iwantto");
}

function print_iwantto_quit_friendship($uid, $expert){
 global $auth, $sess;
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/iwantto", "keep");
 $tpl->set_file(array("iwantto" => "quit_friendship.html"));
 $actionstring = sprintf("uid1=%s&uid2=%s&expert=%s&action=delete", $auth->auth['uid'], $uid, $expert);
 $actionstring = base64_encode($actionstring);
 $tpl->set_var("ACTIONURL", $sess->url("/diredit/dofriend.php?actionstring=$actionstring"));
 $tpl->set_var("USERNAME", $expert);
 
 $tpl->parse("iwantto", "iwantto");
 return $tpl->get("iwantto");
}
*/

/*function print_mypage_iwantto($kat, $capabilities){
 if (!defined("IWANTTO_INC"))
  include("commonapi/iwantto.inc");
 return print_navigation($kat).print_actions($kat);
}*/

function print_mypage_iwantto_old($capabilities, &$expert_data){
 global $iwantto_mypage_order, $auth;
 
 $expert = $expert_data['Username'];
 
 $tpl = new Template(APPLICATION_HOME."/templates/mypage", "keep");
 $tpl->set_file(array("iwantto" => "iwantto.html"));


/* if ( (has_caps($capabilities, CAP_CHANGE_USER_PREFS)) and ($expert == $auth->auth['uname']) and ($auth->auth['uid'] != "nobody") )
  $capsgui['changeprofile'] = print_iwantto_changeprofile($expert); */

/* if ( (has_caps($capabilities, CAP_CHANGE_USER_PREFS)) and ($expert == $auth->auth['uname']) and ($auth->auth['uid'] != "nobody") )
  $capsgui['changepwd'] = print_iwantto_changepwd($expert); */

 if ( (has_caps($capabilities, CAP_CHANGE_USER_PREFS)) and ($expert == $auth->auth['uname']) and ($auth->auth['uid'] != "nobody") )
  $capsgui['changeprefs'] = print_iwantto_changeprefs($expert);

 if ( has_caps($capabilities, CAP_VIEW_MYPAGE) )
  $capsgui['mypage'] = print_iwantto_mypage();

 if (($expert != $auth->auth['uname']) and ($auth->auth['uid'] != "nobody")){
  $res = is_friendship($expert_data['User_ID'], $auth->auth['uid']);
  if ($res === true){
   $capsgui['friendship'] = print_iwantto_friendstuff($expert_data['Username'], $expert_data['User_ID'], "quit");
  }elseif ($res === false){
   $capsgui['friendship'] = print_iwantto_friendstuff($expert_data['Username'], $expert_data['User_ID'], "make");
  }else{
   if ($res['friend_requesting'] == $expert_data['User_ID'] ){
    $capsgui['friendship'] = print_iwantto_friendstuff($expert_data['Username'], $expert_data['User_ID'], "comfirm");
   }else{
    $capsgui['friendship'] = print_iwantto_friendstuff($expert_data['Username'], $expert_data['User_ID'], "cancel");
   }
  }
 }
 
/* if ( ($expert != $auth->auth['uname']) and ($auth->auth['uid'] != "nobody") and (is_friendship($userdata['User_ID'], $auth->auth['uid']) === false))
  $capsgui['friendship'] = print_iwantto_friendship($userdata['User_ID'], $expert);

 if ( ($expert != $auth->auth['uname']) and ($auth->auth['uid'] != "nobody") and (is_friendship($userdata['User_ID'], $auth->auth['uid']) !== false))
  $capsgui['friendship'] = print_iwantto_cancel_friendship($userdata['User_ID'], $expert); */
 
  if (has_caps($capabilities, CAP_LOGON))
   $capsgui['logon'] = print_iwantto_login();

 if (has_caps($capabilities, CAP_DOTOUR))
   $capsgui['dotour'] = print_iwantto_tour();

 if (has_caps($capabilities, CAP_REGISTER))
   $capsgui['register'] = print_iwantto_member();  
 
  if (has_caps($capabilities, CAP_REPORT_BUG))
   $capsgui['bugreport'] = print_iwantto_report_bug();  

  if (has_caps($capabilities, CAP_CONTACT_AUTHOR))
   $capsgui['contactauthor'] = print_iwantto_contactauthor($expert);  

  if (has_caps($capabilities, CAP_VIEW_MESSAGES))
   $capsgui['readmsg'] = print_iwantto_readmsgs();  
   
   
 # extract the block named "iwanttoitem" from "box", creating a
 # reference to {rows} in "box".
 $tpl->set_block("iwantto", "iwanttoitem", "iwanttos");
 # define NUM and BIGNUM, then append "row" to "rows"...
 $count = 0;
 foreach ($iwantto_mypage_order as $item) {
  if ( isset($capsgui[$item]) ){
   $tpl->set_var("ITEM", $capsgui[$item]);
   $tpl->parse("iwanttos", "iwanttoitem", true);
   $count++;
  } 
 }
 if ($count == 0){
   $tpl->set_var("ITEM", "");
   $tpl->parse("iwanttos", "iwanttoitem", true);
 }
  
 $tpl->parse("iwantto", "iwantto");

 return $tpl->get("iwantto");

}
?>