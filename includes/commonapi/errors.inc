<?php
define("ERRORS_INC", true);

function sql_error($error, $in_trail = false){
 if ($in_trail)
  include('errors/error_sql_intrail.html');
 else
  include('errors/error_sql_notrail.html');
}

function error($errtitle, $errstr, $line, $file){
 global $ltrstr;
 
printf('<b><font face="Verdana, Arial, Helvetica, sans-serif" size="2" color="%s">%s</font>
        <font face="Verdana, Arial, Helvetica, sans-serif" size="2" >%s</b><p>', ERR_COLOR, $ltrstr['Error'], $errtitle);
echo("$errstr");
?>
<p>
File: <?= $file ?><br>
Line: <?= $line ?>
<?
die();
}

?>