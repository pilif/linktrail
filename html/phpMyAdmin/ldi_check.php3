<?php
/* $Id: ldi_check.php3,v 1.1.1.1 2000-11-06 20:55:48 linktrai Exp $ */


/* This file checks and builds the sql-string for
LOAD DATA INFILE 'file_name.txt' [REPLACE | IGNORE] INTO TABLE table_name
    [FIELDS
        [TERMINATED BY '\t']
        [OPTIONALLY] ENCLOSED BY "]
        [ESCAPED BY '\\' ]]
    [LINES TERMINATED BY '\n']
    [(column_name,...)]
*/
if (isset($btnLDI) && ($textfile != "none"))
{	$query = "LOAD DATA INFILE '$textfile' $replace INTO TABLE $into_table ";
	if (isset($field_terminater))
	{	$query = $query . "FIELDS TERMINATED BY '".stripslashes($field_terminater)."' ";
	}

	if (strlen($enclose_option)>0)
	{	$query = $query . "OPTIONALLY ";
	}
	
	if (strlen($enclosed)>0)
	{	$query = $query . "ENCLOSED BY '$enclosed' ";
	}

	if (strlen($escaped)>0)
	{	$query = $query . "ESCAPED BY '".stripslashes($escaped)."' ";
	}

	if (strlen($line_terminator)>0)
	{	$query = $query . "LINES TERMINATED BY '".stripslashes($line_terminator)."' ";
	}

	if (strlen($column_name)>0)
	{	$query = $query . "($column_name)";
	}
        $sql_query = addslashes($query);
        require("sql.php3");
}
else
{	require("ldi_table.php3");
}
?>
