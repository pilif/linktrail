#!/usr/local/bin/php -q

include("/home/linktrai/phplib/local.inc");

global $myDB;

 
if (!is_object($myDB)) {
  $myDB = new DB_Linktrail;
  include("/home/linktrai/includes/dbapi/sql_strs.inc");
  include("/home/linktrai/includes/dbapi/sql_util.inc");
}

$fp = fopen("/home/linktrai/test", "w");
foreach($argv as $arg){
 fputs($fp, $arg."\n");
}
fclose($fp);
