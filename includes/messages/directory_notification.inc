<?php
 define("DIRECTORY_NOTIFICATION_INC", true);


 if (!defined("MESSAGES_INC"))
  include("dbapi/messages.inc");
 if (!defined("COMUTILS_INC"))
  include("dbapi/comutils.inc");
 if (!defined("SUBSCRIPTIONS_INC"))
  include("dbapi/subscriptions.inc");

function send_notification($traildata, $sender, $type, $ownersend=false){
 global $myDB;
 $data_array['Trail'] = $traildata['id'];
 $data_array['Title'] = friendlyname($traildata['path']);
 if ($ownersend) //sometimes a message to the owner does not make sense... (when he deletes his own trail for exemple)
  send_message($sender, $traildata['userid'], $type, $data_array); //TODO: After the DB changed: Read the correct urgency
 $subs = get_subscribers($traildata['id']);
 for($x=0; $x < count($subs); $x++){
  send_message($sender, $subs[$x]['userid'], $type, $data_array); //TODO: After the DB changed: Read the correct urgency
 }
}

//send_link_notification($mytrail, $hsh, $auth->auth['uname'], LTMSG_LINKDELE, !($auth->auth['uid'] == $mytrail['owner']));
function send_link_notification($traildata, $linkdata, $sender, $type, $ownersend=false){
 global $myDB;
 $data_array['Trail']      = $traildata['id'];
 $data_array['Link_Url']   = $linkdata['url'];
 $data_array['Link_Title'] = $linkdata['title'];
 $data_array['Title'] = friendlyname($traildata['path']);
 
 if ($ownersend) //sometimes a message to the owner does not make sense... (when he deletes his own trail for exemple)
  send_message($sender, $traildata['userid'], $type, $data_array, $traildata['id']); //TODO: After the DB changed: Read the correct urgency
 $subs = get_subscribers($traildata['id']);
 for($x=0; $x < count($subs); $x++){
  send_message($sender, $subs[$x]['userid'], $type, $data_array, $traildata['id']); //TODO: After the DB changed: Read the correct urgency
 }
}

function send_link_admin_move_notification($traildata, $newname, $reason=0){
 global $myDB;
 
 $data_array['Trail']   = $traildata['id'];
 $data_array['Newname'] = $newname;
 $data_array['Reason']  = $reason; //TODO: Find good reasons...
 $data_array['Title'] = friendlyname($traildata['path']);
 
 send_message('-1', $traildata['userid'], LTMSG_TRAILMOV, $data_array);
 $subs = get_subscribers($traildata['id']);
 for($x=0; $x < count($subs); $x++){
  send_message('-1', $subs[$x]['userid'], LTMSG_TRAILMOV, $data_array);
 }
}

function send_move_notification($sender, $traildata, $newname){
 global $myDB;
 $data_array['Trail']   = $traildata['id'];
 $data_array['Newname'] = $newname;
 $data_array['Title'] = friendlyname($traildata['path']);
  
 $subs = get_subscribers($traildata['id']);

 for($x=0; $x < count($subs); $x++){
  send_message($sender, $subs[$x]['userid'], LTMSG_TRAILMOV, $data_array);
 }
}

//TODO: Define reasons for administration work
function send_admin_notification($traildata, $type, $reason=0){
/* foreach($traildata as $key=>$value)
  echo("$key = $value<br>");
 die("fertig"); */
 $data_array['Trail']  = $traildata['id'];
 $data_array['Reason'] = $reason;
 $data_array['Title']  = friendlyname($traildata['path']);
 
 send_message('-1', $traildata['userid'], $type, $data_array); //TODO: After the DB changed: Read the correct urgency
}
?>