<?php
define("COMMON_PERMISSIONS_INC", true);
if (!defined("PERMISSIONS_INC"))
  include("dbapi/permissions.inc");
/*
 These are general capabilities that are not connected to a particular
 object. They are instead depending of the state of the user ( [not] logged on)
*/
define("CAP_ADD_TRAIL", 32);
define("CAP_VIEW_MYPAGE", 64);
define("CAP_REGISTER", 256);
define("CAP_LOGON", 512);
define("CAP_LOGOFF", 1024);
define("CAP_DOTOUR", 2048);
define("CAP_ADMIN_DIRECTORY", 4096);
define("CAP_SUPERUSER", 8192); //has full perms over links/trails. May add/delete/move anything anwyhere
define("CAP_CHANGE_USER_PREFS", 16384);
define("CAP_EDIT_SLOTS", 32768); //may edit slots
define("CAP_EDIT_EXPERTS", 65536); //may edit slots
define("CAP_REPORT_BUG", 131072);
define("CAP_VIEW_MESSAGES", 262144);
define("CAP_CONTACT_AUTHOR", 524288);
/*
 These are permissions connected to the state of an user depending 
 to an object.
*/
define("PERM_EDIT", 1);
define("PERM_DEL", 2);
define("PERM_ADD", 4);
define("PERM_MOVE", 8);
define("PERM_CHANGE_PERMS", 16);
define("PERM_MOVE_TRAIL", 32); //this is not saved with the object but it is a permission
                               //anyway.

//returns the capabilities-bitflag. Takes the PHPLIB-Perms-Object
function get_caps($perms, $kat="", $uid="", $in_login = false){
 global $auth;
 
 if (is_object($perms) and ($auth->auth['uid'] != "")){
     if ($perms->have_perm("admin"))
      $cap = CAP_ADD_TRAIL | CAP_VIEW_MESSAGES | CAP_VIEW_MYPAGE | CAP_LOGOFF | CAP_ADMIN_DIRECTORY | CAP_SUPERUSER | CAP_CHANGE_USER_PREFS | CAP_EDIT_SLOTS | CAP_EDIT_EXPERTS | CAP_REPORT_BUG | CAP_CONTACT_AUTHOR;
     elseif ($perms->have_perm("red")){
       $cap = CAP_ADD_TRAIL | CAP_VIEW_MESSAGES | CAP_VIEW_MYPAGE | CAP_LOGOFF | CAP_CHANGE_USER_PREFS | CAP_EDIT_SLOTS | CAP_EDIT_EXPERTS | CAP_DOTOUR | CAP_REPORT_BUG | CAP_CONTACT_AUTHOR;
     } 
     elseif ($perms->have_perm("user")){
       $cap = CAP_ADD_TRAIL | CAP_VIEW_MESSAGES | CAP_VIEW_MYPAGE | CAP_LOGOFF | CAP_CHANGE_USER_PREFS | CAP_DOTOUR | CAP_REPORT_BUG | CAP_CONTACT_AUTHOR;
     } 
     else
      $cap = CAP_ADD_TRAIL | CAP_REGISTER | CAP_LOGON | CAP_DOTOUR | CAP_REPORT_BUG | CAP_CONTACT_AUTHOR; 
     $cap = ( (!$perms->have_perm("admin")) and (($kat == "/") or ($kat == '/Experts/New_User'))) ? ($cap & ~CAP_ADD_TRAIL) : $cap; 
     $cap = ( $kat == "/Experts/New_User" ) ? ($cap &~ CAP_REGISTER) : $cap;

     $cap = ( $kat == "/Experts/".$auth->auth['uname']."/Settings" ) ? ($cap &~ CAP_CHANGE_USER_PREFS) : $cap;
     $cap = ( $kat == "/Experts/".$auth->auth['uname']) ? ($cap &~ CAP_VIEW_MYPAGE) : $cap;
     $cap = ( $kat == "/Experts/".$auth->auth['uname']."/Messages" ) ? ($cap &~ CAP_VIEW_MESSAGES) : $cap;
     $cap = ( (strstr($kat, "/Experts/".$auth->auth['uname']) and ($auth->auth['uname'] != "") )) ? ($cap &~ CAP_CONTACT_AUTHOR) : $cap;
     $cap = ( $in_login ) ? ($cap &~ CAP_LOGON ) : $cap;
 }else{
   $cap = CAP_REGISTER | CAP_DOTOUR; 
  }
 return ($cap); 
} 

function has_caps($caps, $cap){
 return (($caps & $cap) != 0);
}

function add_cap($caps, $cap){
 $newcaps = $caps | $cap;
 return $newcaps;
}

function del_cap($caps, $cap){
 $newcaps = $caps & ~$cap;
 return $newcaps;
}

function is_owner($object, $username){
 return ($username == $object['username']);
}

/*
 integer function relevant_perms($object, $uid, $caps="")
 
 This function returns the relevant permissions of the given uid to the given object.
 It checks whether there is friendship between $uid and the owner $object.
 
 If $caps contains CAP_SUPERUSER, it returns all rights.
*/
function relevant_perms($object, $uid, $caps){
 if ( ($uid == $object['userid']) or (has_caps($caps, CAP_SUPERUSER)) )
  return PERM_EDIT | PERM_DEL | PERM_ADD | PERM_MOVE | PERM_CHANGE_PERMS | PERM_MOVE_TRAIL;
 if (is_friendship($uid, $object['userid']) === true )  //=== because is_friendship can return an array and Array == true
  return $object['friendaccess'];
 else
  return $object['useraccess'];
}
?>