#!/usr/local/bin/php -q
<?php

define("BOUNCE_OK", 'ok');
define("BOUNCE_ONCE", "bounce");
define("BOUNCE_CHECK", "bounce_test");
define("BOUNCE_FAIL", "fail");
define("BOUNCE_ERR", -1);

function sql_error($error, $empty){
 die("SQL-Error: ".$error['sql_errdesc']);
}

function get_bounce_state($address){
 global $myDB;
 
 $query = sprintf("SELECT BounceFlag FROM ltrUserData WHERE Email = '%s'", $address);
// die($query);
 if (!$myDB->query($query)) return BOUNCE_ERR;
 if ($myDB->num_rows() == 0) return BOUNCE_ERR;
 $myDB->next_record();
 return $myDB->f("BounceFlag");
}

function set_bounce_flag($address, $flag){
 global $myDB;
 
 $query = sprintf("UPDATE ltrUserData SET BounceFlag = '%s' WHERE Email = '%s'", $flag, $address);
 $myDB->query($query);
}

function decode_address($str){
 return base64_decode(preg_replace('/\.([a-z])/e', "strtoupper('\\1')", $str));
}


// Begin main //
ini_set("include_path", "./:/home/linktrai:/home/linktrai/templates:/home/linktrai/includes:/usr/local/phplib");
define("APPLICATION_HOME", "/home/linktrai");
if (empty($_PHPLIB))
 $_PHPLIB = "";

if (!is_array($_PHPLIB)) {
  $_PHPLIB["libdir"] = "/usr/local/phplib/"; 
}
require("config/config.inc");
require($_PHPLIB["libdir"] . "db_mysql.inc");  
require($_PHPLIB["libdir"] . "ct_sql.inc");    
require($_PHPLIB["libdir"] . "session.inc");   
require($_PHPLIB["libdir"] . "auth.inc");      
require($_PHPLIB["libdir"] . "perm.inc");      
require($_PHPLIB["libdir"] . "user.inc");      
require("phplib/local.inc");
require($_PHPLIB["libdir"] . "page.inc");

global $myDB;

if (!is_object($myDB)) {
  $myDB = new DB_Linktrail;
  include("dbapi/sql_strs.inc");
  include("dbapi/sql_util.inc");
}

$address = decode_address($argv[1]);

if ( (isset($argv[2])) and ($argv[2] == "o")){
 print("Debug: $address\n");
}else{
 $fp = fopen("/home/linktrai/bounce.log", "a");
 fputs($fp, "Doing bounce-code for: $address\n");
 fclose($fp);
}
$flag = get_bounce_state($address);
if ($flag == BOUNCE_ERR) exit(0); //no error code. This would lead to
                                  //bounce being generated. -> loop
/* 
   BOUNCE_ONCE is never the state of a mail that fails. The mailer-
   script sets the Flag to bounce_check before sending the mail
*/
$old_flag = $flag;
if ($flag == BOUNCE_OK)
 $flag = BOUNCE_ONCE;
elseif($flag == BOUNCE_CHECK)
 $flag = BOUNCE_FAIL;

if ($flag != $old_flag)
 set_bounce_flag($address, $flag);

?>
