<?php
define("LAY_TOUR_INC", true);
/*
  lay_tour.inc; 1.00
 */
 
if (!defined("DIRECTORY_INC"))
 include("dbapi/directory.inc");

if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");


function print_next_link($nr){
 global $tour_steps;

 $tpl  = new Template(APPLICATION_HOME."/templates/tour/nav", "keep");
 if ($nr < count($tour_steps)){
  $tpl->set_file(array("main" => "next_page.html")); 
  $tpl->set_var("NEXTURL", '/Tour/'.$tour_steps[$nr][0]);
 }else{
  $tpl->set_file(array("main" => "next_page_inactive.html"));  
 }
 $tpl->parse("main", "main");
 return $tpl->get("main");
} 

function print_prev_link($nr){
 global $tour_steps;

 $tpl  = new Template(APPLICATION_HOME."/templates/tour/nav", "keep");
 if ($nr > 1){
  $tpl->set_file(array("main" => "previous_page.html")); 
  $tpl->set_var("PREVURL", '/Tour/'.$tour_steps[$nr-2][0]);
 }else{
  $tpl->set_file(array("main" => "previous_page_inactive.html"));  
 }
 $tpl->parse("main", "main");
 return $tpl->get("main");
} 

 
function print_nav_links($page_id){
 
 $strs = "";
 $strs .= print_prev_link($page_id);
 $strs .= print_next_link($page_id);
 
 return $strs;
}

function print_tour_content($step){
 if (file_exists(APPLICATION_HOME."/includes/tour-replacement/".$step.".inc")){
  $tpl  = new Template(APPLICATION_HOME."/static_pages/tour", "keep");
  $tpl->set_file(array("main" => "$step.html"));  
  include(APPLICATION_HOME."/includes/tour-replacement/".$step.".inc");
  $tpl->parse("main", "main");
  return $tpl->get("main");
 }else{
  if (file_exists(APPLICATION_HOME."/static_pages/tour/".$step.".html"))
   return implode("\n", file(APPLICATION_HOME."/static_pages/tour/".$step.".html"));
  else
   return "Media file $step.html could not be found on the server";
 }  
}


function print_one_tour_link($active, $title, $url){
 $tpl  = new Template(APPLICATION_HOME."/templates/tour", "keep");
 if ($active)
  $tpl->set_file(array("main" => "link_active.html")); 
 else
  $tpl->set_file(array("main" => "link_inactive.html")); 
 $tpl->set_var("URL", $url);
 $tpl->set_var("TEXT", $title);
 $tpl->parse("main", "main");
 return $tpl->get("main");
}

function print_register_link(){
 global $ltrstr, $sess;
 $tpl  = new Template(APPLICATION_HOME."/templates/tour", "keep");
 $tpl->set_file(array("main" => "link_active.html")); 
 $tpl->set_var("URL", $sess->url('/Experts/New_User?dologout=1'));
 $tpl->set_var("TEXT", $ltrstr['Register now']);
 $tpl->parse("main", "main");
 return $tpl->get("main");
}

function empty_line(){
 global $glob_language_name;
 return implode("\n", file(APPLICATION_HOME."/templates/$glob_language_name/tour/empty_line.html"));
}

function print_tour_navigation($active_id){
 global $tour_steps, $perm, $ltrstr;
 $str = "";
 
 for($x =0; $x<count($tour_steps); $x++){
  $str .= print_one_tour_link( ($x != ($active_id-1)), $tour_steps[$x][1], $tour_steps[$x][0])."\n";
 }
 if(!defined("COMMON_PERMISSIONS_INC"))
  include("commonapi/common_permissions.inc");
 $capabilities = get_caps($perm, '/'); // this is used at many places
 if ( has_caps($capabilities, CAP_REGISTER) ){
  $str .= empty_line();
  $str .= print_register_link();
 }
 return $str;
}
?>