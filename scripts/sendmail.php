#!/usr/local/bin/php -q
<?php

ini_set("include_path", "./:/home/linktrai:/home/linktrai/templates:/home/linktrai/includes:/usr/local/phplib");
define("APPLICATION_HOME", "/home/linktrai");
define("TEMPLATE_ROOT", APPLICATION_HOME."/templates/en/");

if (empty($_PHPLIB))
 $_PHPLIB = "";

if (!is_array($_PHPLIB)) {
  $_PHPLIB["libdir"] = "/usr/local/phplib/"; 
}
require("config/config.inc");
require($_PHPLIB["libdir"] . "db_mysql.inc");  
require($_PHPLIB["libdir"] . "ct_sql.inc");    
require($_PHPLIB["libdir"] . "session.inc");   
require($_PHPLIB["libdir"] . "auth.inc");      
require($_PHPLIB["libdir"] . "perm.inc");      
require($_PHPLIB["libdir"] . "user.inc");      
require("phplib/local.inc");
require($_PHPLIB["libdir"] . "page.inc");

global $myDB;

if (!is_object($myDB)) {
  $myDB = new DB_Linktrail;
  include("dbapi/sql_strs.inc");
  include("dbapi/sql_util.inc");
}

include("template.inc");
include("dbapi/messages.inc");

if (!defined("MESSAGE_CLASSES_INC"))
 include("commonapi/message_classes.inc");

include("dbapi/permissions.inc");
include("dbapi/tmypage.inc");
if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");

$where_clause = "
 (Email != '' AND Email IS NOT NULL)
AND
 ( (UNIX_TIMESTAMP(LastSent) + (NotificationStyle * 7 * 24 * 60 * 60)) < UNIX_TIMESTAMP() )
AND
 ( BounceFlag != 'fail' ) 
";

function sql_error($error, $empty){
 die("\n\tSQL-Error: ".$error['sql_errdesc']."\n");
}

function update_check_flag(){
 global $myDB, $where_clause;
 
 $query = sprintf("UPDATE ltrUserData SET BounceFlag = 'bounce_check' WHERE %s AND (BounceFlag != 'ok')", $where_clause);
 $myDB->query($query);
} 

function update_time_stamp(){
 global $myDB, $where_clause;
 
 $query = sprintf("UPDATE ltrUserData SET LastSent = NOW() WHERE %s", $where_clause);
 $myDB->query($query);
}

function encode_address($str){
 return preg_replace('/([A-Z])/e', "strtolower('.\\1')", base64_encode($str));
}

function send_one_mail($email, $mail_name, $subject, $message){
 $old_error = error_reporting (0);
 $mail_header  = "";
 $mail_header .= sprintf("From: %s <%s>\n", "linktrail team", "preview@linktrail.com");
 $mail_header .= sprintf("To: %s <%s>\n", $mail_name, $email);
 $mail_header .= sprintf("Subject: %s (%s)\n", $subject, strftime("%m/%d/%y", time()));
 $mail_header .= "MIME-Version: 1.0\n";
 $mail_header .= "Content-Type: text/plain;\n";
 $mail_header .= "              charset=\"ISO-8859-1\"\n\n";
 $message  =  $mail_header.$message;
 $env_from = 'webreply-'.encode_address($email).'@linktrail.com';
 //die("/usr/lib/sendmail -f $env_from $email\n");
 $exim     = popen("/usr/lib/sendmail -f $env_from $email", "w");
 printf("\t Exim: $exim \n");
 fputs($exim, $message);
 fclose($exim);
 error_reporting($old_error);
}

function get_all_users(){
 global $myDB, $where_clause;
 
 $arr = array();
 $query = sprintf("SELECT FirstName, LastName, Email, LastSent, LastOnline, ltrUserData.User_ID, auth_user.username as Username  FROM ltrUserData, auth_user WHERE %s AND auth_user.user_id = ltrUserData.User_ID", $where_clause);
 //die("\n\n$query\n\n");
 if (!$myDB->query($query)) return $arr;
 if ($myDB->num_rows() == 0) return $arr;
 while($myDB->next_record()){
  $arr[] = $myDB->Record; //WARNING: Not portable code!
 }
 return $arr;
}

function get_recommendation_count($user_id){
 global $myDB;
 
 $query = sprintf("SELECT * FROM ltrMessages WHERE MessageType = %d AND Target = '%s'", LTMSG_SUGGESTN, $user_id);
 if (!$myDB->query($query)) return 0;
 if ($myDB->num_rows() == 0) return 0;
 $count = 0;
 while($myDB->next_record()){
  $obj = create_message_object($myDB->Record);
  if (!isset($obj->data['action_done'])) 
   $count++;
 }
 return $count;
}


