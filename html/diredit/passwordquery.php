<?php
if ($dologin=="1"){
 Header("Location: /?dologin=1");
 exit;
}
page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm"));
if ( $dologout == "1" ){
// die("unauth");
 $auth->unauth(true);
 $sess->unregister("linknode");  $linknode  = "";
 $sess->unregister("movenode");  $movenode  = "";
 $sess->unregister("movetrail"); $movetrail = "";

 setcookie("ltrLoginAs","",time()+2592000);  
 $HTTP_COOKIE_VARS['ltrLoginAs'] = "";
 page_close();
 Header("Location: ".$PHP_SELF); 
 exit;
}

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

 if (!defined("LAY_MYPAGE_INC"))
  include("layout/lay_mypage.inc");

$kat = (ereg_replace('^/([^-\]*-[^/]*)(.*)', '\\2', $PHP_SELF));
$pl  = build_pathlist($kat, false);
$plf = build_pathlist($kat, true);
$username = $auth->auth['uname']; 
$restriction_list = build_restriction_list($kat);
$in_login = false;
$nobody = ( ($auth->auth["uid"] == "nobody") or ($auth->auth["uid"] == "") or ($auth->auth["uid"] == "form"));
$capabilities = get_caps($perm, '/');

include("template.inc");
include("commonheader2.html");

 
if ($REQUEST_METHOD == "POST")
 doit();
else
 form();

include("commonfooter2.html");
page_close();



function form($error=""){
 global $kat, $capabilities;

 print(print_password_request(false, '/', $capabilities));
}

function doit(){
global $HTTP_POST_VARS;

if ($HTTP_POST_VARS['field_username'] != "")
 $passwordhash = query_password($HTTP_POST_VARS['field_username']);
else
 $passwordhash = query_password($HTTP_POST_VARS['field_email'], false);
if (is_array( $passwordhash ))
 send_mail($passwordhash);
done();
}

function done(){
 global $kat, $capabilities;

 print(print_password_request(true, $kat, $capabilities));
}

function send_mail($passwordhash){
 mail($passwordhash['email'], "Your linktrail password", "Name: ".$passwordhash['username']."\nEmail: ".$passwordhash['email']."\nPassword: ".$passwordhash['password'], "From: linktrail <info@linktrail.com>");
}
?>                            
                                                                                                                                