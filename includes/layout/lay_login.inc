<?php
define("LAY_LOGIN_INC", true);

require_once('thirdparty/recaptchalib.php');

if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");

 if (!defined("COMMON_ERRORS_INC"))
 include("commonapi/common_errors.inc");

function print_loginform_trail($action_login, $current_user, $logintext, $failed=false){
 global $ltrstr, $sess, $HTTP_GET_VARS;
 
 if (!class_exists("Template"))
  include("template.inc");
 $tpl = new Template(APPLICATION_HOME."/templates/login", "keep");
 $tpl->set_file(array("main" => "login_trail.html"));
 
 if ($failed)
   $tpl->set_var("ERROR_STRING", '<font size=2 face="Verdana, Arial, Helvetica, Sans-Serif"><font color="'.ERR_COLOR.'">'.$ltrstr['Error'].'</font>'.$ltrstr['Login failed'].'</font>');
 else
   $tpl->set_var("ERROR_STRING", '');

 $tpl->set_var("FORGOTTEN_URL", $sess->url('/Experts/Forgotten_Password?dologout=1'));
 $tpl->set_var("LOGINTEXT", $logintext);
 $tpl->set_var("ACTION", $action_login);
 $tpl->set_var("OTHER_HIDDEN", make_hidden('action', $HTTP_GET_VARS['action']).make_hidden('kat', $HTTP_GET_VARS['kat']));
 $tpl->set_var("SESSIONID", $sess->id);
 $tpl->set_var("FIELD_NAME", "username");
 $tpl->set_var("VALUE_NAME", $current_user);
 $tpl->set_var("FIELD_PASSWORD", "password");
 $tpl->set_var("VALUE_PASSWORD", "");
 $tpl->parse("main", "main");
 return $tpl->get("main");
}

function print_password_request_form(){
 global $PHP_SELF, $kat, $capabilities;

$tpl = new Template(APPLICATION_HOME."/templates/login", "keep");
$tpl->set_file(array("main" => "password_query_form.html"));
$tpl->set_var("ACTION", $PHP_SELF);
$tpl->parse("main", "main");
return $tpl->get("main");
}


function print_password_request_answer(){
 global $PHP_SELF;
 
$tpl = new Template(APPLICATION_HOME."/templates/login", "keep");
$tpl->set_file(array("main" => "password_query_answer.html"));
$tpl->parse("main", "main");
return $tpl->get("main");
}

function print_password_request($done = false, $kat="", $capabilities=0){
 $tpl = new Template(APPLICATION_HOME."/templates/login", "keep");
  $tpl->set_file(array("main" => "login_main.html"));

 if ($done)
  $form = print_password_request_answer();
 else
  $form = print_password_request_form();
 
 $tpl->set_var("FIRST_FIELD", $form);
 $tpl->set_var("SECOND_FIELD", "");
 
 if (!defined("LAY_DIRECTORY_INC"))
  include("layout/lay_directory.inc");
  
 $tpl->set_var("IWANTTO", print_iwantto($kat, $capabilities)); 
 $tpl->parse("main", "main");
 return $tpl->get("main");
}

function print_login_main($type, $action_login, $action_register, $current_user="", $failed=false, $error=0, $suggestions="", $email="", $username=""){
 global $perm;
 $tpl = new Template(APPLICATION_HOME."/templates/login", "keep");

 if ($type == 1){
  $tpl->set_file(array("main" => "login_main.html"));
  $login    = print_login_form_main($action_login, $current_user, $failed);
  $register = print_register_link();
  $tpl->set_var("FIRST_FIELD", $login);
  $tpl->set_var("SECOND_FIELD", $register);

  if (!defined("COMMON_PERMISSIONS_INC"))
   include("commonapi/common_permissions.inc");
// die("a");
  if (!defined("LAY_DIRECTORY_INC"))
   include("layout/lay_directory.inc");
//die("a");
  $capabilities = get_caps($perm, '/');
  
  $tpl->set_var("IWANTTO", print_iwantto('/', $capabilities));
 }else{
  $tpl->set_file(array("main" => "register_main.html"));
  $register = print_register_form_main($action_register, $error, $suggestions, $email, $username);
  $login    = print_login_form_main($action_login, $current_user, false, false);
  $tpl->set_var("FIRST_FIELD", $register);
  $tpl->set_var("SECOND_FIELD", $login);
  if (!defined("COMMON_PERMISSIONS_INC"))
   include("commonapi/common_permissions.inc");
  if (!defined("LAY_DIRECTORY_INC"))
   include("layout/lay_directory.inc");
   
  
  $capabilities = get_caps($perm, '/Experts/New_User');
  
  $tpl->set_var("IWANTTO", print_iwantto('/Experts/New_User', $capabilities));
 }
 $tpl->parse("main", "main");
 return $tpl->get("main");
}

function print_register_link(){
 global $sess; 
 
 $tpl = new Template(APPLICATION_HOME."/templates/login", "keep");
 $tpl->set_file(array("main" => "register_link.html"));
 $tpl->set_var("JOINURL", $sess->url("/Experts/New_User"));
 $tpl->parse("main", "main");
 return $tpl->get("main"); 
}

