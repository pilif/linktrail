<?php
define("SUBSCRIPTIONS_INC", true);

global $myDB;

 if (!is_object($myDB)) {
  $myDB = new DB_Linktrail;
  include("dbapi/sql_strs.inc");
  include("dbapi/sql_util.inc");
 }

 if (!defined("COMUTILS_INC"))
  include("dbapi/comutils.inc");
 if (!defined("SCORING_INC"))
  include("dbapi/scoring.inc");


function subscribe($username, $trail_id, $notification_method){
 global $myDB;
 
 $query = FormatSQL(SQL_SUBSCRIBE, array($username, $trail_id, $notification_method));
 if ($myDB->query($query)){
  add_bonus($trail_id, BT_SUBSCRIBE);
 }
 return true; 
}

function unsubscribe($username, $trail_id){
 global $myDB;
 
 $query = FormatSQL(SQL_UNSUBSCRIBE, array($username, $trail_id));
 if ($myDB->query($query))
  add_bonus($trail_id, BT_UNSUBSCRIBE);
 return true;
}

function change_subscription($user_id, $trail_id, $notification_method){
 global $myDB;
 
 $query = FormatSQL(SQL_CHSUBSCRIPTION, array($notification_method, $user_id, $trail_id));
 return $myDB->query($query);
}

function change_owner_notification($trail_id, $notification_method){
 global $myDB;
 
 $query = FormatSQL(SQL_CHNOTIFICATION, array($notification_method, $trail_id));
// die("<pre>$query</pre>");
 return $myDB->query($query);
}

function get_notification_method($username, $trail_id){
 global $myDB;
 $query = FormatSQL(SQL_IS_SUBSCRIBED, array($username, $trail_id));
// die($query);
 $myDB->query($query);
 if ($myDB->num_rows() == 0)
  return -1;
 $myDB->next_record();
 return $myDB->f("NotificationMethod");
}

function get_subscribers($trail_id){
 global $myDB;
 $arr = Array();
 
 $query = FormatSQL(SQL_GET_SUBSCRIBERS, array($trail_id));
 if (!$myDB->query($query)) return $arr;
 if ($myDB->num_rows() == 0) return $arr;
 while ($myDB->next_record()){
  $hsh['userid']  = $myDB->f("user_id");
  $hsh['urgency'] = $myDB->f("NotificationMethod"); //senseless field name... historical reasons...
  $arr[] = $hsh;
 }
 return $arr;
}

?>