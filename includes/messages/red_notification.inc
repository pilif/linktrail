<?php
 define("RED_NOTIFICATION_INC", true);


 if (!defined("MESSAGES_INC"))
  include("dbapi/messages.inc");

function send_red_notification($type, $user, $ext_node_id, $node_id){
 $data_array['Ext_Node_Id'] = $ext_node_id;
 $data_array['Node_ID']     = $node_id;
 send_message('-1', $user, $type, LTMSG_URGENCY_REPORT, $data_array);
}
?>