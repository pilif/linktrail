<?php
define("PERMISSIONS_INC", true);

define("FT_ALL", 1);
define("FT_WAITING", 2);
define("FT_AWAITED", 3);


global $myDB;

 if (!is_object($myDB)) {
  $myDB = new DB_Linktrail;
  include("dbapi/sql_strs.inc");
  include("dbapi/sql_util.inc");
 }

 if (!defined("COMUTILS_INC"))
  include("dbapi/comutils.inc");

 if (!defined("SQL_MSGGETFRIENDS_INC"))
  include("dbapi/sql_msggetfriends.inc");
 
function set_permissions($id, $user, $friend){
 global $myDB;
 
 $query = FormatSQL(SQL_UPDATE_PERMS, array($friend, $user, $id));
 $myDB->query($query);
 return $myDB->affected_rows();
}
  
// this function takes an userid and a trail. It returns a hash of two
// integers, containing the friend/user-permissions to the object
//
// $hsh['friend'] = <<bitflag>>
// $hsh['other']  = <<bitflag>>
//
// the trail must be in the Filesystem-like-Format
function get_permissions($trail){
 global $myDB;
 
 $query = FormatSQL(SQL_GET_PERMISSION, (array)$trail); 
 $myDB->query($query);
 if ($myDB->num_rows() <= 0) {
  return -1;
 }else{
  $myDB->next_record();
  $hsh['friend'] = $myDB->f("FriendAccess");
  $hsh['other']  = $myDB->f("UserAccess");
  return $hsh;
 }
}

//checks whether there is friendship between $uid1 and $uid2
/*
 Pilif's note: SQL_IS_FRIENDSHIP_REQUEST looks a bit strange and
 does return its data in a strange way. This is for historical reasons:
 There was a time where no Message System existed, but Friendships 
 did. What I tried when I introduced the Message System, was to keep the
 backend as consitent as possible. When I hate something, it is a change
 in a well known and frequently used interface. So I rather make a SQL-Query a bit
 slower than change the interface of a function.
 
 If I see that it does not work for performance reasons, I can still change it.
 But this time the extra compatibility-overhead takes less then 1/100 seconds.
*/
function is_friendship($uid1, $uid2){
 global $myDB;

 $query = FormatSQL(SQL_HARDFRIENDS, array($uid1, $uid2, $uid1, $uid2));
 $myDB->query($query);
 $myDB->next_record();
 if ($myDB->f("cnt") != 0) return true; //a closed friendship could be detected


 $query = FormatSQL(SQL_IS_FRIENDSHIP_REQUEST, array($uid1, $uid2, $uid2, $uid1));
 $myDB->query($query);
 if ($myDB->num_rows() == 0) return false; //no hard friendship, no request -> no relation !
 $myDB->next_record();

 $hsh['friend_requesting']      = $myDB->f("Sender");
 $hsh['friend_requesting_from'] = $myDB->f("Target");
 $hsh['final']					= false;
 return $hsh;
}

function create_friendship($uid1, $uid2){
 global $myDB;
$query = FormatSQL(SQL_INSERT_FRIENDS, array($uid1, $uid2));
$myDB->query($query);
return true;
}

function remove_friendship($uid1, $uid2){
 global $myDB;
 
 $query = FormatSQL(SQL_REMOVE_FRIENDSHIP, array($uid1, $uid2, $uid2, $uid1));
// die("<pre>$query</pre>");
 $myDB->query($query);
 return true;
}


/* 
   Pilif: The compatibility-note from is_friendship() is also valid here. I had about 15 Minutes
   to get the new SQL-Statements but I had nothing to change but them
*/
function get_friends($uid, $type){
 global $myDB;
 
 switch ($type) {
     case FT_ALL:
         $query = FormatSQL(SQL_GET_FRIENDS, array($uid, $uid, $uid, $uid));
         break;
     case FT_WAITING:
         $query = FormatSQL(WAITING_FOR_BEING_ACCEPTED_BY_MYSELF, array($uid));
		 break;
     case FT_AWAITED:
         $query = FormatSQL(WAITING_FOR_BEING_ACCEPTED_BY_MY_FRIENDS, array($uid));
         break;
	default:
	     die("Oops.. unknown friendshiptype to display in ".__FILE__." at line ".__LINE__);	 
 }
 
 $myDB->query($query);
 if ($myDB->num_rows() == 0)
  return 0;
 $x=0; 
 while ($myDB->next_record()){
  $hsh['username'] = $myDB->f("username");
  $hsh['userid']   = $myDB->f("user_id");
  $arr[$x] = $hsh;
  $x++;
 }
 reset($arr);
 return $arr; 
}

function count_friends($uid){
 global $myDB;
 
 $query = FormatSQL(SQL_COUNT_FRIENDS, array($uid, $uid));
 if (!$myDB->query($query)) return 0;
 if ($myDB->num_rows() == 0) return 0;
 $myDB->next_record();
 return $myDB->f("cnt");
}

?>