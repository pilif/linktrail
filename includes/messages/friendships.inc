<?php
 define("MSG_FRIENDSHIPS_INC", true);


 if (!defined("MESSAGES_INC"))
  include("dbapi/messages.inc");

function request_friendship($user, $target_id, $message){
  $data_array['message'] = $message;
  send_message($user, $target_id, LTMSG_FSHIPREQ, $data_array, (RC_IN_INBOX | RC_IN_OUTBOX));
}

function friendship_notification($user, $target_id, $type="term", $extra_name=""){
  /* 
   This is a classical example for this Unix-like "historical reasons": In the first
   version of the message-center, there was no outbox (and no reference-counting).
   Since the *_self-Messages are the corresponding sent-item to *, I could reduce
   the message count to only the * and use the refcounting. 
   
   But the solution below integrates the new handling easily with the old
   calling sheme (two messages), so I use this small hack around the RC.
  */
  if(strstr($type, "_self")){
   $st = RC_IN_OUTBOX;
  }else{
   $st = RC_IN_INBOX;
  }
  $data['Type']      = $type;
  $data['ExtraName'] = $extra_name; 
  send_message($user, $target_id, LTMSG_FSHIPNOT, $data, $st);
}


?>