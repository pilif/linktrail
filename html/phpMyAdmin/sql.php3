<?php
/* $Id: sql.php3,v 1.1.1.1 2000-11-06 20:55:48 linktrai Exp $ */;

require("lib.inc.php3");
$no_require = true;
// Go back to further page if table should not be dropped
if (isset($btnDrop) && $btnDrop == $strNo)
   {
   if (file_exists($goto))
      {
      include($goto);
      }
   else
      {
      Header("Location: $goto");
      }
   exit;
   }

// Check if table should be dropped
$is_drop_sql_query = eregi("DROP +TABLE|DATABASE|ALTER TABLE +[[:alnum:]]* +DROP|DELETE FROM", $sql_query); // Get word "drop"

if (!$cfgConfirm)
   {
   $btnDrop = $strYes;
   }
if ($is_drop_sql_query && !isset($btnDrop))
   {
   include("header.inc.php3");
   echo $strDoYouReally.urldecode(stripslashes($sql_query))."?<br>";
   ?>
   <form action="sql.php3" method="post" enctype="application/x-www-form-urlencoded">
   <input type="hidden" name="sql_query" value="<?php echo urldecode(stripslashes($sql_query)); ?>">
   <input type="hidden" name="server" value="<?php echo $server ?>">
   <input type="hidden" name="db" value="<?php echo $db ?>">
   <input type="hidden" name="zero_rows" value="<?php echo $zero_rows;?>">   
   <input type="hidden" name="table" value="<?php echo $table;?>">   
   <input type="hidden" name="goto" value="<?php echo $goto;?>">
   <input type="hidden" name="reload" value="<?php echo $reload;?>">   
   <input type="Submit" name="btnDrop" value="<?php echo $strYes; ?>">
   <input type="Submit" name="btnDrop" value="<?php echo $strNo; ?>">
   </form>
   <?php 
   } 

// if table should be dropped or other queries should be perfomed
//elseif (!$is_drop_sql_query || $btnDrop == $strYes) 
else
       {
       $sql_query = isset($sql_query) ? stripslashes($sql_query) : '';
       $sql_order = isset($sql_order) ? stripslashes($sql_order) : '';
       $sql_limit = isset($pos) ? " LIMIT $pos, $cfgMaxRows" : '';
       $result = mysql_db_query($db, $sql_query.$sql_order.$sql_limit);
       // abfrage nochmal ohne limit
       if(eregi("^SELECT", $sql_query))
       {
           $OPresult = mysql_db_query($db, $sql_query.$sql_order);
           $SelectNumRows = @mysql_num_rows($OPresult);
       }
       //

       if (!$result)
          {
          $error = mysql_error();
          include("header.inc.php3");
          mysql_die($error);
          }
      $num_rows = @mysql_num_rows($result);
      
      if ($num_rows < 1)
         {
        if (file_exists($goto)) 
           {
           include("header.inc.php3");
           if (isset($zero_rows) && !empty($zero_rows))
              $message = $zero_rows;
           else
              $message = $strEmptyResultSet;
           include($goto); 
           }
         else
           {
           $message = $zero_rows;
           Header("Location: $goto");
           }
         exit;
         }
      else
         {
         include("header.inc.php3");
         display_table($result);
         if (!eregi("SHOW VARIABLES|SHOW PROCESSLIST|SHOW STATUS", $sql_query))
	     echo "<p><a href=\"tbl_change.php3?server=$server&db=$db&table=$table&goto=sql.php3?".urlencode($GLOBALS['QUERY_STRING'])."\"> $strInsertNewRow</a></p>";
         } 
      }	
require ("footer.inc.php3");
?>
