<?php
if (!defined("SHARE_INC"))
 include("dbapi/share.inc");

if (!defined("SUBSCRIPTIONS_INC"))
 include("dbapi/subscriptions.inc");

page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm", "user" => "Linktrail_User"));
if (!isset($actionstring))
 die("OOps.. wrong call @ ".__FILE__." on line ".__LINE__-1);
$actionstring = base64_decode($actionstring);
parse_str($actionstring);
if ( ($expert == "") or ($trail == "") or ($action == "") )
 die("OOps.. wrong call @ ".__FILE__." on line ".__LINE__-1);
$affected = delete_suggestion($trail, $expert);
if ( ($action == "subscribe") and ($affected > 0)){
 subscribe($expert, $trail, 0);
}
Header("Location: ".$sess->url("/Experts/".rawurlencode($expert)."?ob=$ob&ci=$ci"));
page_close();
?>
