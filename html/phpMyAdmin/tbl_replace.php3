<?php
/* $Id: tbl_replace.php3,v 1.1.1.1 2000-11-06 20:55:48 linktrai Exp $ */

require("lib.inc.php3");
$no_require = true;

reset($fields);
reset($funcs);

if (isset($primary_key))
   {
   $primary_key = stripslashes($primary_key);
   $valuelist = '';
   while (list($key, $val) = each($fields))
      {
      switch (strtolower($val)) {
	  case 'null':
		break;
	  case '$set$':
		$f = "field_$key";
		$val = "'".($$f?implode(',',$$f):'')."'";
		break;
	  default:
         $val = "'$val'";
		 break;
	  }
      if (empty($funcs[$key]))
         {
         $valuelist .= "$key = $val, ";
         }
      else
         {
         $valuelist .= "$key = $funcs[$key]($val), ";
         }
      }
   $valuelist = ereg_replace(', $', '', $valuelist);
   $query = "UPDATE $table SET $valuelist WHERE $primary_key";
   }
else
   {
   $fieldlist = '';
   $valuelist = '';
   while (list($key, $val) = each($fields))
      {
      $fieldlist .= "$key, ";
      switch (strtolower($val)) {
	  case 'null':
		break;
	  case '$set$':
		$f = "field_$key";
		$val = "'".($$f?implode(',',$$f):'')."'";
		break;
	  default:
         $val = "'$val'";
		 break;
	  }
      if (empty($funcs[$key]))
         {
         $valuelist .= "$val, ";
         }
      else
         {
         $valuelist .= "$funcs[$key]($val), ";
         }
      }
   $fieldlist = ereg_replace(', $', '', $fieldlist);
   $valuelist = ereg_replace(', $', '', $valuelist);
   $query = "INSERT INTO $table ($fieldlist) VALUES ($valuelist)";
   }
$sql_query = $query;
$result = mysql_db_query($db, $query);
if (!$result)
   {
   $error = mysql_error();
   include("header.inc.php3");
   mysql_die($error);
   }
else
   {
   if (file_exists($goto))
      {
      include("header.inc.php3");
      $message = $strModifications;
      include($goto);
      }
   else
      Header("Location: $goto");
   exit;
   }

//require ("footer.inc.php3");
?>
