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

if ($REQUEST_METHOD == "GET")
 display_form($HTTP_GET_VARS);
else
 do_process();

function do_process(){
 global $HTTP_POST_VARS;

 $cond_array = Array();
 for($x=0; $x < count($HTTP_POST_VARS['term']); $x++){
  $cond_array[] = Array( 
                         "text" => $HTTP_POST_VARS['term'][$x],
                         "loc"  => $HTTP_POST_VARS['location'][$x],
                         "conn" => (($x > 0) and isset($HTTP_POST_VARS['connection'][$x-1])) ? $HTTP_POST_VARS['connection'][$x-1] : ""
                       );
 }
  
 if (isset($HTTP_POST_VARS['find'])){
  search($cond_array);
  exit;
 }
 
 if ($HTTP_POST_VARS['buildaction'] == "add_condition")
  $cond_array[] = Array( 
                         "text" => "",
                         "loc"  => "",
                         "conn" => "a"
                       );
 if (preg_match('/^erase_condition_([0-9]+)$/', $HTTP_POST_VARS['buildaction'], $matches)){
  unset($cond_array[$matches[1]]);
 }
 display_form($HTTP_POST_VARS, $cond_array);
}

function search(&$cond_array){
 global $PHP_SELF, $sess;
 
 $kat = (ereg_replace('^/([^-\]*-[^/]*)(.*)', '\\2', $PHP_SELF));

 $query = "";
 $connectors = Array(
                      "a" => " AND ",
                      "o" => " OR ",
                      "n" => " AND NOT "
                    );
 foreach($cond_array as $condition){
  if ($condition['text'] == "") continue; //ignore empty conditions!
  if ($condition['loc'] == "a"){
   $query .= $connectors[$condition['conn']];
   $query .= '"%'.$condition['text'].'%"'; 
  }else{
   $query .= $connectors[$condition['conn']];
   switch ($condition['loc']) {
    case "tt":
        $query .= '{"%'.$condition['text'].'%";TrailTitle:100}'; 
        break;
    case "td":
        $query .= '{"%'.$condition['text'].'%";TrailDesc:100}'; 
        break;
    case "lt":
        $query .= '{"%'.$condition['text'].'%";LinkName:100}'; 
        break;
    case "ld":
        $query .= '{"%'.$condition['text'].'%";LinkDesc:100}'; 
        break;
    case "lu":
        $query .= '{"%'.$condition['text'].'%";URL:100}'; 
        break;
   }
  }
 }
// die("<pre>$query</pre>");
 Header("Location: ".$sess->url(build_good_url(parent_path($kat)).'Search?aq='.urlencode($query)));

}

function display_form(&$http_vars, $cond_array=Array()){
 global $PHP_SELF;
 
 if (!defined("LAY_SEARCH_INC"))
  include("layout/lay_search.inc");
 if (!defined("LAY_DIRECTORY_INC"))
  include("layout/lay_directory.inc"); 
 
 $kat = (ereg_replace('^/([^-\]*-[^/]*)(.*)', '\\2', $PHP_SELF));
 
 page_head($http_vars, $kat);
 $tpl = new Template(APPLICATION_HOME."/templates/search", "keep");
 $tpl->set_file(array("main" => "main.html"));
 $tpl->set_var("IWANTTO", print_iwantto('/', $caps));
 $tpl->set_var("EXPERTS", "");
 $tpl->set_var("TRAILS", "");
 $tpl->set_var("CATEGORIES", print_adv_search_form($http_vars, $cond_array, parent_path($kat)));
 $tpl->parse("main", "main");
 $tpl->p("main");
 page_foot();
}

function page_head(&$http_vars, $kat){
 global $PHP_SELF, $auth;

 $pl  = build_pathlist($kat, false);
 $plf = build_pathlist($kat, true);
 $username = $auth->auth['uname']; 
 $restriction_list = build_restriction_list($kat,  $http_vars['restriction'] == "/Experts/");
 $nobody = ( ($auth->auth["uid"] == "nobody") or ($auth->auth["uid"] == "") or ($auth->auth["uid"] == "form"));
 $in_login = false;
 include("template.inc");
 include("commonheader2.html");
}

function page_foot(){
 include("commonfooter2.html");
}
?>                                                                                                                               