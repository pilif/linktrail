<?php page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm")); ?>
<html>
<head>

<LINK REL=stylesheet HREF="/styles/trail_1.css" TYPE="text/css">

<body>
<?
//phpinfo();
include("commonapi/iwantto.inc");
$kat = '/';

$str = print_navigation($kat);
echo $str;
?> 
</body>
</html> 
<?php page_close(); ?>                           
