<?php
function form(){
 global $mytrail, $ltrstr, $sess;

print(msg_box($ltrstr['Edit Permissions'], print_permission_form($mytrail['friendaccess'], $mytrail['useraccess']), $mytrail, $mytrail['path'], 0, $ltrstr['Back without perms']));
}

function doit(){
 global $sess, $HTTP_POST_VARS, $mytrail;
 
 $newf = 0;
 $newo = 0;
 
 if (isset($HTTP_POST_VARS["friend_edit"])) $newf = $newf | PERM_EDIT;
 if (isset($HTTP_POST_VARS["friend_del"]) ) $newf = $newf | PERM_DEL;
 if (isset($HTTP_POST_VARS["friend_move"])) $newf = $newf | PERM_MOVE;
 if (isset($HTTP_POST_VARS["friend_add"]) ) $newf = $newf | PERM_ADD;
 
 if (isset($HTTP_POST_VARS["other_edit"]))  $newo = $newo | PERM_EDIT;
 if (isset($HTTP_POST_VARS["other_del"]) )  $newo = $newo | PERM_DEL;
 if (isset($HTTP_POST_VARS["other_move"]) ) $newo = $newo | PERM_MOVE;
 if (isset($HTTP_POST_VARS["other_add"]))   $newo = $newo | PERM_ADD;

 set_permissions($mytrail['id'], $newo, $newf);
//die("New-User: $newo<br>New-Friend: $newf"); 

 
$sess->unregister("mytrail");
header("Location: ".$sess->url($mytrail['path']));
}


if (!defined("LAY_TRAIL_INC"))
 include("layout/lay_trail.inc");
if (!defined("EDIT_LINKS_INC"))
 include("dbapi/edit_links.inc");
if (!defined("TRAILFUNCS_INC"))
 include("commonapi/trailfuncs.inc");
/*
 First I read the permissions of our user. 
*/
if (!defined("COMMON_PERMISSIONS_INC"))
 include("commonapi/common_permissions.inc");
if (!defined("PERMISSIONS_INC"))
 include("dbapi/permissions.inc");


page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm", "user" => "Linktrail_User"));

$mytrail = get_node_info($PATH_INFO);
if ($mytrail == -1)
 $mytrail = get_node_info($PATH_INFO."?");

if ($auth->auth['uid'] != $mytrail['userid']){
 page_close();
 Header("Location: ".$sess->url($PATH_INFO));
 exit;
}
 

$caps       = get_caps($perm); //used to read the superuser-capability of users with perm->have-perm("admin");
$trailperms = relevant_perms($mytrail, $auth->auth["uid"], $caps);

if (!class_exists("Template"))
 include("template.inc");

if ($action == "exec"){
 doit();
}else{
 form();
}           

page_close();
?>
