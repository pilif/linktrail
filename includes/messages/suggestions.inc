<?php
 define("MSG_SUGGESTIONS_INC", true);


 if (!defined("MESSAGES_INC"))
  include("dbapi/messages.inc");
 if (!defined("SCORING_INC"))
  include("dbapi/scoring.inc");
 if (!defined("COMUTILS_INC"))
  include("dbapi/comutils.inc");

function suggest_trail($user, $target_id, $trail, $message=""){
  $data_array['message']     = $message;
  $data_array['Trail']       = $trail['id'];
  $data_array['Trail_Title'] = friendlyname($trail['path']);
  send_message($user, $target_id, LTMSG_SUGGESTN, $data_array, (RC_IN_INBOX));
  add_bonus($trail['id'], BT_SUGGEST);
}

function sent_suggestion($users, $user, $trail, $message=""){
  $data_array['message']     = $message;
  $data_array['Trail']       = $trail['id'];
  $data_array['Trail_Title'] = friendlyname($trail['path']);
  $data_array['Users']       = $users;
  send_message($user, '-1', LTMSG_SUBCAST, $data_array, (RC_IN_OUTBOX));
}

?>