<?php
/* $Id: db_create.php3,v 1.1.1.1 2000-11-06 20:55:48 linktrai Exp $ */

require("header.inc.php3");

$result = mysql_query("CREATE DATABASE $db");
if (!$result)
   {
   mysql_die();
   }
else
   {
   $message = "$strDatabase $db $strHasBeenCreated";
   require("db_details.php3");
   }

?>
