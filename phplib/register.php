<?php
if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");

if (!defined("LAY_LOGIN_INC"))
 include("layout/lay_login.inc");

include("template.inc");

if (isset($node))
 $kat = base64_decode($node);
else
 $kat = $PATH_INFO;

$kat = trim($kat);

//for the commonheader-template
$pl  = build_pathlist($kat, false);
$plf = build_pathlist($kat, true, true); 
$restriction_list = build_restriction_list($kat);
global $glob_language_name;

$in_login = true;
$username = $this->auth["uname"];
$in_login = true;

include("commonheader2.html");
global $username, $ltrstr, $PHP_SELF; 
$failed = isset($username); 
print(print_login_main(1, $PHP_SELF, "", $username, $failed));
include("commonfooter2.html"); 
?>                            

