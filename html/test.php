<?php page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm")); ?>
<html>
<head>

<LINK REL=stylesheet HREF="/styles/trail_1.css" TYPE="text/css">

<body>
<?
//phpinfo();
include("dbapi/tmypage.inc");
include("dbapi/trailforyou.inc");
include("dbapi/user.inc");
//$auth->login_if( ($auth->auth["uid"] == "nobody") );
$userdata = get_user_from_name('pilif');
$trailsdata  = read_trails_mypage(&$userdata, -1, 'ChangeDate', "DESC");
$all_for_you = read_trails_for_you($trailsdata['trails'], 'pilif', 'buffy135863c15e437b61bfed3ca4d55');
foreach($all_for_you as $sug_trail){
 echo($sug_trail['path']."<br>\n");
}
srand((double) microtime() * 1000000);
$rand = rand(0, count($all_for_you));
printf("<p>Trail: <b>%s</b><br>%s", $all_for_you[$rand]['path'], $all_for_you[$rand]['description']);

?> 
</body>
</html> 
<?php page_close(); ?>                           
