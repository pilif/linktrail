<?php
if (!defined("TRAILS_INC"))
 include("dbapi/trails.inc");

if (!defined("LAY_TRAIL_INC"))
 include("layout/lay_trail.inc");

include("template.inc");
 
$id   = substr($PATH_INFO, 1); 
$info = get_link_info($id);
if ($info == "-1"){
 Header("Location: /");
 exit;
}
print(print_redirecting($info['url'], $info['title']));
?>                            

