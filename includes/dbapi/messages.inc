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
 define("LTMSG_SUBCAST",  LTMSG_BASE+22);  

 define("RC_IN_INBOX", 1);
 define("RC_IN_OUTBOX", 2);
 
 if (!defined("MESSAGE_CLASSES_INC"))
  include("commonapi/message_classes.inc");
 
 
  
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
   $this->rc      = $dataset['ReferenceCount'];
   $this->to_name = $dataset['to_name'];
   $this->data    = unserialize($dataset['Data']);
   $this->is_inbox= ($dataset['inbox'] == "1"); //I use this for encapsulated reference-counted self destruction
   //die(sprintf("Test: %s (%d)", $dataset['inbox'], $this->is_inbox));
  }
  
  function print_simple($val){}
  
  function print_extended($val){}

  function print_error($val){ $this->print_extended($val); }
  
  function user_interaction(&$http_vars){}
  
  function get_target(){
   return $this->target;
  }

  function get_id(){
   return $this->id;
  }
  
  /*
   The following function handles the Reference-Couting. self_destruct() does only
   remove the message from the selected xxx-box. it does only delete it, when
   the message is neither in an inbox nor in an outbox
  */
  function self_destruct(){ 
   global $myDB;
//   die(sprintf("%d",$this->is_inbox));
   $new_rc = ($this->is_inbox) ? ($this->rc & ~RC_IN_INBOX) : ($this->rc & ~RC_IN_OUTBOX);
   if ($new_rc != 0){
    $this->rc = $new_rc;
    $query    = FormatSQL(SQL_UPDATE_RC, array($this->rc, $this->id)); 
   } else{
    $query = FormatSQL(DELETE_MESSAGE, array($this->id));
   }
   $myDB->query($query);
  }
  
  function set_read(){
   global $myDB;
   $query = FormatSQL(SET_DONE, array('y', $this->id));
   $myDB->query($query);
  }
  
  function update_data(){
   global $myDB;
   $query = FormatSQL(SQL_UPDATE_DATA, array(addslashes(serialize($this->data)), $this->id));
//   die("<pre>$query</pre>");
   $myDB->query($query);
  }

  function set_active($new_value){
   global $myDB;
   $str = ($new_value) ? "y" : "n";
   $query = FormatSQL(SQL_SET_ACTIVE, array($str, $this->id));
//   die("<pre>$query</pre>");
   $myDB->query($query);
  }
 }
 
 
 function &create_message_object($dbval){
  global $message_functions;
  eval('$obj = '.$message_functions[$dbval['MessageType']].'($dbval);');
  return $obj;
 }

function &read_messages_ex($userval, $inbox=true, $current_index=0, $ordertext="Date"){
 global $myDB;
 if ($inbox){
  if (!defined("SQL_MSGLIST_IN_INC"))
   include("dbapi/sql_msglist_in.inc");
 }else{
  if (!defined("SQL_MSGLIST_OUT_INC"))
   include("dbapi/sql_msglist_out.inc");
 }
  
 $object_array = Array();
 
 if ($current_index != -1)
  $query = FormatSQL(GET_MESSAGES_EX, array($userval, $ordertext, $current_index));
 else
  $query = FormatSQL(GET_MESSAGES_EX_UL, array($userval, $ordertext));
 if (!$myDB->query($query)) return $object_array; 
 if ($myDB->num_rows() == 0) return $object_array;
 while($myDB->next_record()){
  $object_array[] = create_message_object($myDB->Record); //still not more portable than in the above
 }                                                        //now obsolete function...
 $count = count_max_messages($userval, $inbox);
 
 $hsh['arr']   = &$object_array;
 $hsh['count'] = $count;
 return $hsh;
} 


function count_max_messages($userval, $inbox=true){
 global $myDB;
 
 if ($inbox){
  if (!defined("SQL_MSGLIST_IN_INC"))
   include("dbapi/sql_msglist_in.inc");
 }else{
  if (!defined("SQL_MSGLIST_OUT_INC"))
   include("dbapi/sql_msglist_out.inc");
 }
  
 $query = FormatSQL(COUNT_RESULTS_MSG, array($userval));
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

 
function send_message($sender, $user, $type, $data_array, $rc){
  global $myDB;
  $dataser = serialize($data_array); 
  $query   = FormatSQL(SEND_MESSAGE, array($user, $type, $sender, addslashes($dataser), $rc));
  //echo("<pre>$query</pre><p>");
  $myDB->query($query);
}

function &read_one_message($id, $in=true){
 global $myDB;
 
 $res = -1;
 $intext = ($in) ? "1" : "0";
 $query = FormatSQL(GET_ONE_MESSAGE, array($intext, $id));
 if (!$myDB->query($query)) return $res;
 if ($myDB->num_rows() == 0) return $res;
 $myDB->next_record();
 return create_message_object($myDB->Record);
}

function count_new_messages($user_id, $timestamp){
 global $myDB;
 $res = 0;
 $query = FormatSQL(NEW_MSG_COUNT, array($user_id));
// die($query);
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
// die("<pre>$query</pre>");
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