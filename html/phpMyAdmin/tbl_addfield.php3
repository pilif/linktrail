<?php
/* $Id: tbl_addfield.php3,v 1.1.1.1 2000-11-06 20:55:48 linktrai Exp $ */

require("header.inc.php3");

if (isset($submit))
   {
   $query = '';
   for ($i=0; $i<count($field_name); $i++)
       {
       $query .= "$field_name[$i] $field_type[$i] ";
       if ($field_length[$i] != "")
          {
          $query .= "(".stripslashes($field_length[$i]).") ";
          }
       if ($field_attribute[$i] != "")
          {
          $query .= "$field_attribute[$i] " ;
          }
       if ($field_default[$i] != "")
          {
          $query .= "DEFAULT '".stripslashes($field_default[$i])."' ";
          }
       $query .= "$field_null[$i] $field_extra[$i], ADD ";
       }
   $query = stripslashes(ereg_replace(", ADD $", "", $query));

   $sql_query = "ALTER TABLE $table ADD $query";
   $result = mysql_db_query($db, "ALTER TABLE $table ADD $query");

   $primary = '';
   for ($i=0;$i<count($field_primary);$i++)
       {
       $j = $field_primary[$i];
       $primary .= "$field_name[$j], ";
       }
   $primary = ereg_replace(", $", "", $primary);
   if (count($field_primary) > 0)
      {
      $primary = "ADD PRIMARY KEY ($primary)";
      $sql_query .= "\nALTER TABLE $table $primary";
      $result = mysql_db_query($db, "ALTER TABLE $table $primary") or mysql_die();
      }
 
   $index = '';
   for ($i=0;$i<count($field_index);$i++)
       {
       $j = $field_index[$i];
       $index .= "$field_name[$j], ";
       }
   $index = ereg_replace(", $", "", $index);
   if (count($field_index) > 0)
      {
      $index = "ADD INDEX ($index)";
      $sql_query .= "\nALTER TABLE $table $index";
      $result = mysql_db_query($db, "ALTER TABLE $table $index") or mysql_die();
      }
   $unique = '';
   for ($i=0;$i<count($field_unique);$i++)
       {
       $j = $field_unique[$i];
       $unique .= "$field_name[$j], ";
       }
   $unique = ereg_replace(", $", "", $unique);
   if (count($field_unique) > 0)
      {
      $unique = "ADD UNIQUE ($unique)";
      $sql_query .= "\nALTER TABLE $table $unique";
      $result = mysql_db_query($db, "ALTER TABLE $table $unique") or mysql_die();
      }
   $query_keys = $primary.$index.$unique;	
   $query_keys = ereg_replace(", $", "", $query_keys);
   
   // echo "ALTER TABLE $table ADD ".$query.$query_keys;
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
   $action = "tbl_addfield.php3";
   require("tbl_properties.inc.php3");
   }

require ("footer.inc.php3");
?>
