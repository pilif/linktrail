<?php
 define("MSG_FRIENDSHIPS_INC", true);


 if (!defined("MESSAGES_INC"))
  include("dbapi/messages.inc");

function request_friendship($user, $target_id, $message){
  $data_array['message'] = $message;
  send_message($user, $target_id, LTMSG_FSHIPREQ, LTMSG_URGENCY_STORE, $data_array);
}

function friendship_notification($user, $target_id, $type="term", $extra_name=""){
  $data['Type']      = $type;
  $data['ExtraName'] = $extra_name; 
  send_message($user, $target_id, LTMSG_FSHIPNOT, LTMSG_URGENCY_STORE, $data);
}


?>