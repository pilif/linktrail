<?php

function print_mypage_profile_item(&$tpl, $showname, $filename, $sub_templatename, $dbname, $maintemplatename){
 global $friend, $itsme, $userdata;
 
 eval('$text = ( ((show_var($userdata["ShowFriend"], SHOW_'.$showname.') and $friend) or ( show_var($userdata["ShowAnyone"], SHOW_'.$showname.'))) and ($userdata["$dbname"] != ""))  ? print_mypage_common("$filename", "$sub_templatename", $userdata["$dbname"]) : "";');
 $tpl->set_var("$maintemplatename", $text);


}

 define("DISPLAY_DIRECTORY_INC", true);
 
function display_mypage($expert){
 global $kat, $auth, $nobody, $PHP_SELF, $capabilities, $userdata, $friend, $itsme, $sess, $HTTP_GET_VARS, $phplib_user, $user_ob, $sess_ci;
// echo("Expert: $expert; username: ".$auth->auth['uname']);
 
 if (!defined("USER_INC"))
  include("dbapi/user.inc");

if (!defined("PERMISSIONS_INC"))
  include("dbapi/permissions.inc");

if (!defined("TMYPAGE_INC"))
  include("dbapi/tmypage.inc");
  
 if (!defined("LAY_MYPAGE_INC"))
  include("layout/lay_mypage.inc");

 if (!defined("LAY_DIRECTORY_INC"))
  include("layout/lay_directory.inc");
  
if (!defined("COMMON_USER_INC"))
  include("commonapi/common_user.inc");
 
if  (!defined("TRAILFORYOU_INC"))
  include("dbapi/trailforyou.inc");
  
 $userdata = get_user_from_name($expert);
 if ($userdata == -1){
  page_close();
  Header("Location: /?");
  exit;
 }
 
 if (isset($HTTP_GET_VARS['ci'])){
  $ci = $HTTP_GET_VARS['ci'];
  if ($ci == "all") $ci = -1;
  $current_index = intval($ci);
 }elseif(isset($sess_ci)){
  $current_index = $sess_ci;
 }else{
  $current_index = 0;
 }
 
 $sess_ci = $current_index;
 $sess->register("sess_ci");

 
 $ot = "ASC";
 if (isset($HTTP_GET_VARS['ob']))
  $ob = $HTTP_GET_VARS['ob'];
 elseif(isset($user_ob))
  $ob = $user_ob;
 else
  $ob = "";
 
 $user_ob = $ob;
 $GLOBALS['user']->register("user_ob");
 
 $sortselect['TITL'] = false;
 $sortselect['NODE'] = false;
 $sortselect['DATE'] = false;
 $sortselect['SUBS'] = false;
 $sortselect['ACTU'] = false;
 
 
 if ($ob == "t"){
  $sortselect['TITL'] = true;
  $ob = "friendlyname_trail";
 }elseif ($ob == "c"){
  $sortselect['NODE'] = true;
  $ob = "friendlyname_parent";
 }elseif ($ob == "d"){
  $ob = "AddDate";
  $ot = "DESC";
  $sortselect['DATE'] = true;
 }elseif ($ob == "a"){
  $ob = "ChangeDate";
  $ot = "DESC";
  $sortselect['ACTU'] = true;
 }elseif ($ob == "s"){
  $ob = "SubscribedToTrail";
  $ot = "DESC";
  $sortselect['SUBS'] = true;
 }else{
  $ob = "AddDate";
  $ot = "DESC";
  $sortselect['DATE'] = true;
 }
 include("template.inc");
 $tpl = new Template(APPLICATION_HOME."/templates/mypage", "keep");
 $tpl->set_file(array("main" => "mypage.html"));
 $pl  = build_pathlist($kat, false);
 $plf = build_pathlist($kat, true);
 $username = $auth->auth['uname']; 
 $restriction_list = build_restriction_list($kat);
 $in_login = false;

 
 $friend = (is_friendship($auth->auth['uid'], $userdata['User_ID']) === true);
 $itsme = false;
 if ( ( ($expert == $auth->auth['uname']) and ($auth->auth['uid'] != "nobody") ) or has_caps($capabilities, CAP_SUPERUSER) )
  $itsme = true;
 if ($itsme)
  $friend = true;
 
 if ($userdata['Favourites'] != "") 
  $fav_array =  explode('|||', $userdata['Favourites']);
 
/* $trails = get_trails_by_uname($expert, $userdata['User_ID'], $current_index, $ob, $ot, !$itsme);
 $max_trail_count = count_trails_mypage($expert, !$itsme); */

 
 /*if (!isset($viewdata_mypage['ob'])) $viewdata_mypage['ob'] = $ob;
 if (!isset($viewdata_mypage['ot'])) $viewdata_mypage['ob'] = $ot;
 $phplib_user->register("viewdata_mypage");
 */
 
 $trailsdata = read_trails_mypage(&$userdata, $current_index, $ob, $ot);
 if ($expert == $auth->auth['uname'])
  $t4u = read_a_trail_for_you($trailsdata['trails'], $auth->auth['uname'], $auth->auth['uid']);

 include("commonheader2.html");
 $tpl->set_var("SORTACTION", $PHP_SELF);
 
 foreach($sortselect as $key => $value){
  $text = ($value) ? "selected" : "";
  $tpl->set_var("SELECTED_SORT_".$key, $text);
 }
 $text = ($current_index == -1) ? "all" : "$current_index";
 $tpl->set_var("CURRENT_INDEX", $text);
 $tpl->set_var("USERNAME", $expert);
 $tpl->set_var("USERNAME_BIG", strtoupper($expert));

 $text = ( ((show_var($userdata["ShowFriend"], SHOW_NAME) and $friend) or ( show_var($userdata["ShowAnyone"], SHOW_NAME))) and ( ($userdata['FirstName'] != "") or ($userdata['LastName'] != "")))  ? print_mypage_name($userdata['FirstName'], $userdata['LastName']) : "";
 $tpl->set_var("NAME", $text);
 
 print_mypage_profile_item($tpl, "ABOUT", 'description.html', 'DESCRIPTION', 'Description', 'ABOUTME');
 print_mypage_profile_item($tpl, "EMAIL", 'email.html', 'EMAIL', 'Email', 'EMAIL');

 $text = ( ((show_var($userdata["ShowFriend"], SHOW_GENDER) and $friend) or ( show_var($userdata["ShowAnyone"], SHOW_GENDER))) and ($userdata['Gender'] != ""))  ? print_mypage_gender($userdata['Gender']) : "";
 $tpl->set_var("GENDER", $text);


 print_mypage_profile_item($tpl, "AGE", 'age.html', 'AGE', 'Age', 'AGE');
 print_mypage_profile_item($tpl, "COUNTRY", 'country.html', 'COUNTRY', 'Country', 'COUNTRY');
 print_mypage_profile_item($tpl, "PIC", 'photo.html', 'IMGURL', 'PhotoUrl', 'PIC');
 
 
 $tpl->set_var("FRIENDLIST", print_mypage_friendlist($userdata['User_ID'], $userdata, $itsme));
 $str = "";
 if ( is_array($trailsdata['trails']) ){
  $c=0;
  for($x=0; $x < count($trailsdata['trails']); $x++){
   $res = print_mypage_trail($trailsdata['trails'][$x], $expert, ( ($c % 2) == 0)   ? BACK_COLOR1 : BACK_COLOR2, $itsme);
   if ($res != ""){
    $str.=$res;
    $c++; //h„h„h„
   }
  }
 }
 //echo($expert);
 if ($userdata['Username'] == $auth->auth['uname'])
  $tpl->set_var("T4U", print_mypage_t4u($t4u));
 else
  $tpl->set_var("T4U", "");
 $tpl->set_var("TRAILS", $str);
 if ($trailsdata['count'] > DIR_MAX_TRAILS)
  $tpl->set_var("LIMITBOX", print_limitbox($current_index, $trailsdata['count'])); 
 else
  $tpl->set_var("LIMITBOX", ""); 
  
 $tpl->set_var("IWANTTO",  print_iwantto($kat));//print_mypage_iwantto($capabilities, $userdata));
 $tpl->parse("main", "main");
 $tpl->p("main");
 include("commonfooter2.html"); 
 }
 
