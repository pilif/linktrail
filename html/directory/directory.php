<?php
page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm"));
if ( $dologout == "1" ){
 //die("unauth");
 
 $auth->unauth(true);
 $sess->unregister("linknode");  $linknode  = "";
 $sess->unregister("movenode");  $movenode  = "";
 $sess->unregister("movetrail"); $movetrail = "";
 setcookie("ltrLoginAs","",time()+2592000);  
 $HTTP_COOKIE_VARS['ltrLoginAs'] = "";
 page_close();
 Header("Location: ".$PATH_INFO); 
 exit;
}
if ( $glob_immp ){
 $glob_imp = false;
 $sess->unregister("glob_immp");
 page_close();
 Header("Location: ".$sess->url("/Experts/".rawurlencode($auth->auth['uname'])));
 exit;
}
$auth->login_if( ($dologin=="1") and ($auth->auth["uid"] == "nobody") );
$nobody = ( ($auth->auth["uid"] == "nobody") or ($auth->auth["uid"] == "") or ($auth->auth["uid"] == "form"));

//$perm->check("admin");

//activekat contains the active category. This is used for checking if a new
//directory-Window should be opened because the trail got requested directly
//without going ovber our directory. Advantages: 
// ->Trails can be bookmarked
// ->Bookmarked trails appear in the correct format (Popup-Window)
// ->Our brand is even more present to the user

if (!isset($activekat))
 $sess->register("activekat");

/*if (isset($kat)){
 $kat = "";
 $sess->unregister("kat");
}*/
if (!defined("COMUTILS_INC"))
 include_once("dbapi/comutils.inc");
require_once("commonapi/common_permissions.inc");
require_once("application/display_directory.inc");


if (isset($node))
 $kat = base64_decode($node);
else
 $kat = $PATH_INFO;

$kat = trim($kat);
 
 
$capabilities = get_caps($perm, $kat); // this is used at many places

/*
 Mod_rewrite directs us all the queries to the directory to this file. Now
 we will have to find out whether we have to display a node, or a trail.
 
 If it's a trail, we must find out, if we can only redirect to the Trail-Window
 (we got called from the correct directory-node) or if we have to display
 the correct node (parent of Trail) and open the Trail in a new Window
*/
 
if (preg_match('/^\/Experts\/(.+)/', $kat, $found)){
  display_mypage($found[1]);
}else{

 if ((!is_trail($kat)) or ($kat == "/")){
  display_directory(); 
 }else{
  $autoopen = $kat;
  $sess->register("autoopen");
  header("Location: ".$sess->url(parent_path($kat)));
 }
}
page_close();
//echo("<p>");
/*foreach($auth->auth as $key => $value)
 echo("$key = $value<br>"); */
/*foreach($GLOBALS as $key => $value)
 echo("$key = $value<br>");*/
?>                            

