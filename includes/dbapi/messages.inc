<?php
 define("MESSAGES_INC", true);

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

 if (!defined("SQL_MESSAGES_INC"))
  include("dbapi/sql_messages.inc");

 define("LTMSG_BASE", 1000);
 define("LTMSG_TRAILDEL", LTMSG_BASE+1);
 define("LTMSG_TRAILMOV", LTMSG_BASE+2);
 define("LTMSG_TRAILCHG", LTMSG_BASE+3);
 define("LTMSG_LINKADD",  LTMSG_BASE+4);
 define("LTMSG_LINKMOVD", LTMSG_BASE+5);
 define("LTMSG_LINKCHGD", LTMSG_BASE+6);
 define("LTMSG_LINKDELE", LTMSG_BASE+7);
 define("LTMSG_SUGGESTN", LTMSG_BASE+8);
 define("LTMSG_USERTALK", LTMSG_BASE+9);
 define("LTMSG_USERTRNS", LTMSG_BASE+10);
 define("LTMSG_FSHIPREQ", LTMSG_BASE+11);
 define("LTMSG_FSHIPNOT", LTMSG_BASE+12);
 define("LTMSG_LTRANNCE", LTMSG_BASE+13);
 define("LTMSG_POLLSTRT", LTMSG_BASE+14);
 define("LTMSG_POLLFINI", LTMSG_BASE+15);
 define("LTMSG_TRAILSLT", LTMSG_BASE+16);
 define("LTMSG_SLOTTERM", LTMSG_BASE+17);
 define("LTMSG_FEELECT",  LTMSG_BASE+18);
 define("LTMSG_FETERMN",  LTMSG_BASE+19);
 define("LTMSG_REPORTSN", LTMSG_BASE+20);  
 define("LTMSG_SUBNOTI",  LTMSG_BASE+21);  

 if (!defined("MESSAGE_CLASSES_INC"))
  include("commonapi/message_classes.inc");
 
 
 define("LTMSG_URGENCY_URGENT", "u");
 define("LTMSG_URGENCY_REPORT", "r");
 define("LTMSG_URGENCY_STORE",  "s");
 
  
//some kind of an "abstract" base-class....
 class ltrMessage{
  var $dat;
  var $id;
  var $sender;
  var $data;
  var $type;
  var $target;
  var $childm; 

  
  function ltrMessage($dataset){
   $this->dat     = $dataset['Date'];
   $this->id      = $dataset['Message_ID'];
   $this->sender  = $dataset['Sender'];
   $this->type    = $dataset['MessageType'];
   $this->target  = $dataset['Target'];
   $this->is_read = $dataset['Done'] == 'y';
   $this->data = unserialize($dataset['Data']);
  }
  
  function print_simple($val){}
  
  function print_extended($val){}

  function print_error($val){ $this->print_extended($val); }
  
  function user_interaction(&$http_vars){}
  
  function send_urgent(){}

  function get_target(){
   return $this->target;
  }

  function get_id(){
   return $this->id;
  }
  
  function self_destruct(){ //this will kill this message from the database
   global $myDB;
   $query = FormatSQL(DELETE_MESSAGE, array($this->id));
   $myDB->query($query);
  }
  
  function set_read(){
   global $myDB;
   $query = FormatSQL(SET_DONE, array('y', $this->id));
   $myDB->query($query);
  }
 }
 
 
 function &create_message_object($dbval){
  global $message_functions;
  eval('$obj = '.$message_functions[$dbval['MessageType']].'($dbval);');
  return $obj;
 }

 function read_messages($target_user, $current_index=0){
  global $myDB;
  
  $object_array = Array();
  
  $query = FormatSQL(GET_MESSAGES, array($target_user, $current_index));
//  die("Test: $query");
  if (!$myDB->query($query)) return $object_array;
  if ($myDB->num_rows() == 0) return $object_array;
  while($myDB->next_record()){ 
   $obj = create_message_object($myDB->Record); //this is not portable but works with mysql
   /*
    $obj is 0 if we read a child-message of a user-content-notification
    from the database. We will not have more than one CN-message visible
    per trail...
   */
   if (is_object($obj))
    $object_array[] = $obj;
  } 
  return $object_array;
 }

