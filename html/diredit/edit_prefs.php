<?php
page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm"));

if ( $dologout == "1" ){
 $auth->unauth(true);
 $sess->unregister("linknode");  $linknode  = "";
 $sess->unregister("movenode");  $movenode  = "";
 $sess->unregister("movetrail"); $movetrail = "";

 setcookie("ltrLoginAs","",time()+2592000);  
 $HTTP_COOKIE_VARS['ltrLoginAs'] = "";
 page_close();
 Header("Location: /"); 
 exit;
}
if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");

if (!defined("USER_INC"))
 include("dbapi/user.inc");

 if (!defined("EDIT_USER_INC"))
 include("dbapi/edit_user.inc");

if (!defined("LAY_MYPAGE_INC"))
 include("layout/lay_mypage.inc");

if (!defined("LAY_EDIT_MYPAGE_INC"))
  include("layout/lay_edit_mypage.inc");
  
if (!defined("COMMON_USER_INC"))
 include("commonapi/common_user.inc"); 
  
global $leer, $expert, $method, $extension;
list($leer, $expert, $method, $extension) = split('/', $PATH_INFO);
$virt_dir = Array("Settings", "Messages");

if ($expert == "")
 error("Unknown Expert", "The given Expert is not known. PLease do not call this file directely", __LINE__, __FILE__);

if ($method == ""){
 Header("Location: ".$sess->url("/Experts/".rawurlencode($expert)));
 exit;
}

//All of this pages but Send_Message are private for the user that is on his mypage.
//Send_Message works for all users that are logged on
if ( ($method != "Send_Message") and ($method != 'Make_friendship') and ($method != 'Settings')){
 if ( ($expert != $auth->auth['uname']) or ($auth->auth['uid'] == "nobody") ){
  Header("Location: ".$sess->url("/Experts/".rawurlencode($expert)));
  exit;
 }
}elseif ( ($method == "Send_Message") or ($method == "Make_Friendship")){
 //user must be authenticated when sending a message
 $auth->login_if( ($auth->auth["uid"] == "nobody") );
}else{
 //user must authenticated and THE user when sending a message
 $auth->login_if( ($auth->auth["uid"] == "nobody") or ($auth->auth['uname'] != $expert) );
}

if ( ($method != "Settings") and ($method != "Make_friendship") and ($method != "Send_Message") and ($method != "Messages"))
 error("Unknown Method", "The given Method ($method) is not known. Please do not call this file directely", __LINE__, __FILE__);

$kat = "/Experts".$PATH_INFO;

foreach($virt_dir as $dir){
 if ( ($method == $dir) and ($extension == "" and (substr($PATH_INFO, -1) != '/') ) ){
  page_close();
  Header("Location: ".$sess->url("/Experts/".rawurlencode($expert)).'/'.$method.'/');
  exit;
 } 
}

global $HTTP_POST_VARS;

if (($REQUEST_METHOD == "GET") or ($HTTP_POST_VARS['username'] != ""))
 display_form();
else
 do_changes();
 

 
