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

if ($dologin=="1"){
 Header("Location: ".$sess->url("/?dologin=1"));
 exit;
}

include("dbapi/comutils.inc");
//die("b");

if (isset($sq))
 print(print_simple_results($sq));
else
 echo("Advanced form not implemneted yet");

page_close();

function print_simple_results($query){
 global $perm, $sess, $HTTP_GET_VARS, $glob_userdata;
 if (!defined("SEARCH_INC"))
  include("search/search2.inc");

 if (!defined("LAY_DIRECTORY_INC"))
  include("layout/lay_directory.inc"); 
 if (!defined("LAY_SEARCH_INC"))
  include("layout/lay_search.inc"); 
 if (!defined("COMMON_PERMISSIONS_INC"))
  include("commonapi/common_permissions.inc");

$ciu = ($HTTP_GET_VARS['ciu'] == "") ? 0 : $HTTP_GET_VARS['ciu'];
$cit = ($HTTP_GET_VARS['cit'] == "") ? 0 : $HTTP_GET_VARS['cit'];
$cic = ($HTTP_GET_VARS['cic'] == "") ? 0 : $HTTP_GET_VARS['cic'];
if ($glob_userdata['ResPerPage'] == "")
 $glob_userdata['ResPerPage'] = DIR_MAX_SEARCH;
if ($glob_userdata['HighlightSearch'] == "")
 $glob_userdata['HighlightSearch'] = true;

$restriction = $HTTP_GET_VARS['restriction'];

if ($query != ""){
 $err = "";
 $query_hash   = format_query($query);
 $query = $query_hash['query'];
 global $glob_search_words;
 $glob_search_words = $query_hash['words'];
 foreach($glob_search_words as $key => $value)
  $glob_search_words[$key] = str_replace('"', "", $value);
  
 $users = Array();
 if (!ereg('^/Experts', $restriction) ){
  $cats    = execute_query($query, $sess->id, $cic, "c", $restriction, $glob_userdata['ResPerPage']);
  
  if ( !isset($cats['error']) )
   $trails  = execute_query($query, $sess->id, $cit, "t", $restriction, $glob_userdata['ResPerPage']);
  else
   $err = $cats['error'];
   
  if ( !is_array($err) )
   $experts = execute_query($query, $sess->id, 0, "e", $restriction, $glob_userdata['ResPerPage']);

  if ((isset($experts['error'])) and (!is_array($err)))
   $err = $experts['error']; 
  
 }else{
  $users  = execute_query($query, $sess->id, $ciu, "t", $restriction, $glob_userdata['ResPerPage']);
//  die($users['resinfo']['rescount']);
  if (isset($users['error']))
   $err = $users['error']; 
  $users['resinfo']['ci'] = $ciu;
 }
}
print_page_begin();
$tpl = new Template(APPLICATION_HOME."/templates/search", "keep");
$tpl->set_file(array("main" => "main.html"));
$caps = get_caps($perm, '/');
$tpl->set_var("IWANTTO", print_iwantto('/', $caps));
//printf("Test: %d / %d / %d<p>",count($trails['results']) ,count($trails['results']) ,count($trails['results']) );
//printf("Test: %d, %d, %d", count($cats['results']), count($trails['results']), count($users['results']));

if ( ((count($cats['results']) == 0) and (count($trails['results']) == 0 ) and (count($users['results']) == 0)) or ($query == "") ){
 if(is_array($err)){
  $tpl->set_var("CATEGORIES", print_search_error($err));
 }else{
  $tpl->set_var("CATEGORIES", implode("\n", file(TEMPLATE_ROOT.'search/nothing_found.html')));
 }
 $tpl->set_var("TRAILS", "");
 $tpl->set_var("EXPERTS", "");
}elseif (count($users) > 0){

 $tpl->set_var("CATEGORIES", print_found_users($users));
 $tpl->set_var("EXPERTS", "");
 $tpl->set_var("TRAILS", "");
}else{
 $tpl->set_var("CATEGORIES", print_found_categories($cats));
 $tpl->set_var("TRAILS", print_found_trails($trails));
 $tpl->set_var("EXPERTS", print_found_experts($experts));
}

$tpl->parse("main", "main");
$tpl->p("main");
print_page_end();
}

function print_page_begin(){
global $PHP_SELF, $auth, $HTTP_GET_VARS;

$kat = (ereg_replace('^/([^-\]*-[^/]*)(.*)', '\\2', $PHP_SELF));
$pl  = build_pathlist($kat, false);
$plf = build_pathlist($kat, true);
$username = $auth->auth['uname']; 
$restriction_list = build_restriction_list($kat,  $HTTP_GET_VARS['restriction'] == "/Experts/");
$nobody = ( ($auth->auth["uid"] == "nobody") or ($auth->auth["uid"] == "") or ($auth->auth["uid"] == "form"));
$in_login = false;
include("template.inc");
include("commonheader2.html");
}

function print_page_end(){
include("commonfooter2.html");
}
?>                            
                                                                                                                                