<?php
/* $Id: tbl_alter.php3,v 1.1.1.1 2000-11-06 20:55:48 linktrai Exp $ */

require("header.inc.php3");

if (isset($submit))
   {
   if(!isset($query)) $query = "";
   $query .= " $field_orig[0] $field_name[0] $field_type[0] ";
   if ($field_length[0] != "")
      {
      $query .= "($field_length[0]) ";
      }
  if ($field_attribute[0] != "")
      {
      $query .= "$field_attribute[0] ";
      }
   if ($field_default[0] != "")
      {
      $query .= "DEFAULT '$field_default[0]' ";
      }
   $query .= "$field_null[0] $field_extra[0]";
   $query = stripslashes($query);
   $sql_query = "ALTER TABLE $table CHANGE $query";
   $result = mysql_db_query($db, "ALTER TABLE $table CHANGE $query");
   if (!$result)
      {
      mysql_die();
      }
   else
      {
      $message = "$strTable $table $strHasBeenAltered";
      include("tbl_properties.php3");
      exit;
      }
   }
else
   {
   $result = mysql_db_query($db, "SHOW FIELDS FROM $table LIKE '$field'");
   if (!$result)
      {
      mysql_die();
      }
   $num_fields = mysql_num_rows($result);
   $action = "tbl_alter.php3";
   require("tbl_properties.inc.php3");
   }

require ("footer.inc.php3");
?>
