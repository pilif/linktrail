<?php
 define("SHARE_INC", true);

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

 function create_share_suggestion($trail, $suggestor, $target_user){
  global $myDB;
  $query = FormatSQL(SQL_SHARE_ADD, array($trail, $suggestor, $target_user));
  if($myDB->query($query))
    add_bonus($trail, BT_SUGGEST);

  return true;
 } 
 
 /*
  It does not make sense to either suggest a trail twice or to suggest 
  a trail that is already subscribed by the Target-User. The following
  function takes a trail and an array of users to test if they
  have already subscribed the trail (most usual: The friends of the
  current user).
  This function takes a trail and a reference to an array containing
  possible users (most usual a list of friends of the current user)
  and removes forbidden users from it.
 */
 function get_allowed_users($trail, &$userlist){
  global $myDB;
  if (!is_array($userlist))
   error('Invalid call', 'userlist is not an array', __LINE__, __FILE__);
  $str = "";
  for($x=0; $x < count($userlist); $x++)
   $str .= "'".$userlist[$x]['username']."',";
  $str = substr($str, 0, strlen($str)-1); //remove the last ,
  
  $query = FormatSQL(SQL_SHARE_PERM_OWNER_SUBS, array($str, $trail, $str, $trail, $str, $trail));
  $myDB->query($query);
//  die("<pre>$query</pre>");
  //don't call this strange code. it is not. it is actually guite cool
  while ($myDB->next_record()){
   $donot[$myDB->f("subscribed")] = 1;
   $donot[$myDB->f("owner")] = 1;
  }
  $query = FormatSQL(SQL_SHARE_SUGGESTED_MSGS, array($str));
  $myDB->query($query);
  while ($myDB->next_record()){
   $data = unserialize($myDB->f("Data"));
   if ($data['Trail'] == $trail)
    $donot[$myDB->f("username")] = 1;
  }
  foreach($userlist as $key => $value){
   if($donot[$value['username']] == 1){
    array_splice($userlist, $key, 1);
   }
  }
  reset($userlist);  
 } 
 
 function delete_suggestion($trail, $target_user){
  global $myDB;
 
  $query = FormatSQL(SQL_SHARE_DEL, array($target_user, $trail));
  $myDB->query($query);
  $count = $myDB->affected_rows();
  if ($count != 0)
    add_bonus($trail, BT_UNSUGGEST);
  return $count;
 }
?>