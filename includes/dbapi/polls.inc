<?php
/* $Id: polls.inc,v 1.1 2000-11-13 21:57:25 pilif Exp $ */
 define("POLLS_INC", true);

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

 define("LTPOLL_BASE", 100);
 define("LTPOLL_ADDNODE",  LTPOLL_BASE+1);
 define("LTPOLL_EDITNODE", LTPOLL_BASE+2);

 //This is a class handling one poll
 class ltrPoll{
  var $id;
  var $votes = Array(); //assoziative Array
  var $type;
  var $dat;
  var $data;
  var $node_id;
  
  //Contructor
  function ltrPoll(&$recordset){
   $this->dat     = $recordset['AddDate'];
   $this->type    = $recordset['PollType'];
   $this->data    = unserialize($recordset['ExtraData']);
   $this->node_id = $recordset['Node_ID'];
   $this->id      = $recordset['Poll_ID'];
   $this->owner   = $recordset['Owner'];
   //Recordset contains -1: We will add ourselves to the 
   //database...
   if ($this->id == "-1")
    $this->add_poll();
  }
  
  function add_poll(){
   global $myDB;
   
   $query = FormatSQL(CREATE_POLL, array($this->node_id, $this->type, addslashes(serialize($this->data)), $this->owner));
   $myDB->query($query);
   $this->id = mysql_insert_id(); //deadly unportable but very nice!
   //many methods rely on $this->id being a valid Poll_ID
  }
  
  function has_voted($uid){
   global $myDB;
   
   $query = FormatSQL(HAS_VOTED, array($uid, $this->id));
   if (!$myDB->query($query)) return false;
   return ($myDB->num_rows() != 0);
  }
  
  function poll($result_id, $uid){
   global $myDB;
   
   if (has_voted($uid)){
    $query = FormatSQL(UPDATE_VOTE, array($result_id, $this->id, $uid));
   }else{
    $query = FormatSQL(PLACE_VOTE, array($uid, $this->id, $result_id));
   }
   $myDB->query($query);
  }
  
 }

}
?>