function print_error_string_login($errmsg){
$tpl = new Template(APPLICATION_HOME."/templates/login", "keep");
$tpl->set_file(array("main" => "error_login.html"));
$tpl->set_var("ERROR", $errmsg);
$tpl->parse("main", "main");
return $tpl->get("main"); 
} 
 
function print_login_form_main($action_url, $current_user="", $failed=false, $backlink=true){
 global $ltrstr, $PHP_SELF, $sess;
 
 $tpl = new Template(APPLICATION_HOME."/templates/login", "keep");
 if ($backlink)
  $tpl->set_file(array("main" => "existing_user.html"));
 else
  $tpl->set_file(array("main" => "existing_user_noback.html"));
 
 
 if ($failed)
   $tpl->set_var("ERROR_STRING", print_error_string_login($ltrstr['Login failed']));
 else
   $tpl->set_var("ERROR_STRING", '');
 if ($backlink)
  $tpl->set_var("BACKURL", $PHP_SELF."?dologout=1");
 $tpl->set_var("ACTION", $action_url);
 $tpl->set_var("FORGOTTEN_URL", $sess->url('/Experts/Forgotten_Password?dologout=1') );
 $tpl->set_var("FIELD_NAME", "username");
 $tpl->set_var("VALUE_NAME", $current_user);
 $tpl->set_var("FIELD_PASSWORD", "password");
 $tpl->set_var("VALUE_PASSWORD", "");
 $tpl->parse("main", "main");
 return $tpl->get("main");

}

function print_errors($errarr){
 if (!is_array($errarr))
  return "";
 $tpl = new Template(APPLICATION_HOME."/templates/login", "keep");
 $tpl->set_file(array("main" => "new_error.html"));
 
  $tpl->set_block("main", "error", "errors");
 # define NUM and BIGNUM, then append "row" to "rows"...
 for($x=0; $x < count($errarr); $x++) {
   $tpl->set_var("ERROR_STRING", $errarr[$x]);
   $tpl->parse("errors", "error", true);
 }
 $tpl->parse("main", "main");
 return $tpl->get("main");
}

function print_username_fields($suggestions, $predef_uname){
$tpl = new Template(APPLICATION_HOME."/templates/login", "keep");
if (is_array($suggestions)){
 $tpl->set_file(array("main" => "new_username_exist.html"));
 $tpl->set_var("SUGGESTION_COUNT", count($suggestions));
 $tpl->set_var("SUGGESTION_COUNT_MAX", count($suggestions)+1);
 $tpl->set_var("FIELD_USERNAME_SUGGESTION", 'field_username_suggestion');
 $tpl->set_var("FIELD_USERNAME_TXT", 'field_username_txt');
 $tpl->set_block("main", "suggestion", "suggestions");
 for($x=0; $x < count($suggestions); $x++) {
   $tpl->set_var("FIELD_USERNAME_SUGGESTION", 'field_username_suggestion');
   $tpl->set_var("VALUE_SUGGESTION", $suggestions[$x]);
   $tpl->set_var("SUGGESTION_IDX", $x);
   $tpl->parse("suggestions", "suggestion", true);
 }
}else{
 $tpl->set_file(array("main" => "new_username_std.html"));
 $tpl->set_var("FIELD_USERNAME", "field_username");
 $tpl->set_var("VALUE_USERNAME", $predef_uname);
}
 $tpl->parse("main", "main");
 return $tpl->get("main");
}

function print_register_form_main($action_url, $errors="", $suggestions="", $predef_email="", $predef_username=""){
 global $ltrstr, $PHP_SELF, $sess;
 $tpl = new Template(APPLICATION_HOME."/templates/login", "keep");
 $tpl->set_file(array("main" => "new_user.html"));
  if ( ($errors & ERR_PASSWORDMATCH) != 0)
  $errarr[] .= $ltrstr['Pass_Nomatch'];
 if ( ($errors & ERR_USEREXISTS) != 0)
  $errarr[] .= $ltrstr['User Exists'];
 if ( ($errors & ERR_NOUSERNAME) != 0)
  $errarr[] .= $ltrstr['No Username Provided'];
 if ( ($errors & ERR_NOEMAIL) != 0)
  $errarr[] .= $ltrstr['No Email Provided'];
 if ( ($errors & ERR_CAPTCHA) != 0)
  $errarr[] .= $ltrstr['Captcha verification failed'];

 $tpl->set_var("BACKURL", $sess->url("/"));
 $tpl->set_var("ACTION", $sess->url("/Experts/New_User"));
 $tpl->set_var("ERRORS", print_errors($errarr));
 $tpl->set_var("USERNAME", print_username_fields($suggestions, $predef_username));
 $tpl->set_var("FIELD_EMAIL", 'field_email');
 $tpl->set_var("VALUE_EMAIL", $predef_email);
 $tpl->set_var("FIELD_PASSWORD1", 'field_password1');
 $tpl->set_var("FIELD_PASSWORD2", 'field_password2');
 $tpl->set_var('RECAPTCHA', recaptcha_get_html('6LcvTQoAAAAAAJE4O775YtFdRW1JcdqZs_DvIalD'));

 $tpl->parse("main", "main");
 return $tpl->get("main");
}
?>
