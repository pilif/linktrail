<?php
/*
 * Session Management for PHP3
 *
 * Copyright (c) 1998,1999 SH Online Dienst GmbH
 *                    Boris Erdmann, Kristian Koehntopp
 *
 * $Id: local.inc,v 1.2 2000-11-20 21:21:18 pilif Exp $
 *
 */ 

class DB_Linktrail extends DB_Sql {
  var $Host     = DB_SERVER;
  var $Database = DB_DATABASE;
  var $User     = DB_USER;
  var $Password = DB_PASS;
  var $Debug    = false;
  
  function haltmsg($msg){
   global $in_trail, $ltrstr;
   
   $error['short']       = "SQL - Error";
   $error['description'] = $ltrstr['Database Error'];
   $error['filename']    = basename(__FILE__);
   $error['line']        = __LINE__;
   $error['sql_errno']   = $this->Errno;
   $error['sql_errdesc'] = $this->Error;
   $error['sql_query']   = $this->Query;
   sql_error($error, isset($in_trail));
  }
}

class DB_Tester extends DB_Sql {
  var $Host     = DB_SERVER;
  var $Database = "AlphaTest";
  var $User     = "root";
  var $Password = "oelberg";
  var $Debug    = false;
  
  function haltmsg($msg){
   echo("error: $msg"); 
  }
}


class Linktrail_CT_Sql extends CT_Sql {
  var $database_class = "DB_Linktrail";    	  ## Which database to connect...
  var $database_table = "phplib_active_sessions"; ## and find our session data in this table.
}

class Linktrail_Session extends Session {
  var $classname = "Linktrail_Session";

  var $cookiename     = "sid";                ## defaults to classname
  var $magic          = "Linktrailistcool33";      ## ID seed
  var $mode           = "cookie";          ## We propagate session IDs with cookies
  var $fallback_mode  = "env";
  var $allowcache     = "passive";
  var $lifetime       = 0;                 ## 0 = do session cookies, else minutes
  var $that_class     = "Linktrail_CT_Sql"; ## name of data storage container
  //var $that_class = "Linktrail_CT_Shm";
  var $gc_probability = 20;  
}

class Linktrail_User extends User {
  var $classname = "Linktrail_User";

  var $magic          = "Linktrailrocks!";     ## ID seed
  var $that_class     = "Linktrail_CT_Sql"; ## data storage container
}

class Linktrail_Auth extends Auth { //this class uses default_authentication.
  var $classname      = "Linktrail_Auth";

  var $lifetime       =  30;

  var $database_class = "DB_Linktrail";
  var $database_table = "auth_user";
  var $nobody    = true; // <- defauth

function logout_done(){
 global $glob_userdata, $sess, $HTTP_COOKIE_VARS;
// die("logout_done");
  //echo("logout_done<br>");
 $glob_userdata = "";
 $sess->unregister("glob_userdata");
} 
 
function login_done(){
 global $glob_userdata, $glob_immp, $sess, $PHP_SELF, $REQUEST_METHOD;
  //echo("login_done<br>");
 if (!defined("USER_INC"))
  include("dbapi/user.inc");
 if (!defined("EDIT_USER_INC"))
  include("dbapi/edit_user.inc");
// die("login done");
 $glob_userdata = get_user_from_name($this->auth['uname']);
 $sess->register("glob_userdata");
 $glob_immp = false;
 $glob_immp = $glob_userdata['MypageAfterLogon'];
 $sess->register("glob_immp");
 update_logon_date($this->auth['uid']);
 
// page_close();
// if ($REQUEST_METHOD == "POST"){
//  Header("Location: $PHP_SELF");
//  exit;
// } 
}
  
 function auth_preauth(){
   global $sess, $logon_now_as, $HTTP_COOKIE_VARS;
   if (isset($logon_now_as)){
    $sess->unregister("logon_now_as");
    $query = sprintf("select perms, username from %s where user_id='%s'", $this->database_table, $logon_now_as);
    $this->db->query($query);
    while($this->db->next_record()) {
      $this->auth["perm"] = $this->db->f("perms");
      $this->auth["uname"] = $this->db->f("username");
    }
    return $logon_now_as;
   }
  }
  
  function auth_loginform() {
    global $sess, $auth;
    global $_PHPLIB;
	global $PHP_SELF, $PATH_INFO, $REQUEST_URI, $node, $kat, $in_trail, $backurl;

	$backurl = $PHP_SELF;
	$sess->register("backurl");
	if (!defined("COMUTILS_INC"))
     include("dbapi/comutils.inc");
	if (!isset($in_trail))
	 $in_trail = 0;
    if ( (is_trail($PHP_SELF)) and (!preg_match('/\/Tour/', $PHP_SELF)) and (!preg_match('/\/Experts/', $PHP_SELF)) and (!preg_match('/\/About/', $PHP_SELF)) and (!preg_match('/\/Help/', $PHP_SELF))){
      $in_trail = 4; 
	}
    if ($in_trail != 0) 
	 include("logon_intrail.php"); // in the phplib-subdir 
	else
	 include("register.php"); //register.php is in the phplib-Subdir of the Application
  }
  
  function auth_validatelogin() {
    global $username, $password, $savepass, $HTTP_POST_VARS;

    if(isset($username)) {
      $this->auth["uname"]=strtolower($username);        ## This provides access for "loginform.ihtml"
    }
    
    
    $uid = false;
    $query = sprintf("select user_id, perms ".
                             "        from %s ".
                             "       where username = '%s' ".
                             "         and password = '%s'",
                          $this->database_table,
                          addslashes($username),
                          addslashes($password));
    $this->db->query($query);

    while($this->db->next_record()) {
	  $uid = $this->db->f("user_id");
      $this->auth["perm"] = $this->db->f("perms");
    }
    return $uid;
  }
  
  function first_step(){
   global $PHP_SELF, $backurl, $sess, $forcelogin;

   if ($forcelogin == "1"){
    return "form";
   }
   $result = $this->auth['uid'];
   if ( isset($backurl) and ($backurl != $PHP_SELF)){
    $result = "nobody";
	$backurl = "";
	$this->unauth("true");
   }
   $sess->unregister("backurl");
   return $result;
  }
  
}


class Linktrail_Perm extends Perm {
  var $classname = "Linktrail_Perm";
  
  var $permissions = array(
                            "user"       => 1,
                            "red"        => 2,
                            "unused2"    => 4,
                            "unused3"    => 8,
                            "admin"      => 16
                          );

  function perm_invalid($does_have, $must_have) {
    global $perm, $auth, $sess;
    global $_PHPLIB;
    
    include($_PHPLIB["libdir"] . "perminvalid.ihtml");
  }
}
?>
