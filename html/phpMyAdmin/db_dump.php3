<?php
/* $Id: db_dump.php3,v 1.1.1.1 2000-11-06 20:55:48 linktrai Exp $ */
@set_time_limit(600);
$crlf="\n";
if (empty($asfile)) {
        include("header.inc.php3");
        print "<div align=left><pre>\n";

} else {
        include("lib.inc.php3");
        header("Content-disposition: filename=$db.sql");
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

$tables = mysql_list_tables($db);

$num_tables = @mysql_numrows($tables);
if ($num_tables == 0)
   {
   echo $strNoTablesFound;
   }
else
   {
   $i = 0;
   print "# phpMyAdmin MySQL-Dump$crlf";
   print "# http://phpwizard.net/phpMyAdmin/$crlf";
   print "#$crlf";
   print "# $strHost: " . $cfgServer['host'];
   if (!empty($cfgServer['port'])) {
      print ":" . $cfgServer['port'];
   }
   print " $strDatabase: $db$crlf";

   while ($i < $num_tables)
         { 
         $table = mysql_tablename($tables, $i);

print $crlf;
print "# --------------------------------------------------------$crlf";
print "#$crlf";
print "# $strTableStructure '$table'$crlf";
print "#$crlf";
print $crlf;

echo get_table_def($db, $table, $crlf).";$crlf$crlf";

if ($what == "data")
   {

   print "#$crlf";
   print "# $strDumpingData '$table'$crlf";
   print "#$crlf";
   print $crlf;

   get_table_content($db, $table, "my_handler");
   }
       $i++;
       }
   }
if(empty($asfile)){
	print "</pre></div>\n";
	include ("footer.inc.php3");
}
?>
