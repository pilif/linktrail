<?php
 define("BOUNCE_INC", true);


 if (!defined("MESSAGES_INC"))
  include("dbapi/messages.inc");

function warn_bounce($target_id, $bounce_type){
  $data_array['bounce_type'] = $bounce_type;
  send_message('-1', $target_id, LTMSG_BOUNCEN, $data_array, (RC_IN_INBOX));
}  
?>