<?php
/* $Id: polls_classes.inc,v 1.1 2000-11-17 22:07:42 pilif Exp $ */
 define("POLLS_CLASSES_INC", true);

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

 if (!defined("POLLS_INC"))
  include("dbapi/polls.inc");

 if (!defined("EDIT_DIRECTORY_INC"))
  include("dbapi/edit_directory.inc");

class ltrPollAdd extends ltrPoll{
  
  function execute_action($result_id){
   if ($result_id == VOTE_NO){
    $this->self_destruct();
   }elseif($result_if == VOTE_YES){
    $obj['name']         = $this->data['name'];
    $obj['level']        = level_count($this->data['name']);
    $obj['description']  = $this->data['description'];
    $obj['useraccess']   = 0;
    $obj['friendaccess'] = 0;
    $obj['owner']        = '';
    $obj['language']     = $this->data['language'];
    add_object($obj, true);
    $this->self_destruct();
   }
  }
  
  function print_results(){
   //gui not implemented yet!
  }
  
  function print_form(){
   //gui not implemented yet!
  }
}

class ltrPollEdit extends ltrPoll{

  function execute_action($result_id){
   if ($result_id == VOTE_NO){
    $this->self_destruct();
   }elseif($result_if == VOTE_YES){
    $obj['name']         = ($this->data['new_name'] != "") ? $this->data['new_name'] : $this->data['old_name'];
    $obj['level']        = level_count($obj['name']);
    $obj['description']  = $this->data['description'];
    $obj['useraccess']   = 0;
    $obj['friendaccess'] = 0;
    $obj['owner']        = '';
    $obj['language']     = $this->data['language'];
    if ($obj['name'] != $this->data['old_name'])
     sub_rename($this->data['old_name'], $obj['name']);
    edit_object($this->data['id'], $obj);
   }
  }
  
  function print_results(){
   //gui not implemented yet!!
  }
  
  function print_form(){
   //no gui yet!
  }
}
?>