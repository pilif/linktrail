<?php
define("COMMON_SUBSCRIPTIONS_INC", true);

if (!defined("SUBSCRIPTIONS_INC"))
  include("dbapi/subscriptions.inc");

define("NT_EMAIL", 1);
define("NT_ICQ", 2);
define("NT_AIM", 4);
define("NT_YAHOO", 8);
define("NT_MSN", 16);

define("SFT_SUBSCRIBE", 1);
define("SFT_CHSUBS", 2);
define("SFT_CHNOT", 3);



function has_notification($notifications, $notification){
 return (($notifications & $notification) != 0);
}

function add_notification($notifications, $cap){
 $newcaps = $notifications & $notification;
 return $newcaps;
}

function del_notification($notifications, $cap){
 $newcaps = $notifications ^ $notification;
 return $newcaps;
}
?>