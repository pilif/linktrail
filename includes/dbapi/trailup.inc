<?php
/**
* API for reading trails from the directory
*
* This module is used for reading Trails from a directory-Node.
* I recreated this new module because of the complexity of the
* task to be done.
*
* @module       trailup
* @modulegroup  dbapi
* 
*/
 define("TRAILUP_INC", true);

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

 if (!defined("SQL_TRAILUP_INC"))
  include("dbapi/sql_trailup.inc");

  

function read_trails(&$nodeinfo, $current_index){
 /* Tasks to be done:
  1) Count the trails in our node (TRAILCOUNT)
  2) Create a unique Tablename
  3) Get our trails (CREATE_AND_FILL); a temporary table
     will be created by this task
  4) Get Climbing Trails (INSERT_CLIMBING);
  5) Read the temporary table, sorted by points and Change-Date
  6) Create Array of Nodeinfo
  7) Drop temp-table
*/
 $level    = level_count($nodeinfo['path']);
 $nodename = addslashes($nodeinfo['path']);
 $node_id  = $nodeinfo['id'];
 $nodes = Array();
 srand ((double)microtime()*1000000);
 $randval = rand(1111111,9999999);
 $trailcount = count_trails($node_id);      //1

 if ($trailcount < MIN_TRAILS)
  $maxcount = MIN_TRAILS-$trailcount;
 else
  $maxcount   = Ceil( ($trailcount / 100) * TRAIL_UP_COUNT);

// die($maxcount);
 if ($maxcount == 0) $maxcount = 1;

 if ($trailcount === false) return $nodes;
 
 $tablename  = 'tt'.uniqid($randval);       //2

 if (! create_temp_table($tablename, $node_id) ) return $nodes;   //3
 if($node_id != 1 and ($node_id != 761)){
  if (! put_climbing_trails($tablename, $level, $maxcount, $nodename) ) return $nodes; //4
 }

 $count = count_results($tablename);
 $nodes = read_table($tablename, $node_id, $current_index);           //5+6

 delete_table($tablename);                  //7
 $hsh['nodes'] = $nodes;
 $hsh['count'] = $count;
 return $hsh;
}  

function count_trails($node_id){
 global $myDB;
 
 if (! $myDB->query(FormatSQL(TRAILCOUNT, array($node_id))) ) return false;
 $myDB->next_record();

 return $myDB->f("nr");
}

function create_temp_table($tablename, $node_id){
 global $myDB;
 $query = FormatSQL(CREATE_AND_FILL, array($tablename, $node_id));
 if (! $myDB->query($query) ) return false;
 return true;
}

function put_climbing_trails($tablename, $level, $max_count, $nodename){
 global $myDB;
 $query = FormatSQL(INSERT_CLIMBING, array($tablename, $level, $nodename, $level+TRAIL_LEVEL_UP_COUNT+1, $level, $max_count));
 if (! $myDB->query($query) ) return false;
 return true;
}

function read_table($tablename, $node_id, $limit){
 global $myDB;
 $arr = Array();
 if ($limit == -1)
  $query = FormatSQL(GET_TRAILSU, array($tablename));
 else
  $query = FormatSQL(GET_TRAILS, array($tablename, $limit));

 if (! $myDB->query($query) ) return false;
 while ($myDB->next_record()){
  $hsh['path']              = $myDB->f("Name");
  $hsh['id']                = $myDB->f("Node_ID");
  $hsh['description']       = $myDB->f("Description");
  $hsh['userid']            = $myDB->f("Owner");
  $hsh['username']          = $myDB->f("username");
  $hsh['useraccess']        = $myDB->f("UserAccess");
  $hsh['friendaccess']      = $myDB->f("FriendAccess");
  $hsh['changedate']        = $myDB->f("ChangeDate");
  $hsh['adddate']           = $myDB->f("AddDate");
  $hsh['hasimage']          = $myDB->f("HasImage");
  $hsh['from']              = $myDB->f("Parent");
  $hsh['sponsored']         = $myDB->f("SponsoredBy");
  $hsh['ownernotification'] = $myDB->f("OwnerNotification");
  $hsh['points']            = $myDB->f("new_points");
  $arr[] = $hsh;
 }
 reset($arr);
 return $arr;  
}

function count_results($tablename){
 global $myDB;
 if (! $myDB->query(FormatSQL(COUNT_RESULTS, array($tablename)))) return 0;
 $myDB->next_record();
 return $myDB->f("cnt");
}

function delete_table($tablename){
 global $myDB;
 $myDB->query(FormatSQL(DROP_TEMP, array($tablename)));
}

