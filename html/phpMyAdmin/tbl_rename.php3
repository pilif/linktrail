<?php
/* $Id: tbl_rename.php3,v 1.1.1.1 2000-11-06 20:55:48 linktrai Exp $ */
$old_name = $table;
$table = $new_name;
require("header.inc.php3");

$result = mysql_db_query($db, "ALTER TABLE $old_name RENAME $new_name") or mysql_die();
$table = $old_name;
eval("\$message =  \"$strRenameTableOK\";");
$table = $new_name;
include("tbl_properties.php3");
exit;

require ("footer.inc.php3");
?>
