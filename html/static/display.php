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

if (!defined("LAY_STATIC_INC"))
 include("layout/lay_static.inc");

if (!defined("ERRORS_INC"))
 include("commonapi/errors.inc");

if ( ($REQUEST_METHOD == "POST") and ($HTTP_POST_VARS['forminfo'] != '') ){
  $parse_res = parse_static_form($HTTP_POST_VARS['forminfo']);
  if (!is_array($parse_res)){
   page_close();
   Header("Location: ".$sess->url($parse_res));
   exit;
  }
}

$alle = split('/', $PATH_INFO);
$method=strtolower($alle[1]);
if (($method != "about") and ($method != "help"))
 error('Method not valid', "The requested method ($method) is not valid", __LINE__, __FILE__);

$path = strtolower($PATH_INFO);
if ( (substr($path, -1) != "/" ) and (!file_exists(APPLICATION_HOME.'/static_pages'.$path.'.html')) ){
   page_close();
   Header("Location: ".$sess->url($PATH_INFO.'/'));
   exit;
}
 
if (ereg('/$', $path)){
 $path .= "index";
}

$title = $alle[count($alle)-1];
$kat = $PATH_INFO;
$pl  = build_pathlist($kat, false);
$plf = build_pathlist($kat, true);
$username = $auth->auth['uname']; 
$restriction_list = build_restriction_list($kat);
$in_login = false;

//die($path." ".$file);
//echo($path);
include("template.inc");
include("commonheader2.html");

$tpl = new Template(APPLICATION_HOME."/templates/static", "keep");
$tpl->set_file(array("main" => "main.html"));
$tpl->set_var("SIDEBAR", print_sidebar($method, $PATH_INFO));
$tpl->set_var("TITLE", get_static_title($PATH_INFO, $method));
$tpl->set_var("CONTENT", print_static_page($path, $parse_res));
$tpl->parse("main", "main");
$tpl->p("main");

include("commonfooter2.html");

page_close();
?>                            
                                                                                                                                