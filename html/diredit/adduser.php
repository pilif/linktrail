<?php
if ($laypreview== "1"){
 display_done();
 exit;
}

if ($dologin=="1"){
 Header("Location: /?dologin=1");
 exit;
}
page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm"));

include("dbapi/comutils.inc");

if (!defined("USER_INC"))
 include("dbapi/user.inc");

 if (!defined("EDIT_USER_INC"))
 include("dbapi/edit_user.inc");

if (!defined("LAY_LOGIN_INC"))
 include("layout/lay_login.inc");

if (!defined("COMMON_USER_INC"))
 include("commonapi/common_user.inc"); 

if (!defined("COMMON_ERRORS_INC"))
 include("commonapi/common_errors.inc");

if (!defined("PERMISSIONS_INC"))
 include("dbapi/permissions.inc");

 
  
$kat = $PHP_SELF;
if ($auth->auth['uid'] != "nobody"){
 $auth->unauth(true);
 $sess->unregister("linknode");  $linknode  = "";
 $sess->unregister("movenode");  $movenode  = "";
 $sess->unregister("movetrail"); $movetrail = "";
 setcookie("ltrLoginAs","",time()+2592000);  
 $HTTP_COOKIE_VARS['ltrLoginAs'] = "";
 page_close();
 Header("Location: $PHP_SELF");
}

if ($REQUEST_METHOD == "GET")
 display_form();
else{
 $errors = do_changes();
 if ( $errors == 0){
  $logon_now_as = $uid;
  if (isset($savepass))
   setcookie("ltrLoginAs",$uid,time()+2592000);  
  $sess->register("logon_now_as");
  page_close();
  display_done();
  exit;
 } 
 $suggestions = "";
 if ( ($errors & ERR_USEREXISTS) != 0 )
  $suggestions = get_suggestions(strtolower($username));
 
 $uname = "";
 if ($suggestions == ""){
 $uname = $username;
 }
 display_form($errors, $suggestions, $HTTP_POST_VARS['field_email'], $uname); 
}
 

function display_done(){
global $kat, $auth, $uid, $perm, $sess;

 if (!defined("COMMON_PERMISSIONS_INC"))
  include("commonapi/common_permissions.inc");

/* 
 auth_preauth() will authenticatge the user since logon_now_as is registered and set to true
*/
page_close();
page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm"));
$caps = get_caps($perm, '/');


 if (!defined("LAY_MYPAGE_INC"))
  include("layout/lay_mypage.inc");

 if (!defined("LAY_DIRECTORY_INC"))
  include("layout/lay_directory.inc");


$pl  = build_pathlist($kat, false);
$plf = build_pathlist($kat, true);
$restriction_list = build_restriction_list($kat);
//$username = $auth->auth['uname']; 
$in_login = false;

include("template.inc");
include("commonheader2.html");
$tpl = new Template("/home/linktrai/templates/login", "keep");
$tpl->set_file(array("main" => "regdone.html"));
//$tpl->set_var("IWANTTO", print_mypage_iwantto($caps, $auth->auth['uname']));
$tpl->set_var("USERNAME", $auth->auth['uname']);

$tpl->set_var("MYPAGEURL", $sess->url(build_good_url("/Experts/".$auth->auth['uname'])));
$tpl->set_var("SETTINGSURL", $sess->url(build_good_url("/Experts/".$auth->auth['uname'].'/Settings')));
$tpl->set_var("TOURURL", $sess->url("/Help/Tour/"));

$tpl->set_var("IWANTTO", print_iwantto('/', $caps));

$tpl->set_var("HOME_URL", $sess->url("/"));


$tpl->parse("main", "main");
$tpl->p("main");
include("commonfooter2.html");
page_close();
exit;
}

function display_form($error=0, $suggestions="", $email="", $uname=""){
 global $kat, $in_login, $perm, $ltrstr, $auth, $glob_language_name;

$nobody = ( ($auth->auth["uid"] == "nobody") or ($auth->auth["uid"] == "") or ($auth->auth["uid"] == "form"));
$pl  = build_pathlist($kat, false);
$plf = build_pathlist($kat, true);
$username = $auth->auth['uname']; 
$restriction_list = build_restriction_list($kat);
$in_login = false;

include("template.inc");
include("commonheader2.html");
echo(print_login_main(2, '/?forcelogin=1', $kat, "", false, $error, $suggestions, $email, $uname));
include("commonfooter2.html");
} //end of function display_form

function do_changes(){
 global $HTTP_POST_VARS, $invalid_user_names, $username, $uid;
 $errors = 0;
 
 $username = strtolower($HTTP_POST_VARS['field_username']);
 if ( ($username == "") and ($HTTP_POST_VARS['field_username_suggestion'] != '')){
  if ($HTTP_POST_VARS['field_username_suggestion'] == 'other')
   $username = $HTTP_POST_VARS['field_username_txt'];
  else
   $username = $HTTP_POST_VARS['field_username_suggestion'];
 }
 foreach($invalid_user_names as $baduser)
  if ($username == $baduser)
   $errors = $errors | ERR_NOUSERNAME;
 if ($username == "")
  $errors = $errors | ERR_NOUSERNAME;  
// lock_table('auth_user'); //we do not allow any writes of usernames whiile checking for dupes...
 if (user_exists($username))
   $errors = $errors | ERR_USEREXISTS; 
 if ($HTTP_POST_VARS['field_email'] == "")
   $errors = $errors | ERR_NOEMAIL;
 if (! validate_email($HTTP_POST_VARS['field_email']) )
   $errors = $errors | ERR_NOEMAIL;
 if ($HTTP_POST_VARS['field_password1'] != $HTTP_POST_VARS['field_password2'])
   $errors = $errors | ERR_PASSWORDMATCH;
 if ($errors != 0)
  return $errors;
 
 $userval['name']     = $username;
 $userval['email']    = $HTTP_POST_VARS['field_email'];
 $userval['password'] = $HTTP_POST_VARS['field_password1'];
 $uid=add_user($userval);
 create_friendship($uid, 'c1b984f523714d0bdf9b0bbf9bceea4e');
 create_friendship('c1b984f523714d0bdf9b0bbf9bceea4e', $uid);
 return 0;
}

page_close();
?>                            
                                                                                                                                