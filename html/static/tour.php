<?php
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

$kat = (ereg_replace('^/([^-\]*-[^/]*)(.*)', '\\2', $PHP_SELF));

if (preg_match('/^\/Tour[\/]*(.*)$/', $kat, $matches)){
 if ($matches[1] == ""){
  if ( (substr($kat, -1)) != '/'){
    page_close();
    Header("Location: ".$sess->url("/Tour/"));
  }else{ 
   $matches[1] = $tour_steps[0][0];
  } 
 }
 $step_name = $matches[1];
 $step = get_step($step_name) + 1;
 $step_title = $tour_steps[$step-1][1];
 if ($step <= 0){
  page_close();
  Header("Location: ".$sess->url("/Tour/"));
  exit;
 }
}else{
 if ( (substr($kat, -1)) != '/')
  Header("Location: ".$sess->url("/Tour/"));
 else{
  $step_name  = $tour_steps[0][0]; 
  $step_title = $tour_steps[0][1]; 
  $step       = 1;
 }
}

//printf("step: %d; step-name: %s; step-title: %s", $step, $step_name, $step_title);
if ( $dologout == "1" ){
// die("unauth");
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

if (!defined("ERRORS_INC"))
 include("commonapi/errors.inc");

if (!defined("LAY_TOUR_INC"))
 include("layout/lay_tour.inc");

if (!defined("LAY_DIRECTORY_INC"))
 include("layout/lay_directory.inc");

if (!defined("COMMON_PERMISSIONS_INC"))
 include("commonapi/common_permissions.inc");
 
 $count = 0;
 $d = dir('/home/linktrai/static_pages/tour/');
 while($entry=$d->read()) {
   if(preg_match('/^([0-9]+)\.html/', $entry, $idx)) {
    $files[$count] = $idx[1];
    $count++;
   }
 }
$d->close();

if ($HTTP_GET_VARS['noflash'] == "true"){
 $sess_noflash = true;
 $sess->register("sess_noflash");
}elseif($HTTP_GET_VARS['noflash'] == "false"){
 $sess_noflash = false;
 $sess->unregister("sess_noflash");
}

$pl  = build_pathlist($kat, false);
$plf = build_pathlist($kat, true);
$username = $auth->auth['uname']; 
$restriction_list = build_restriction_list($kat);
$in_login = false;
$capabilities = get_caps($perm, '/'); // this is used at many places


//die($path." ".$file);
//echo($path);
include("template.inc");
include("commonheader2.html");
$tpl = new Template("/home/linktrai/templates/tour", "keep");
$tpl->set_file(array("main" => "main.html"));
$tpl->set_var("CONTENT", print_tour_content($step));
$tpl->set_var("IWANTTO", print_iwantto('/', $capabilities));
$tpl->set_var("NAVIGATION", print_tour_navigation($step));
$tpl->set_var("TOURNAV", print_nav_links($step));
$tpl->parse("main", "main");
$tpl->p("main");

include("commonfooter2.html");

page_close();

function get_step($stepname){
 global $tour_steps;
 
 for($x=0; $x<count($tour_steps);$x++){
  if (strtolower($tour_steps[$x][0]) == strtolower($stepname))
   return $x;
 }
 return -1;
}

?>                            
                                                                                                                                