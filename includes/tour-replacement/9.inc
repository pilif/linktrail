<?php
 global $auth, $sess, $perm;

function print_reg_link_content(){
 global $ltrstr, $sess;
 $tpl  = new Template(APPLICATION_HOME."/templates/tour", "keep");
 $tpl->set_file(array("main" => "link_simple.html")); 
 $tpl->set_var("URL", $sess->url('/Experts/New_User?dologout=1'));
 $tpl->set_var("TEXT", $ltrstr['Register now']);
 $tpl->parse("main", "main");
 return $tpl->get("main");
}

 if(!defined("COMMON_PERMISSIONS_INC"))
  include("commonapi/common_permissions.inc");

$capabilities = get_caps($perm, '/'); // this is used at many places
if ( has_caps($capabilities, CAP_REGISTER) ){
 $tpl->set_var("REGLINK", print_reg_link_content());
}else{
 $tpl->set_var("REGLINK", "");
}
?>