function display_directory($exp_error=""){
 global $kat, $activekat, $autoopen, $sess, $HTTP_COOKIE_VARS, $nobody, $PHP_SELF, $auth, $capabilities, $editmode, $HTTP_GET_VARS, $movetrail, $movenode, $linknode, $glob_language_name;

if (!defined("LAY_DIRECTORY_INC"))
 include("layout/lay_directory.inc");
if (!defined("DIRECTORY_INC"))
 include("dbapi/directory.inc");
 
$mid = $HTTP_GET_VARS['movetrail'];
if ($mid){
 $trailinfo = get_node_info($mid);
 if ( ($trailinfo != -1) and ( ($trailinfo['userid'] == $auth->auth['uid']) or (has_caps($capabilities, CAP_SUPERUSER)) )){
  $movetrail = $mid;
  $sess->register("movetrail");
 }else{
  Header("Location: ".$sess->url($kat));
  exit;
 }
}

$kmid = $HTTP_GET_VARS['movenode'];
if ($kmid){
 $nodeinfo = get_node_info($kmid);
 if ( ($trailinfo != -1) and (has_caps($capabilities, CAP_ADMIN_DIRECTORY)) ){
  $movenode = $kmid;
  $sess->register("movenode");
 }else{
  Header("Location: ".$sess->url($kat));
  exit;
 }
}

$kmid = $HTTP_GET_VARS['linknode'];
if ($kmid){
 $nodeinfo = get_node_info($kmid);
 if ( ($trailinfo != -1) and (has_caps($capabilities, CAP_ADMIN_DIRECTORY)) ){
  $movenode = $kmid;
  $sess->register("linknode");
 }else{
  Header("Location: ".$sess->url($kat));
  exit;
 }
}

if ( $HTTP_GET_VARS['drop'] == "1" ){
 if (!defined("EDIT_DIRECTORY_INC"))
  include("dbapi/edit_directory.inc"); 
 if (!defined("DIRECTORY_NOTIFICATION_INC"));
  include("messages/directory_notification.inc"); 

 if ($movetrail != ""){
  $oldname = id2path($movetrail);
  mv_object($oldname, $kat.friendlyname($oldname));
  $ni = get_node_info($movetrail);
/*  if (has_caps($capabilities, CAP_SUPERUSER))
   send_link_admin_move_notification($ni, $newname);
  else
   send_move_notification($auth->auth['uid'], $ni, $newname); */
  $movetrail = "";
  
  $sess->unregister("movetrail");
 } 

 page_close();
 Header("Location: ".$sess->url($kat));
 exit;
  
}

if ( $HTTP_GET_VARS['nodedrop'] == "1" ){
 if (!defined("EDIT_DIRECTORY_INC"))
  include("dbapi/edit_directory.inc");
 
 if ($movenode != ""){
  $oldname = id2path($movenode);
  $newname = $kat.friendlyname($oldname)."/";
  mv_object($oldname, $newname);
  sub_rename($oldname, $newname);
  $movenode = "";
  $sess->unregister("movenode");
 }
 page_close();
  Header("Location: ".$sess->url($kat));
 exit;
}

if ( $HTTP_GET_VARS['linkdrop'] == "1" ){
 if (!defined("EDIT_DIRECTORY_INC"))
  include("dbapi/edit_directory.inc");
 if ($linknode != ""){
  $linksto = id2path($linknode);
  $newname = $kat.addslashes(str_replace(" ", "_", friendlyname($linksto))).'/';
  create_symlink($newname, $linksto);
  $linknode = "";
  $sess->unregister("linknode");
 } 
 page_close();
  Header("Location: ".$sess->url($kat));
 exit;
}

$ci = $HTTP_GET_VARS['ci'];
if ( $ci == "all")
 $ci = -1;
 
$current_index = intval($ci);

//$current_index = -1;
include("template.inc");
$pl  = build_pathlist($kat, false);
$plf = build_pathlist($kat, true);
$nodeinfo = get_node_info($kat);
$username = $auth->auth['uname'];
$restriction_list = build_restriction_list($kat,  $HTTP_GET_VARS['restriction'] == "/Experts/");
$in_login = false;

include("commonheader2.html");
include("directory/main/1.html");

 if ( ($autoopen) != "") {
  $sess->unregister("autoopen");
  print(print_autotrail($autoopen));
  $autoopen = "";
 }

$subnodes = get_subnodes_ex($nodeinfo['id']);

 if ($nodeinfo != -1)
  print(print_subnodes($kat, $subnodes, $capabilities));
 else
  print(print_kat_error($kat));

 if ($nodeinfo != -1){
  if ( isset($editmode) and ($editmode == "1") ){
   if (!defined("EDIT_DIRECTORY_INC"))
    include("dbapi/edit_directory.inc");
   if (!defined("LAY_DIREDIT_INC"))
    include("layout/lay_diredit.inc");
    
   print(print_editform($kat));
  }elseif( (isset($HTTP_GET_VARS['defexp']) and ($HTTP_GET_VARS['defexp'] == "1")) or ( is_array($exp_error) ) ){
   if (!defined("EDIT_DIRECTORY_INC"))
    include("dbapi/edit_directory.inc");
   if (!defined("LAY_DIREDIT_INC"))
    include("layout/lay_diredit.inc");
   print(print_defexp_form($nodeinfo, $exp_error));
  }else  
   print_trailgroup($nodeinfo, $capabilities, $current_index);
 }

include("directory/main/2.html");
 $activekat = $kat; 
 print(print_iwantto($kat, $capabilities));
 print(print_featured($kat, $capabilities));

 print(print_topex($kat));

 if ($kat == "/"){
  $test = "";
  print(print_newtrails($test));
 }else{
  print(print_didyouknow($nodeinfo['id']));
  print(print_newtrails($nodeinfo));
 }
 
include("commonfooter2.html"); 
}


?>