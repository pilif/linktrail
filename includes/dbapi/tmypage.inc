<?php
 define("TMYPAGE_INC", true);

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

 if (!defined("SQL_MYPAGE_INC"))
  include("dbapi/sql_mypage.inc");

  

function read_trails_mypage(&$userdata, $current_index, $order_by, $order_type="ASC"){

 $trails = Array();
 
 srand ((double)microtime()*1000000);
 $randval = rand(1111111,9999999);
 $tablename  = 'tt'.uniqid($randval);

 if (! create_temp_table_mp($tablename, $userdata['User_ID']) ) return $trails;   
 if (! put_subscribed_trails($tablename, $userdata['Username']) ) return $trails;

 $count = count_results_mp($tablename);
 if ($current_index < $count)
  $current_index = 0;
 $trails = read_table_mp($tablename, $order_by, $order_type, $current_index);

 delete_table_mp($tablename);                  //7
 $hsh['trails'] = $trails;
 $hsh['count']  = $count;
 return $hsh;
}  


function create_temp_table_mp($tablename, $user_id){
 global $myDB;
 $query = FormatSQL(CREATE_AND_FILL_MYPAGE, array($tablename, $user_id));
 if (! $myDB->query($query) ) return false;
 return true;
}

function put_subscribed_trails($tablename, $username){
 global $myDB;
 $query = FormatSQL(INSERT_SUBSCRIBED, array($tablename, $username));
 if (! $myDB->query($query) ) return false;
 return true;
}

function read_table_mp($tablename, $order_by, $order_type, $limit){
 global $myDB;
 $arr = Array();
 if ($limit == -1)
  $query = FormatSQL(GET_TRAILS_MYPAGEU, array($tablename, $order_by, $order_type));
 else
  $query = FormatSQL(GET_TRAILS_MYPAGE, array($tablename, $order_by, $order_type, $limit));

 if (! $myDB->query($query) ) return false;
 while ($myDB->next_record()){
  $hsh['path']                = $myDB->f("Name");
  $hsh['id']                  = $myDB->f("Node_ID");
  $hsh['changedate']          = $myDB->f("ChangeDate");
  $hsh['adddate']             = $myDB->f("AddDate");
  $hsh['parent']              = $myDB->f("parent");
  $hsh['subscribed']          = $myDB->f("SubscribedToTrail") != 0;
  $hsh['language']            = $myDB->f("Language");
  $hsh['parent_id']           = $myDB->f("parent_id");
  $arr[] = $hsh;
 }
 reset($arr);
 return $arr;  
}

function count_results_mp($tablename){
 global $myDB;
 if (! $myDB->query(FormatSQL(COUNT_RESULTS_MYPAGE, array($tablename)))) return 0;
 $myDB->next_record();
 return $myDB->f("cnt");
}

function delete_table_mp($tablename){
 global $myDB;
 $myDB->query(FormatSQL(DROP_TEMP_MYPAGE, array($tablename)));
}

