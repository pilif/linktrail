<?php
/*
 Message_clases contains the class hirearchy for the different types of 
 Messages linktrail sends to an user.
 
 Also definied here is the assoziazion between a message-object and the
 message-id from the database.
 
 The parent-object of all those classes is defined in dbapi/messages.inc
*/
 define("MESSAGE_CLASSES_INC", true);

 if(!class_exists("Template"))
  include("template.inc");
 
global $message_functions;
 
$message_functions[LTMSG_TRAILDEL] = "admin_modification";
$message_functions[LTMSG_TRAILMOV] = "admin_modification";
$message_functions[LTMSG_TRAILCHG] = "admin_modification";
$message_functions[LTMSG_LINKADD]  = "user_modification";
$message_functions[LTMSG_LINKMOVD] = "user_modification";
$message_functions[LTMSG_LINKCHGD] = "user_modification";
$message_functions[LTMSG_LINKDELE] = "user_modification";
$message_functions[LTMSG_SUGGESTN] = "trail_suggestion";
$message_functions[LTMSG_USERTALK] = "lms_message";
$message_functions[LTMSG_USERTRNS] = "lms_message";
$message_functions[LTMSG_FSHIPREQ] = "friendship_request";
$message_functions[LTMSG_FSHIPTRM] = "std_message";
$message_functions[LTMSG_LTRANNCE] = "std_message";
$message_functions[LTMSG_POLLSTRT] = "poll_started";
$message_functions[LTMSG_POLLFINI] = "std_message";
$message_functions[LTMSG_TRAILSLT] = "std_message";
$message_functions[LTMSG_SLOTTERM] = "std_message";
$message_functions[LTMSG_FEELECT]  = "std_message";
$message_functions[LTMSG_FETERMN]  = "std_message";
$message_functions[LTMSG_REPORTSN] = "std_message";
 

class ltrStdMessageMsg extends ltrMessage{
  function ltrStdMessageMsg($dataset){
   $this->ltrMessage($dataset); //call parent constructor
  }
}

class ltrAdminModificationMsg extends ltrMessage{
  function ltrAdminModificationMsg($dataset){
   $this->ltrMessage($dataset);
  }
}

class ltrUserModificationMsg extends ltrMessage{
  var $child_messages = Array(); //contains the childs
  function ltrUserModificationMsg($dataset){
   $this->ltrMessage($dataset);
   //no further processing needed. dataset->data is unserialized in the 
   //constructor above
  }
  
  function add_child($dataset){
   //we have to do some dirty work...
   $usda = unserialize($dataset['Data']);
   $hsh['trail_id']    = $usda['Trail'];
   $hsh['link_title']  = $usda['Link_Title'];
   $hsh['link_url']    = $usda['Link_Url'];
   $hsh['sender']      = $dataset['Sender'];
   $hsh['message_typ'] = $dataset['MessageType'];
   $hsh['message_id']  = $dataset['Message_ID'];
   $hsh['date']        = $dataset['Date'];
   $this->child_messages[]['trail_id'] = $hsh;
  }
  
 function print_simple(){
  $tpl = new Template(APPLICATION_HOME."/templates/messages/short", "keep");
  $tpl->set_file(array("simple" => "user_cn.html"));
  
  $trailpath = id2path($this->data['Trail']);
  $tpl->set_var("TRAILURL", build_good_url($trailpath));
  $tpl->set_var("TRAILNAME", friendlyname($trailpath));
  
  
  $tpl->parse("main", "simple");
  return $tpl->get("main");
 }
}

class ltrSuggestionMsg extends ltrMessage{
  function ltrSuggestionMsg($dataset){
   $this->ltrMessage($dataset);
  }
}

class ltrLMSMessageMsg extends ltrMessage{
  function ltrLMSMessageMsg($dataset){
   $this->ltrMessage($dataset);
  }
}

class ltrFriendshipRequestMsg extends ltrMessage{
  function ltrFriendshipRequestMsg($dataset){
   $this->ltrMessage($dataset);
  }
}

class ltrPollStartedMsg extends ltrMessage{
  function ltrPollStartedMsg($dataset){
   $this->ltrMessage($dataset);
  }
}


function &std_message($dataset){
 return new ltrStdMessageMsg($dataset);
}

function &admin_modification($dataset){
 return new ltrAdminModificationMsg($dataset);
}

function &user_modification($dataset){
 global $have_notifications, //this is dirty style, but I don't see an other way to do this
        $var;                //in PHP. And we are not multi-threaded, so it is not really a 
                             //problem but just extremely dirty.
                             
 $deserialized_array = unserialize($dataset['Data']);
 
 if (is_object($have_notifications[$deserialized_array['Trail']])){
  $have_notifications[$deserialized_array['Trail']]->add_child($dataset);
  $var = "";
  return $var; //we must return a reference. Here is one...
 }else{
  $have_notifications[$deserialized_array['Trail']] = new ltrUserModificationMsg($dataset);
  return $have_notifications[$deserialized_array['Trail']];
 }
  
}

function &trail_suggestion($dataset){
 return new ltrSuggestionMsg($dataset);
}

function &lms_message($dataset){
 return new ltrLMSMessageMsg($dataset);
}

function &friendship_request($dataset){
 return new ltrFriendshipRequestMsg($dataset);
}

function &poll_started($dataset){
 return new ltrPollStartedMsg($dataset);
}
?>
