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
  send_message($user, $target_id, LTMSG_SUGGESTN, $data_array, (RC_IN_INBOX | RC_IN_OUTBOX));
  add_bonus($trail, BT_SUGGEST);
}


?>