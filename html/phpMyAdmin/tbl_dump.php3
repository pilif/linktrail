<?php
/* $Id: tbl_dump.php3,v 1.1.1.1 2000-11-06 20:55:48 linktrai Exp $ */
@set_time_limit(600);
$crlf="\n";
if (empty($asfile)) { 
	include("header.inc.php3");
	print "<div align=left><pre>\n";

} else {
	include("lib.inc.php3");
        $ext = "sql";
        if ($what == "csv")
           $ext = "csv";
        header("Content-disposition: filename=$table.$ext");
	header("Content-type: application/octetstream");
	header("Pragma: no-cache");
	header("Expires: 0");
	// doing some DOS-CRLF magic...
	$client=getenv("HTTP_USER_AGENT");
	if (ereg('[^(]*\((.*)\)[^)]*',$client,$regs)) {
		$os = $regs[1];
		// this looks better under WinX
		if (eregi("Win",$os)) $crlf="\r\n";
	}
}

function my_handler($sql_insert)
 {
 global $crlf, $asfile;
 if (empty($asfile))
    {
    echo htmlspecialchars("$sql_insert;$crlf");
    }
 else
    {
    echo "$sql_insert;$crlf";
    }
 }
function my_csvhandler($sql_insert)
 {
 global $crlf, $asfile;
 if (empty($asfile))
    {
    echo htmlspecialchars("$sql_insert;$crlf");
    }
 else
    {
    echo "$sql_insert;$crlf";
    }
 }

if ($what != "csv") {

	print "# phpMyAdmin MySQL-Dump$crlf";
        print "# http://phpwizard.net/phpMyAdmin/$crlf";
	print "#$crlf";
	print "# $strHost: " . $cfgServer['host'];
        if (!empty($cfgServer['port'])) {
          print ":" . $cfgServer['port'];
        }
        print " $strDatabase: $db$crlf";
	print "# --------------------------------------------------------$crlf";
	print "$crlf#$crlf";
	print "# $strTableStructure '$table'$crlf";
	print "#$crlf$crlf";

	print get_table_def($db, $table, $crlf).";$crlf";

	if ($what == "data")
   		{
		print "$crlf#$crlf";
		print "# $strDumpingData '$table'$crlf"; 
		print "#$crlf$crlf";

		get_table_content($db, $table, "my_handler");
   	}

} else {// $what != "csv"
  	get_table_csv($db, $table, $separator, "my_csvhandler");
}
if(empty($asfile)){
	print "</pre></div>\n";
	include ("footer.inc.php3");
}
?>
