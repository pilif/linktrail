<?php
if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");

if (!defined("DIRECTORY_INC"))
 include("dbapi/directory.inc");
 
if (!defined("LAY_LOGIN_INC"))
 include("layout/lay_login.inc");

if (!defined("LAY_TRAIL_INC"))
 include("layout/lay_trail.inc");

global $username, $ltrstr, $sess, $HTTP_POST_VARS, $HTTP_GET_VARS, $mytrail, $PHP_SELF, $PATH_INFO; 
$failed = isset($username); 
if (is_trail($PHP_SELF)){
 $mytrail = get_node_info($PHP_SELF);
}
if (!is_array($mytrail)){
 $mytrail = get_node_info($PATH_INFO);
}

if ($in_trail == 1){ 
 //die(class_exists("Template"));
 if (!class_exists("Template"))
  include("template.inc");
 $tpl = new Template("/home/linktrai/templates/trail", "keep");
 $tpl->set_file(array("simpleframe" => "simpleframe.html"));
 $tpl->set_var("CONTENT", print_loginform_trail($PHP_SELF, $this->auth["uname"], $ltrstr['Trail_Login'],$failed));
 $tpl->parse("simpleframe", "simpleframe");
 global $action, $kat;
 $action = $HTTP_GET_VARS['action'];
 $kat = $HTTP_GET_VARS['kat'];
 //die($action);
 $sess->register("kat"); $sess->register("action");
 print $tpl->get("simpleframe");
}elseif ($in_trail == 2){
 print(msg_box($ltrstr['LOGIN'], print_loginform_trail(build_good_url($PHP_SELF), $this->auth["uname"], $ltrstr['Trail_Login_Add'],$failed), $mytrail, $mytrail['path']."?dologout=1", 0, $ltrstr['Back without logging in']));
}elseif ($in_trail == 3){
 print(msg_box($ltrstr['LOGIN'], print_loginform_trail(build_good_url($PHP_SELF), $this->auth["uname"], $ltrstr['Trail_Login_Sub'],$failed), $mytrail, $mytrail['path']."?dologout=1", 0, $ltrstr['Back without logging in'])); 
}elseif ($in_trail == 4){
 global $test;

 print(msg_box($ltrstr['LOGIN'], print_loginform_trail(build_good_url($PHP_SELF), $this->auth["uname"], $ltrstr['Trail_Login_Req'],$failed), $mytrail, $mytrail['path']."?dologout=1", 0, $ltrstr['Back without logging in']));
} 
?>                            