function display_form($passerror = false, $senderror="", $ssenderror=false, $posconf=false){
 global $kat, $expert, $in_login, $perm, $extension, $sess, $method, $ltrstr, $HTTP_GET_VARS, $auth, $viewdata_messages, $PATH_INFO;

list($leer, $expert, $method, $extension) = split('/', $PATH_INFO);

$pl  = build_pathlist($kat, false);
$plf = build_pathlist($kat, true);
$restriction_list = build_restriction_list($kat);
$username = $auth->auth['uname']; 
$in_login = false;
$userdata = get_user_from_name($expert);
$capabilities = get_caps($perm, $kat); 

if (!class_exists("Template"))
 include("template.inc");

if (!(isset($HTTP_GET_VARS['viewtype']) and ($HTTP_GET_VARS['viewtype'] == "0"))){
 $tpl = new Template(APPLICATION_HOME."/templates/mypage", "keep");
 $tpl->set_file(array("main" => "editall.html"));

 include("commonheader2.html");
 $tpl->set_var("USERNAME", $expert);
}

switch ($method) {

     case "Messages":
         if ($extension == "") $extension = "Inbox";
         if (!isset($viewdata_messages)){
          $viewdata_messages['ci'] =   0;
          $viewdata_messages['ob'] = 'd';
          $sess->register("viewdata_messages");
         }
         if (isset($HTTP_GET_VARS['ci'])){
          $ci = $HTTP_GET_VARS['ci'];
          $ci = ($HTTP_GET_VARS['ci'] == "all") ? "-1" : $ci;
          $viewdata_messages['ci'] = ($ci == "") ? 0 : $ci;
         }
         if (isset($HTTP_GET_VARS['ob'])){
          $viewdata_messages['ob'] = ($HTTP_GET_VARS['ob'] == "") ? 'd' : $HTTP_GET_VARS['ob'];
         }
         if (isset($HTTP_GET_VARS['viewtype']) and ($HTTP_GET_VARS['viewtype'] == "0")){
          do_mor($userdata, $HTTP_GET_VARS, ($extension == "Inbox"));
         }elseif (isset($HTTP_GET_VARS['viewtype']) and ($HTTP_GET_VARS['viewtype'] != "0")){
          $tpl->set_var("EDITFORM", print_my_lms($userdata, $viewdata_messages['ci'], $viewdata_messages['ob'], $senderror, $HTTP_GET_VARS['objectid'], $HTTP_GET_VARS['viewtype'], $extension));
         }else{
          $tpl->set_var("EDITFORM", print_my_lms($userdata, $viewdata_messages['ci'], $viewdata_messages['ob'], $senderror, "", 1, $extension));
         }
         $subnav = Array();
         $subnav[0] = array( "title" => $ltrstr['Inbox'], "url" => "Inbox" );
         $subnav[1] = array( "title" => $ltrstr['Outbox'], "url" => "Outbox" );
//         die(count_friends($userdata['User_ID']));
         if (count_friends($userdata['User_ID']) != 0)
          $subnav[2] = array( "title" => $ltrstr['Composer'], "url" => "Composer" );
         switch ($extension) {
            case "Inbox":
                $idx=0;
                break;
            case "Outbox":
                $idx=1;
                break;
            case "Composer":
                $idx=2;
                break;
            default: 
                $idx=-1;    
         }
         $tpl->set_var("SUBNAV", "");
         update_read_stamp($userdata['User_ID']);
         break;
     case "Send_Message":
         $tpl->set_var("EDITFORM", print_message_send_form($userdata['Username'], $ssenderror));
         $tpl->set_var("SUBNAV", "");
         break;
     case "Make_friendship":
         $tpl->set_var("EDITFORM", print_make_friendship($userdata['Username']));
         $tpl->set_var("SUBNAV", "");
         break;
     case "Settings":
         if ($extension == "") $extension = "Profile";
         //die("Ext: ".$extension);
         $subnav = Array();
         $subnav[0] = array( "title" => $ltrstr['Profile'], "url" => "Profile" );
         $subnav[1] = array( "title" => $ltrstr['Security Preferences'], "url" => "Password" );
         $subnav[2] = array( "title" => $ltrstr['Preferences'], "url" => "Preferences" );
         switch ($extension) {
            case "Profile":
                $idx=0;
                break;
            case "Password":
                $idx=1;
                break;
            case "Preferences":
                $idx=2;
                break;
            default: 
                $idx=-1;    
         }
         
         $tpl->set_var("EDITFORM", print_all_settings_form($extension, $userdata, $passerror, $posconf));
         $tpl->set_var("SUBNAV", "");
         break;
 }
      

$itsme = ($auth->auth['uname'] == $expert) or has_caps($capabilities, CAP_SUPERUSER);
$tpl->set_var("FRIENDLIST", print_mypage_friendlist($userdata['User_ID'], $userdata, $itsme));
$tpl->set_var("IWANTTO",  print_iwantto($PATH_INFO));
$tpl->parse("main", "main");
$tpl->p("main");
include("commonfooter2.html");
} //end of function display_form

//This is a small functoin that is build similar to the above display_form.
//It distinguishes, what has changed and calls an appropriate function
function do_changes(){
 global $sess, $HTTP_POST_VARS, $extension;
 
 global $method, $expert;

 switch ($method) {
     case "Send_Message":
         $userdata = get_user_from_name($expert);
         simple_send($userdata);
         $back_to_mypage = true;
         break;

     case "Make_friendship":
         $userdata = get_user_from_name($expert);
         do_fs_request($userdata);
         $back_to_mypage = true;
         break;
         
     case "Messages":
         $userdata = get_user_from_name($expert);
         if (isset($HTTP_POST_VARS['objectid']))
          do_mor($userdata, $HTTP_POST_VARS, ($extension == "Inbox")); //the Form of a message-object was called. messageobjectrequest handles this
         else
          do_send_message($userdata);
         $back_to_mypage = false;
         break;
     case "Settings":
         $userdata = get_user_from_name($expert);
         do_change_settings($userdata);
         $back_to_mypage = false;
         break;
 }
 if ($back_to_mypage)
  Header("Location: ".$sess->url("/Experts/".rawurlencode($expert))); 
}

