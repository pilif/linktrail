<?php
/* $Id: db_readdump.php3,v 1.1.1.1 2000-11-06 20:55:48 linktrai Exp $ */
@set_time_limit(10000);

function split_sql($sql)
{
    $sql = trim($sql);
    $buffer = array();
    $ret = array();
    $in_string = false;

    for($i=0; $i<strlen($sql); $i++)
    {
         if($sql[$i] == ";" && !$in_string)
        {
            $ret[] = substr($sql, 0, $i);
            $sql = substr($sql, $i + 1);
            $i = 0; 
        }

        if($in_string && ($sql[$i] == $in_string) && $buffer[0] != "\\")
        {
             $in_string = false;
        }
        elseif(!$in_string && ($sql[$i] == "\"" || $sql[$i] == "'") && $buffer[0] != "\\")
        {
             $in_string = $sql[$i];
        }
        if(isset($buffer[1]))
        {
            $buffer[0] = $buffer[1];
        }
        $buffer[1] = $sql[$i];
     }    
    
    if (!empty($sql))
    {
        $ret[] = $sql;
    }

    return($ret);
}

if(!empty($sql_file) && $sql_file != "none")
{
    $sql_query = fread(fopen($sql_file, "r"), filesize($sql_file));
}

$pieces  = split_sql($sql_query);

if (count($pieces) == 1)
   {
   $sql_query = trim($pieces[0]);
   include ("sql.php3");
   exit;
   }

include("header.inc.php3");
for ($i=0; $i<count($pieces); $i++)
    {
    $pieces[$i] = stripslashes(trim($pieces[$i]));
    $result = mysql_db_query ($db, $pieces[$i]) or mysql_die();
    $sql_query = stripslashes($sql_query);
    }
$message = $strSuccess;
include("db_details.php3");

?>
