<?php
 define("USER_INC", true);
/*
  directory.inc; 1.01
  
  Contains DataBase-Functions used to build the directory-tree
  Documentation is inline and in directory.inc.html when ready
  
  Written by Pilif and Jody
  
  Created:  Feb, 20th 2000
  Modified: Aug, 19th 2000
 
  © 2000 by Linktrail. All rights reserved  
 */

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

function query_password($text, $is_username = true){
 global $myDB;
 
 $query = ($is_username) ? SQL_GET_PW_FROM_UNAME : SQL_GET_PW_FROM_EMAIL;
 $query = FormatSQL($query, array($text));
 $myDB->query($query);
 
 if ($myDB->num_rows() == 0)
  return "";
 
 $myDB->next_record();
 $hsh['password'] = $myDB->f("password");
 $hsh['email']    = $myDB->f("Email");
 $hsh['username'] = $myDB->f("username");
 return $hsh;
}

function get_suggestions($username){
 global $myDB;
 
 //Firstly, I'll try to get the next username with the next smallest available number appended
 $query = FormatSQL(SQL_USER_SUGGESTIONS, array($username));
 $myDB->query($query);
 srand((double)microtime()*1000000);
 
 $count = 0;
 while ($myDB->next_record())
  $badusers[$myDB->f("username")] = true;

 for ($x = 1; $x < 2000; $x++){
  if ($count >= 3) break; //only 3 with this algorithmus...
  if ( !$badusers[$username.$x]){
   $count++;
   $fertig[]   = $username.$x;
   $badusers[] = $username.$x;
   $x += rand(1,222);
  } //if... 
 } //for;
 
 //more a joke than really usable... :-)
 if (!user_exists(strrev($username))){
  $fertig[]   = strrev($username);
  $badusers[] = strrev($username);
 }
 
 //now do something really funny: add one to three random characters to the username
 //but do only try up to ten times to get a valid username and only get two nmes
 //generated this way.
 $count = 0;
 $found = 0;
 while( ($count < 10) and ($found < 3) ){
  $cc = rand(1,3);
  $testuser = $username.randomString($cc);
  if (!$badusers[$testuser]){
   $fertig[]   = $testuser;
   $badusers[] = $testuser;
   $found++;
  }
  $count++;
 }
 
 //oops... none did work... we have maaany users that all want to have the same name...
 //I want to have at least one suggestion, so try it with brute-force, but only 20 times.
 //if it STILL does not work, I give up and jump out of the window crying (about the very
 //bad ramdomizer in php...).
 if (count($fertig) == 0){
  $count = 0;
  while($count < 21){
   $testuser = $username.rand(2000,99999999);
   if (!$badusers[$testuser]){
    $fertig[]   = $testuser;
    $badusers[] = $testuser;
    break;
   } //user exists
   $count++;
  } //while
 } //last desperate algorithmus...
 
 return $fertig;
}

function user_exists($username){
 global $myDB;
 
 $query = FormatSQL(SQL_USER_EXISTS, array($username));
 $myDB->query($query);
 return ($myDB->num_rows() > 0);
}
  
 /*
     function get_user_from_name($name: string): hash
  
  The funtion will read the data of an user from the database.
  The function takes an UserID (uid) and returns the following Hash:
  
  the functin will return -1 if the user could not be found
*/

function get_user_from_name($name){
 global $myDB;

 $query = FormatSQL(SQL_GETUSERFUID, (array)$name);
 //die($query);
 $myDB->query($query);
 if ($myDB->num_rows() <= 0) {
 // die("fuckyou. ich will einen debugger");
  return -1;
 }else{
   $myDB->next_record();
   $hsh['User_ID']           = $myDB->f("user_id");
   $hsh['Username']          = $myDB->f("username");   
   $hsh['FirstName']         = $myDB->f("FirstName");
   $hsh['LastName']          = $myDB->f("LastName");
   $hsh['Email']             = $myDB->f("Email");
   $hsh['Homepage']          = $myDB->f("Homepage");
   $hsh['Description']       = $myDB->f("Description"); 
   $hsh['Gender']  	         = $myDB->f("Gender");
   $hsh['Age']  	         = $myDB->f("Age");
   $hsh['Country']           = $myDB->f("Country");
   $hsh['PhotoUrl']          = $myDB->f("PhotoUrl");
   $hsh['ShowNobody']        = $myDB->f("ShowNobody");
   $hsh['ShowFriend']        = $myDB->f("ShowFriend");
   $hsh['ShowAnyone']        = $myDB->f("ShowAnyone");
   $hsh['PopupPos']          = $myDB->f("PopupPos");
   $hsh['NS6Sidebar']        = $myDB->f("NS6Sidebar") == "y";
   $hsh['ResPerPage']        = $myDB->f("ResPerPage");
   $hsh['HighlightSearch']   = $myDB->f("HighlightSearch") == "y";
   $hsh['NotificationStyle'] = $myDB->f("NotificationStyle");
   $hsh['Vacation']  		 = $myDB->f("Vacation") == "y";
   $hsh['MypageAfterLogon']	 = $myDB->f("MypageAfterLogon") == "y";
   $hsh['LastOnline']	     = $myDB->f("LastOnline");
   $hsh['NewWindow']	     = $myDB->f("NewWindow") == "y";
   $hsh['LastReadStamp']     = $myDB->f("LastReadMessages");
   reset($hsh);
   return $hsh;
 }
}

function uid2name($uid){
 global $myDB;
 
 $query = FormatSQL(SQL_UID2NAME, array($uid));
 if (!$myDB->query($query)) return -1;
 $myDB->next_record();
 return $myDB->f("username");
}
 
?>