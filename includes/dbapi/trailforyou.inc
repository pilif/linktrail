<?php

 define("TRAILFORYOU_INC", true);

 /*
   The following is common to all DBAPI-Units. The DB-Connection 
   will only be opened once
 */

 global $myDB;

 
 if (!is_object($myDB)) {
  $myDB = new DB_Linktrail;
  include("dbapi/sql_strs.inc");
  include("dbapi/sql_util.inc");
 }
 
 if (!defined("COMUTILS_INC"))
  include("dbapi/comutils.inc");

 if (!defined("SQL_TRAILFORYOU_INC"))
  include("dbapi/sql_trailforyou.inc");

  

function &read_a_trail_for_you(&$nodes, $username, $userid){
 global $myDB;
 
 $trails = Array();
 $in_str = "";
 if (!is_array($nodes)) return $trails;
 $in_str = $nodes[0]['parent_id'];
 for($x=1; $x < count($nodes); $x++){
  $in_str .= ', '.$nodes[$x]['parent_id'];
 }
 $query = FormatSQL(SQL_QUERYTRAILS, array($username, $in_str, $userid));
 //die("Test: <pre>$query</pre>");
 if (!$myDB->query($query)) return $trails;
 if ($myDB->num_rows() == 0) return $trails;
 while($myDB->next_record()){
  $hsh['path']        = $myDB->f("Name");
  $hsh['description'] = $myDB->f("Description");
  $trails[] = $hsh;
 }
 srand((double) microtime() * 1000000);
 $rand = rand(0, count($trails)-1);
 return $trails[$rand];
}