function sub_change_profile(&$currentuser, $dbname, $fieldname){
 global $HTTP_POST_VARS;
 
 if ($dbname != "")
  $currentuser[$dbname] = $HTTP_POST_VARS['field_'.$fieldname];
 eval('if ($HTTP_POST_VARS["radio_show_".$fieldname] == "a")  $currentuser["ShowAnyone"] = $currentuser["ShowAnyone"] | SHOW_'.strtoupper($fieldname).';');
 eval('if ($HTTP_POST_VARS["radio_show_".$fieldname] == "f")  $currentuser["ShowFriend"] = $currentuser["ShowFriend"] | SHOW_'.strtoupper($fieldname).';');
 eval('if ($HTTP_POST_VARS["radio_show_".$fieldname] == "n")  $currentuser["ShowNobody"] = $currentuser["ShowNobody"] | SHOW_'.strtoupper($fieldname).';');
}


function do_change_profile_ex($currentuser){
 global $HTTP_POST_VARS; //there are much too many vars to import them.
 
 $old_mail = $currentuser['Email'];

 foreach($HTTP_POST_VARS as $key => $value){
  $HTTP_POST_VARS[$key] = cleanup_string($value);
 }
 
 $currentuser['ShowNobody'] = 0;
 $currentuser['ShowFriend'] = 0;
 $currentuser['ShowAnyone'] = 0;


 $currentuser['FirstName'] = $HTTP_POST_VARS['field_firstname'];
 $currentuser['LastName']  = $HTTP_POST_VARS['field_lastname'];
 if ($HTTP_POST_VARS['radio_show_name'] == "a")  $currentuser['ShowAnyone'] =  $currentuser['ShowAnyone'] | SHOW_NAME;
 if ($HTTP_POST_VARS['radio_show_name'] == "f")  $currentuser['ShowFriend'] =  $currentuser['ShowFriend'] | SHOW_NAME;
 if ($HTTP_POST_VARS['radio_show_name'] == "n")  $currentuser['ShowNobody'] =  $currentuser['ShowNobody'] | SHOW_NAME;
 
 sub_change_profile($currentuser, 'Email', 'email');
 sub_change_profile($currentuser, 'Gender', 'gender');
 sub_change_profile($currentuser, 'Age', 'age');
 sub_change_profile($currentuser, 'Description', 'about');

 sub_change_profile($currentuser, 'Country', 'country');
 sub_change_profile($currentuser, 'PhotoUrl', 'pic');

 $currentuser['Description'] = plattform_nl2br($currentuser['Description']);
 
 /* If the Email-Address changes, suppose, it is valid */
 if (strtolower($currentuser['Email']) != strtolower($old_mail))
  $currentuser['BounceFlag'] = BOUNCE_OK;
  
 set_user_at_name($currentuser['Username'], $currentuser); 
}


function do_change_security($currentuser){
 global $HTTP_POST_VARS;
 
 if ($HTTP_POST_VARS['field_password2'] != $HTTP_POST_VARS['field_password'])
  return false;
 
 if ($HTTP_POST_VARS['field_password2'] == "")
  return true;
  
 change_password($currentuser, $HTTP_POST_VARS['field_password']);

 return true;
}

function do_change_preferences($currentuser){
 global $HTTP_POST_VARS, $glob_userdata;

foreach($currentuser as $key => $value)
 $currentuser[$key] = cleanup_string($value);
 
foreach($HTTP_POST_VARS as $key => $value)
  $HTTP_POST_VARS[$key] = cleanup_string($value);
  
$currentuser['PopupPos']          = $HTTP_POST_VARS['field_popup_pos'];  
$currentuser['PopupPos']          = $HTTP_POST_VARS['field_popup_pos'];  
$currentuser['NS6Sidebar']        = (isset($HTTP_POST_VARS['field_nsside']))  ? true : false;
$currentuser['ResPerPage']        = $HTTP_POST_VARS['field_count_search_results'];
$currentuser['HighlightSearch']   = (isset($HTTP_POST_VARS['field_highlight_results']))  ? true : false;
$currentuser['MypageAfterLogon']  = (isset($HTTP_POST_VARS['field_mypage_after_login']))  ? true : false;
$currentuser['NewWindow']         = (isset($HTTP_POST_VARS['field_newwindow']))  ? true : false;

$currentuser['NotificationStyle'] = $HTTP_POST_VARS['field_notification_method'];

$currentuser['Vacation']   = (isset($HTTP_POST_VARS['field_vacation_mode']))  ? true : false;

set_user_at_name($currentuser['Username'], $currentuser); 
$glob_userdata = $currentuser;
}

