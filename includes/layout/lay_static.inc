<?php
define("LAY_STATIC_INC", true);
/*
  lay_static.inc; 1.00
 */
 
if (!defined("DIRECTORY_INC"))
 include("dbapi/directory.inc");

if (!defined("LAY_DIRECTORY_INC"))
 include("layout/lay_directory.inc");

if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");

if (!defined("FORMS_INC"))
 include("commonapi/forms.inc");
 
function print_one_side_link($text, $url=""){
  global $sess;
  $tpl = new Template(APPLICATION_HOME."/templates/static", "keep");
  if ($url == "")
   $tpl->set_file(array("link" => "link_inactive.html"));
  else{
   $tpl->set_file(array("link" => "link_active.html"));
   $tpl->set_var("URL", $sess->url($url));
  } 
  $tpl->set_var("TEXT", $text);
  
  $tpl->parse("link", "link");
  return $tpl->get("link");  
}
 
function print_side_links($idx, $active_url, $rubrik){

 $str = "";
 $var = "static_$rubrik";
 global $$var;
 $arr = $$var; 
 foreach($arr[$idx] as $text => $arr){
  if ($active_url != $arr[0])
   $str .= print_one_side_link($text, $arr[0]);
  else
   $str .= print_one_side_link($text);
 }
 return $str;
} 
 
function print_sidebar($rubrik, $active_url){
 global $perm;
 if (!defined("COMMON_PERMISSIONS_INC"))
  include("commonapi/common_permissions.inc");
 $capabilities = get_caps($perm, '/'); // this is used at many places

 $tpl = new Template(APPLICATION_HOME."/templates/static/sidebars", "keep");
 $tpl->set_file(array("main" => "$rubrik.html"));

 $var = "static_$rubrik";
 global $$var;
 $arr = $$var; 
 foreach($arr as $key => $value)
  $tpl->set_var("LINKS_".strtoupper($key), print_side_links($key, $active_url, $rubrik));
 $tpl->set_var("IWANTTO", print_iwantto($active_url, $capabilities)); 
 $tpl->parse("main", "main");
 return $tpl->get("main");  
}

function print_static_page($path, $errors=false){
global $HTTP_POST_VARS, $PHP_SELF, $glob_userdata, $ltrstr;

$tpl = new Template(APPLICATION_HOME."/static_pages".dirname($path), "keep");
$tpl->set_file(array("main" => basename($path).".html"));
if ($errors == false){
 $errors = array();
 $tpl->set_var("ERRORS", "");
}else{
  $tpl->set_var("ERRORS", $ltrstr['Form Error']);
}
/* Before we begin parsing some form-vars, We can define
   template-Markers using a filler-template (that is 100% PHP-Code)
*/
if (file_exists(APPLICATION_HOME.'/includes/static-replacement/'.str_replace('/', '_', $path).'.inc'))
 include(APPLICATION_HOME.'/includes/static-replacement/'.str_replace('/', '_', $path).'.inc');
$fields = $tpl->get_undefined("main");
if (is_array($fields)){
 if (in_array('ACTION', $fields))
  $tpl->set_var("ACTION", $PHP_SELF);
 foreach($fields as $key){
  if(preg_match('/(.*)?_([A-Z]+)/', $key, $erg)){
   if ($erg[1] == "WARN"){
    $wflag = in_array("field_".strtolower($erg[2]), $errors) ? $ltrstr['Form Error Mark'] : "";
    $tpl->set_var($key, $wflag);
   }else{
    if (defined('FIELD_NAME_'.strtoupper($erg[2])))
     eval('$tpl->set_var($key, FIELD_NAME_'.strtoupper($erg[2]).');');
   }
  }else{
   $glob_userdata['DateTime'] = strftime("%D %T", time());
   $str = ($HTTP_POST_VARS['field_'.strtolower($key)] != "") ? $HTTP_POST_VARS['field_'.strtolower($key)] : "";
   if ($str == "")
    eval('$str = (defined("HASH_ENTRY_".strtoupper($key))) ? $glob_userdata[HASH_ENTRY_'.strtoupper($key).'] : "";');
   $tpl->set_var($key, $str);
  }
 }
 if (in_array('ACTION', $fields))
  $tpl->set_var("ACTION", $PHP_SELF);
}
$tpl->parse("main", "main");
return $tpl->get("main");  
}

function parse_static_form($file){
 global $HTTP_POST_VARS;
 include("form-scripts/$file");
 //imported using the include-statement above...
 foreach($required_fields as $field)
  if ( (!isset($HTTP_POST_VARS[$field])) or ($HTTP_POST_VARS[$field] == "") )
   $errors[] = $field;

 foreach($valid_fields as $field => $validation_method)
  if (!is_valid_field($HTTP_POST_VARS[$field], $validation_method) )
   $errors[] = $field;
 
 if (is_array($errors))
  return $errors;
    
 $bodytext = parse_form_mail_template($HTTP_POST_VARS, $mail_body);
 $header  = "From: $mail_from\n";
 $header .= "X-Mailer: Linktrail/0.1 (PHP/". phpversion().")\n";
 foreach($mailto as $recp)
  mail($recp, $mail_subject, $bodytext, $header);
 return $ok_url;
}

function get_static_title($url, $rubrik){
 $var = "static_$rubrik";
 global $$var;
 $arr = $$var; 
 
 foreach($arr as $key => $value){
  foreach($value as $title => $data){
   if ($data[0] == $url)
    return ($data[1] != "") ? $data[1] : str_replace("_", " ", basename($url));
  }
 }
 return  str_replace("_", " ", basename($url));
}
?>