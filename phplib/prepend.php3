<?php

//ripped from http://www.php-center.de/artikel/show.php3?id=33
function start_timer($name="default") { 
 global $glob_timers; 
 
 $microtime=explode( " ", microtime()); 
 $glob_timers[$name]=$microtime[1]+$microtime[0]; 
}

//ripped from http://www.php-center.de/artikel/show.php3?id=33
function stop_timer($stellen, $name="default") { 
 global $glob_timers; 

 $microtime=explode( " ", microtime()); 
 $script_stop_time=$microtime[1]+$microtime[0]; 
 return number_format(($script_stop_time-$glob_timers[$name]),$stellen); 
}

start_timer(); 

$host_language['prototyp.linktrail.work'] = 1;
$host_language['prototypde.linktrail.work'] = 2;
$language_host[1] = 'prototyp.linktrail.work';
$language_host[2] = 'prototypde.linktrail.work';
$language_name[1] = "en";
$language_name[2] = "de";
$glob_language = $host_language[strtolower($HTTP_HOST)];
if (!$glob_language)
 $glob_language = 1;
$glob_language_name = $language_name[$glob_language];

define("TEMPLATE_ROOT", "/home/linktrai/templates/$glob_language_name/");

//We are setting a new include-path...
$inc = ini_get('include_path');
$inc = str_replace('/home/linktrai/templates/', TEMPLATE_ROOT, $inc);
ini_set("include_path", $inc);
//die($inc);

require("config/config.inc");
require("commonapi/errors.inc");

if (empty($_PHPLIB))
 $_PHPLIB = "";
 
if (!is_array($_PHPLIB)) {
# Aren't we nice? We are prepending this everywhere 
# we require or include something so you can fake
# include_path  when hosted at provider that sucks.
  $_PHPLIB["libdir"] = "/usr/local/phplib/"; 
}

require($_PHPLIB["libdir"] . "db_mysql.inc");  /* Change this to match your database. */
require($_PHPLIB["libdir"] . "ct_sql.inc");    /* Change this to match your data storage container */
require($_PHPLIB["libdir"] . "session.inc");   /* Required for everything below.      */
require($_PHPLIB["libdir"] . "auth.inc");      /* Disable this, if you are not using authentication. */
require($_PHPLIB["libdir"] . "perm.inc");      /* Disable this, if you are not using permission checks. */
require($_PHPLIB["libdir"] . "user.inc");      /* Disable this, if you are not using per-user variables. */ 
/* Additional require statements go below this line */

/* Additional require statements go before this line */

require("local.inc");     /* Required, contains your local configuration. */

require($_PHPLIB["libdir"] . "page.inc");      /* Required, contains the page management functions. */


srand ((double)microtime()*1000000);
$randval = rand(11111,99999);
?>