function do_send_message($userdata){
 global $HTTP_POST_VARS, $ltrstr;
 $errstr = $ltrstr['Errors occurred']."<ul>\n";
 $error = 0;
 if ( ($HTTP_POST_VARS['field_subject'] == "") and ($HTTP_POST_VARS['field_message'] == "") ){
  $error  = $error | 1;
  $errstr .= '<li>'.$ltrstr['Subject missing'];
 }

 $target_user = ($HTTP_POST_VARS['field_friend'] != "-1") ? get_user_from_name($HTTP_POST_VARS['field_friend']) : get_user_from_name($HTTP_POST_VARS['field_username']);

 if ($target_user == "-1"){ //cannot happen if we are not hacked...
  $error  = $error | 2;
  $errstr .= '<li>'.$ltrstr['User not found']; 
 }else{
  $target_user = $target_user['User_ID'];
 }

if ($error != 0){
 $errorh['msg']            = $errstr.'</ul>';
 $errorh['subject']        = ($error & 1) != 0;
 $errorh['username']       = ($error & 2) != 0; 
 $errorh['predef_message'] = $HTTP_POST_VARS['field_message'];
 $errorh['predef_subject'] = $HTTP_POST_VARS['field_subject'];
 $errorh['predef_user']    = $HTTP_POST_VARS['field_username'];
 display_form(false, $errorh);
}else{
 if (!defined("TALK_INC"))
  include("messages/talk.inc");
 talk($userdata['Username'], $target_user, $HTTP_POST_VARS['field_subject'], $HTTP_POST_VARS['field_message']);
 display_form(false, 999);
}

}

function do_mor($userdata, &$http_vars, $in){
 global $sess, $SERVER_NAME, $extension;
 
 if(!defined("MESSAGES_INC"))
  include("dbapi/messages.inc");

 if (!$http_vars['objectid']){
  page_close();
  Header($sess->url(build_good_url("/Experts/".$userdata['Username']."/Messages")));
  exit;
 }
 
 $msg = read_one_message($http_vars['objectid'], $in);
 if (!is_object($msg)){
  page_close();
  Header("Location: ".$sess->url(build_good_url("/Experts/".$userdata['Username']."/Messages")));
  exit;
 }
 
 if ( (($msg->get_target() != $userdata['User_ID']) and ($msg->is_inbox)) and (($msg->sender != $userdata['Username']) and (!$msg->is_inbox))){
  /* 
   Some script-kiddie just noticed the message-id in the URI and tried to 
   enter the ID of a message that was not sent to you. I'm just laughing and
   redirecting the funny guy back...
  */
  page_close();
  Header("Location: ".$sess->url(build_good_url("/Experts/".$userdata['Username']."/Messages")));
  exit;
 }

 $str = "";
 $str = $msg->user_interaction($http_vars);
 if ($str == "")
  $str = $sess->url(build_good_url("/Experts/".$userdata['Username']."/Messages/$extension")."?t=2");
// page_close();
// echo("aa");
header("Location: ".$str);
// die("Str: $str");
// echo("a");
 exit;
}

function simple_send($userdata){
 global $HTTP_POST_VARS, $auth;
 if ( ($HTTP_POST_VARS['field_subject'] == "") and ($HTTP_POST_VARS['field_message'] == "") ){
   display_form(false, "", true);
   page_close();
   exit(); //there is a header(Location: mypage) after this funtion so don't exit this one!
 }else{
 if (!defined("TALK_INC"))
  include("messages/talk.inc");
 talk($auth->auth['uname'], $userdata['User_ID'], $HTTP_POST_VARS['field_subject'], $HTTP_POST_VARS['field_message']);
 }
 
}

function do_fs_request($userdata){
 global $HTTP_POST_VARS, $auth;
 
 if (!defined("MSG_FRIENDSHIPS_INC"))
  include("messages/friendships.inc");

 request_friendship($auth->auth['uname'], $userdata['User_ID'], $HTTP_POST_VARS['field_message']);
}


function do_change_settings($userdata){
 global $HTTP_POST_VARS, $expert, $sess, $extension;
 
 if (isset($HTTP_POST_VARS['change_profile'])){
  do_change_profile_ex($userdata);
 }elseif(isset($HTTP_POST_VARS['change_preferences'])){
  do_change_preferences($userdata);
 }elseif(isset($HTTP_POST_VARS['change_password'])){
  if (!do_change_security($expert))
   display_form(true);
 }
 $extension = "";
 display_form(false, "", false, true);
}

page_close();
//echo($viewdata_messages['ci']);
?>
