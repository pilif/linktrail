<?php
/* $Id: tbl_create.php3,v 1.1.1.1 2000-11-06 20:55:48 linktrai Exp $ */

require("header.inc.php3");

if (isset($submit))
   {
   if(!isset($query)) $query = "";
   for ($i=0; $i<count($field_name); $i++)
       {
       $query .= "$field_name[$i] $field_type[$i] ";
       if ($field_length[$i] != "")
          {
          $query .= "(".stripslashes($field_length[$i]).") ";
          }
       if ($field_attribute[$i] != "")
          {
          $query .= "$field_attribute[$i] ";
          }
       if ($field_default[$i] != "")
          {
          $query .= "DEFAULT '".stripslashes($field_default[$i])."' ";
          }
       $query .= "$field_null[$i] $field_extra[$i], ";
       }
   $query = ereg_replace(", $", "", $query);

   if(!isset($primary)) $primary = "";
   for ($i=0;$i<count($field_primary);$i++)
       {
       $j = $field_primary[$i];
       $primary .= "$field_name[$j], ";
       }
   $primary = ereg_replace(", $", "", $primary);
   if (count($field_primary) > 0)
      {
      $primary = ", PRIMARY KEY ($primary)";
      }
 
   if(!isset($index)) $index = "";
   for ($i=0;$i<count($field_index);$i++)
       {
       $j = $field_index[$i];
       $index .= "$field_name[$j], ";
       }
   $index = ereg_replace(", $", "", $index);
   if (count($field_index) > 0)
      {
      $index = ", INDEX ($index)";
      }
   if(!isset($unique)) $unique = "";
   for ($i=0;$i<count($field_unique);$i++)
       {
       $j = $field_unique[$i];
       $unique .= "$field_name[$j], ";
       }
   $unique = ereg_replace(", $", "", $unique);
   if (count($field_unique) > 0)
      {
      $unique = ", UNIQUE ($unique)";
      }
   $query_keys = $primary.$index.$unique;
   $query_keys = ereg_replace(", $", "", $query_keys);

   // echo "$query $query_keys";
   $sql_query = "CREATE TABLE ".$table." (".$query." ".$query_keys.")";
   if (MYSQL_MAJOR_VERSION == "3.23" && !empty($comment))
      $sql_query .= " comment = '$comment'";
   $result = mysql_db_query($db, $sql_query);
   if (!$result)
      {
      mysql_die();
      }
   $message = "$strTable $table $strHasBeenCreated";
   include("tbl_properties.php3");
   exit;
   }
else
   {
   $action = "tbl_create.php3";
   require("tbl_properties.inc.php3");
   }

require ("footer.inc.php3");
?>