function &read_messages_ex($target_user, $current_index=0, $ordertext="Date"){
 global $myDB;
 
 if (!defined("SQL_MSGLIST_INC"))
  include("dbapi/sql_msglist.inc");
  
 /* Tasks to be done:
  1) Create a unique Tablename
  2) Get our ungrouped messages (CREATE_AND_FILL_MSG); a temporary table
     will be created by this task
  3) Get grouped messages (INSERT_GROUPED_MSG);
  4) Read the temporary table, sorted by date
  5) Create Array of Objects
  6) Drop temp-table
  
*/
 
 $object_array = Array();
 
 srand ((double)microtime()*1000000);
 $randval = rand(1111111,9999999);
 $tablename  = 'tt'.uniqid($randval);  //1
 
 if (!create_temp_table_msg($tablename, $target_user)) return $object_array; //2
 if (!get_grouped_messages($tablename, $target_user)) return $object_array; //3
 
 if ($current_index != -1)
  $query = FormatSQL(GET_MESSAGES_EX, array($tablename, $ordertext, $current_index));
 else
  $query = FormatSQL(GET_MESSAGES_EX_UL, array($tablename, $ordertext));
  
 if (!$myDB->query($query)) return $object_array; 
 if ($myDB->num_rows() == 0) return $object_array;
 while($myDB->next_record()){
  $object_array[] = create_message_object($myDB->Record); //still not more portable than in the above
 }                                                        //now obsolete function...
 $count = count_max_messages($tablename);
 drop_temp($tablename);
 
 $hsh['arr']   = &$object_array;
 $hsh['count'] = $count;
 return $hsh;
} 

function create_temp_table_msg($tablename, $target_user){
 global $myDB;

 if (!defined("SQL_MSGLIST_INC"))
  include("dbapi/sql_msglist.inc"); 

 $query = FormatSQL(CREATE_AND_FILL_MSG, array($tablename, $target_user));
 return $myDB->query($query);
}

function get_grouped_messages($tablename, $target_user){
 global $myDB;

 if (!defined("SQL_MSGLIST_INC"))
  include("dbapi/sql_msglist.inc"); 

 $query = FormatSQL(INSERT_GROUPED_MSG, array($tablename, $target_user));
 return $myDB->query($query);
}

function count_max_messages($tablename){
 global $myDB;
 
 if (!defined("SQL_MSGLIST_INC"))
  include("dbapi/sql_msglist.inc"); 
 
 $query = FormatSQL(COUNT_RESULTS_MSG, array($tablename));

 if (!$myDB->query($query)) return 0;
 $myDB->next_record();
 return $myDB->f("cnt"); 
}

function drop_temp($tablename){
 global $myDB;
 
 if (!defined("SQL_MSGLIST_INC"))
  include("dbapi/sql_msglist.inc"); 
 
 $query = FormatSQL(DROP_TEMP_MSG, array($tablename));
 return $myDB->query($query);
}

 
function send_message($sender, $user, $type, $urgency, $data_array, $trail = "NULL"){
  global $myDB;
  $dataser = serialize($data_array); 
  $query   = FormatSQL(SEND_MESSAGE, array($user, $type, $sender, $urgency, addslashes($dataser), $trail));
  //die("<pre>$query</pre>");
  $myDB->query($query);
  //We have an urgent Message. Now we will create a ltrMessage-Object
  //and utilize it to send the Message.
  if ($urgency == LTMSG_URGENCY_URGENT){
   $record['Message_ID']  = mysql_insert_id();
   $record['Target']      = $user;
   $record['MessageType'] = $type;
   $record['Sender']      = $sender;
   $record['Date']        = sql_date(unix2friendly(time()));
   $record['Urgency']     = $urgency;
   $record['Data']        = $dataser;
   $obj = create_message_object($record);
   $obj->send_urgent();
  }
}

function &read_one_message($id){
 global $myDB;
 
 $res = -1;
 $query = FormatSQL(GET_ONE_MESSAGE, array($id));
 if (!$myDB->query($query)) return $res;
 if ($myDB->num_rows() == 0) return $res;
 $myDB->next_record();
 return create_message_object($myDB->Record);
}

function count_new_messages($user_id, $timestamp){
 global $myDB;
 $res = 0;
 $query = FormatSQL(NEW_MSG_COUNT, array($timestamp, $user_id));
 if (!$myDB->query($query)) return $res;
 if ($myDB->num_rows() == 0) return $res;
 $myDB->next_record();
 return $myDB->f("cnt"); 
}

function update_read_stamp($user_id){
 global $myDB, $glob_userdata;
 $glob_userdata['LastReadStamp'] = strftime("%Y-%m-%d %H:%M:%S", time());
 $query = FormatSQL(SQL_UPDATE_READ, array($glob_userdata['LastReadStamp'], $user_id));
 $myDB->query($query);
}

function delete_message($id){
 global $myDB;
 
 $query = FormatSQL(DELETE_MESSAGE, array($id));
 $myDB->query($query);
}

function delete_friendship_request($my, $he){
 global $myDB;
 
 /*
  I am killing here all Friendship Requests pending between two users. The application-
  logic looks that no two requests are pending between the same two poeple.
 */
 $query = FormatSQL(SQL_READ_REQUESTS, array($my, $he, $he, $my));
 $myDB->query($query);
 $str = "";
 $x   = 0;
 while ($myDB->next_record()){
  $x++;
  $str .= $myDB->f("Message_ID");
  $str .= ($x == $myDB->num_rows()) ? "" : ', ';
 }
 $query = FormatSQL(SQL_DEL_REQUESTS, array($str));
 $myDB->query($query);
}
?>