<?php
 define("TALK_INC", true);


 if (!defined("MESSAGES_INC"))
  include("dbapi/messages.inc");

function talk($user, $target_id, $subject, $message, $trail=-1, $quoted_reply=""){
  if ($trail != -1)
   $data_array['trail'] = $trail;

  $data_array['message'] = $message;
  $data_array['subject'] = $subject;
  $data_array['quoted_reply'] = $quoted_reply;

  send_message($user, $target_id, LTMSG_USERTALK, $data_array, 3);
}  
?>