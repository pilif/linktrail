<?php
 define("EDIT_USER_INC", true);

 /*
   The following is common to all DBAPI-Units. The DB-Connection 
   will only be opened once
 */

 global $myDB;

 
 if (!is_object($myDB)) {
  $myDB = new DB_Linktrail;
  include("dbapi/sql_strs.inc");
  include("dbapi/sql_util.inc");
 }
 
 if (!defined("COMUTILS_INC"))
  include("dbapi/comutils.inc");
 
function update_logon_date($uid){
 global $myDB;
 
 $query = FormatSQL(SQL_UPDATE_USER_LOGIN, array($uid));
 $myDB->query($query);
}

function set_user_at_name($name, $newuserdata){
 global $myDB, $glob_userdata;
// die($glob_userdata);
 $glob_userdata  = $newuserdata;
 //for FormatSQL, I am copying the hash to a linear array
 $uid = $newuserdata['User_ID']; // will need this one later;
 unset($newuserdata['User_ID']);
 unset($newuserdata['Username']);
 unset($newuserdata['LastOnline']);
 unset($newuserdata['LastReadStamp']);
 unset($newuserdata['BounceFlag']);

 $newuserdata['MypageAfterLogon'] = ($newuserdata['MypageAfterLogon'])  ? "y" : "n";
 $newuserdata['HighlightSearch']  = ($newuserdata['HighlightSearch'])  ? "y" : "n";
 $newuserdata['NS6Sidebar']       = ($newuserdata['NS6Sidebar'])  ? "y" : "n";
 $newuserdata['Vacation']         = ($newuserdata['Vacation'])  ? "y" : "n";
 $newuserdata['NewWindow']        = ($newuserdata['NewWindow'])  ? "y" : "n";
 
 
 foreach($newuserdata as $key => $value){
//  echo("$key = $value<br>");
  $userarr[] = $value;
 }
 $userarr[] = $uid;
// die($uid);
 $query = FormatSQL(SQL_UPDATE_USER, $userarr);
 $myDB->query($query);
}

function change_password($user, $newpassword){
 global $myDB;

 $query = FormatSQL(SQL_CHANGE_PASSWORD, array($newpassword, $user));
 $myDB->query($query);
}

function add_user($userdata){
 global $myDB;
 
 srand((double)microtime()*1000000);
 $uid = md5(uniqid(rand()+$username+rand()));
 $query = FormatSQL(SQL_ADD_USER1, array($uid, $userdata['name'], $userdata['password'], 'user'));
 $myDB->query($query);
 $query = FormatSQL(SQL_ADD_USER2, array($userdata['email'], $uid));
 $myDB->query($query);
 return $uid;
}

 
?>