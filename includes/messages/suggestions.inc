<?php
 define("MSG_SUGGESTIONS_INC", true);


 if (!defined("MESSAGES_INC"))
  include("dbapi/messages.inc");
 if (!defined("SCORING_INC"))
  include("dbapi/scoring.inc");
 if (!defined("COMUTILS_INC"))
  include("dbapi/comutils.inc");

function suggest_trail($user, $target_id, $trail, $message=""){
  $data_array['message'] = $message;
  $data_array['Trail']   = $trail;
  send_message($user, $target_id, LTMSG_SUGGESTN, LTMSG_URGENCY_STORE, $data_array);
  add_bonus($trail, BT_SUGGEST);
}

function sub_action($user, $target_id, $traildata, $subtype){
 $data_array['Type']  = $subtype;
 $data_array['Title'] = friendlyname($traildata['path']);
 $data_array['Trail'] = $traildata['id'];
 send_message($user, $target_id, LTMSG_SUBNOTI, LTMSG_URGENCY_STORE, $data_array); 
}

?>