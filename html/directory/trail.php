<?php
page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm", "user" => "Linktrail_User"));
if ($HTTP_POST_VARS['username'] != "")
 page_close(); //call this one more if the user just logged on to prevent the state
               //from being overwritten if parent-node-window reloads!

if (preg_match('/^\/Experts\/(.+)/', $PATH_INFO, $found)){
 Header("Location: ".$sess->url("/directory/directory.php$PATH_INFO"));
 exit;
}
include("template.inc");
$in_trail = 4;
$nobody = ($auth->auth["uid"] == "nobody");

if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");

if (!defined("TRAILS_INC"))
 include("dbapi/trails.inc");

if (!defined("DIRECORY_INC"))
 include("dbapi/directory.inc");

if (!defined("LAY_TRAIL_INC"))
 include("layout/lay_trail.inc");
if (!defined("BUTTONS_INC"))
 include("layout/buttons.inc");
if (!defined("ACTIONS_INC"))
 include("layout/actions.inc");
if (!defined("TRAILFUNCS_INC"))
 include("commonapi/trailfuncs.inc");
if (!defined("COMMON_PERMISSIONS_INC"))
 include("commonapi/common_permissions.inc");

$trail = $PATH_INFO;
//$sqltrail = addslashes($trail);
$nodeinfo = get_node_info($trail);
if ($nodeinfo == -1){
 $nodeinfo = get_node_info($trail.'?');
 $trail = $trail.'?';
}
$links = get_links($trail);
$mytrail = $nodeinfo;
$hilight_words = explode("|", base64_decode($hilight));
/*
 First I read the permissions of our user. 
*/
$caps       = get_caps($perm); //used to read the superuser-capability of users with perm->have-perm("admin");
$trailperms = relevant_perms($nodeinfo, $auth->auth["uid"], $caps);

//die("Perms: $trailperms");

//$sess->register("mytrail");
 
$auth->login_if(($dologin == "1") and (empty($password)));

print_trail_direct($links, $nodeinfo, $trailperms);

/*echo("sid: ".$sess->id." uid: ".$auth->auth['uid']."<p>");
echo("Test ist: $test<p>");*/
page_close();
//echo("Test ist: $test");
?>