function get_new_trail_count($date){
 global $myDB;
 
 $query = "
  SELECT 
   COUNT(*) AS cnt 
  FROM 
   ltrDirectory
  LEFT JOIN ltrDirectoryData ON ltrDirectoryData.Node_ID = ltrDirectory.Node_ID
  WHERE AddDate > '$date'
 ";
// die("\n\n\n $query \n\n\n");
 if (!$myDB->query($query)) return 0;
 if ($myDB->num_rows() == 0) return 0;
 $myDB->next_record();
 return $myDB->f("cnt");
}


$new_trails = -1;
printf("linktrail Application 0.1\nAuto-Email\n\n");
printf("Setting new BounceFlag....\n");
update_check_flag();
printf("Getting users to send message to.... ");
$users = get_all_users();
printf("found %d users\n\n", count($users));
foreach($users as $user){
 printf("User: %s\n", $user['Username']);

 printf("\tGetting Message-Count...\t\t");
 $msg_count = count_new_messages($user['User_ID'], 0);
 printf("%d unread messages\n", $msg_count);

 printf("\tGetting Recommendations-Count...\t");
 $rec_count = get_recommendation_count($user['User_ID']);
 printf("%d pending recommendations\n", $rec_count);

 printf("\tFriendship-Request-Count...\t\t");
 $req_count = get_friends($user['User_ID'], FT_WAITING);
 $req_count = is_array($req_count) ? count($req_count) : 0;
 printf("%d pending friendship requests\n", $req_count);
 
 if ($new_trails == -1){
  printf("\tGetting count of new trails...\t\t");
  $new_trails = get_new_trail_count($user['LastSent']);
  printf("found %d trails\n", $new_trails);
 }
 
 printf("\tGetting list of Trails on Mypage...\n");
 $trails = read_trails_mypage($user, -1, "AddDate");
 printf("\t\t... read trails (%d found)\n", count($trails['trails']));
 $upd_trails = Array();
 for($x = 0; $x < count($trails['trails']); $x++){
  $cdate = unixdate(friendlydate($trails['trails'][$x]['changedate']));
  $adate = unixdate(friendlydate($trails['trails'][$x]['adddate']));
  $udate = unixdate(friendlydate($user['LastOnline']));
  $str = "";
  if ($udate != -1){
   if ($cdate > $udate)
    $str = "(updated)";
   if ($adate >= $udate) //added has priority before changed
    $str = "(new)";
   if ($str != "")
    $upd_trails[] =  array( "url" => build_good_url($trails['trails'][$x]['path']), "flag" => $str);
  }
 }
 printf("\t\t... build updated_array (found %d trails)\n", count($upd_trails));
 
 $tpl = new Template(APPLICATION_HOME."/templates/mails", "keep");
 $tpl->set_file(array("mail" => "weekly_report.txt"));
 
 $tpl->set_var("USERNAME", $user['Username']);
// $tpl->set_var("LASTNAME", $user['LastName']);
 
 $strs['unread'] = ($msg_count > 0) ? " - $msg_count unread message(s)\n": "";
 $strs['freq']   = ($req_count > 0) ? " - $req_count pending friendship request(s)\n": "";
 $strs['recc']   = ($rec_count > 0) ? " - $rec_count trail recommendation(s)\n": ""; 
 
 if ( ($strs['unread'] == "") and ($strs['unread'] == "") and ($strs['unread'] == ""))
   $stats_i = "Since your last visit, there were no\nnew messages, no pending friendship requests and no new trail\nrecommendations in your message center.";
 else
   $stats_i = "Since your last visit to linktrail,\nthere have been the following changes in your message center:\n";
 
 $tpl->set_var("STATS_INTRODUCTION", $stats_i);
 $tpl->set_var("UNREAD", $strs['unread']);
 $tpl->set_var("FREQ", $strs['freq']);
 $tpl->set_var("RECC", $strs['recc']);
 
 $tpl->set_var("NEWTRAILS", $new_trails);
 $str = "";
 foreach($upd_trails as $trail){
  $str .= " - ".friendlyname($trail['url'])." ".$trail['flag']."\n";
 }
 if ($str != ""){
  $str = "\nMoreover, the following trails from your Mypage have been updated since\nyour last visit:\n\n".$str;
  $str .= "\nPlease visit your Mypage:\nhttp://preview.linktrail.com/Experts/".rawurlencode($user['Username'])."?dologin=1\n";
 }  
 $tpl->set_var("TRAILLIST", $str);
 $tpl->set_var("UNSUBURL", "http://preview.linktrail.com".build_good_url("/Experts/".$user['Username']."/Settings/Preferences"));
 $tpl->parse("main", "mail");
 $body = $tpl->get("main");
 printf("\tBuilt mail body... preparing to send\n");
 //printf("\n\n\n$body\n\n");
 send_one_mail($user['Email'], $user['Username'], 'linktrail report', $body);
 printf("\tMail sent. Proceeding to next user\n\n");
}
printf("All users done. Setting new LastSent-Timestamp\n");
 update_time_stamp();
printf("Finished!\n\n");
?>
