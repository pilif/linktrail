<?php
page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm", "user" => "Linktrail_User"));
// page access requires that the user is authenticated and has "admin" permission
$perm->check("admin");
include("dbapi/comutils.inc");
include("dbapi/trails.inc");
include("dbapi/edit_links.inc");

include("template.inc");
 
$trail = base64_decode($trail);
$trailid = path2id($trail);
$obj['trail']       = $trailid; 
$obj['title']       = $title; 
$obj['description'] = $description;
$obj['url']         = $url; 
$obj['position']    = 0; 
$obj['owner']       = $auth->auth["uid"];
add_link($obj, true);
?>
<a href="./trail.php?v=<?php echo(base64_encode($trail)); ?>">Debug: Back</a>
<?php
page_close();
?>
