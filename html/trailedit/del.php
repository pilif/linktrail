<?php
if (!defined("LAY_TRAIL_INC"))
 include("layout/lay_trail.inc");
if (!defined("EDIT_LINKS_INC"))
 include("dbapi/edit_links.inc");

include("template.inc");

function ask_del($link){
 global $PHP_SELF, $s;
 
 print ask_delete($link['id'], $link['trail'], $PHP_SELF);
 $s=2;
}

function delete($id){
 
 rm_link($id, true);
}

page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm", "user" => "Linktrail_User"));
// page access requires that the user is authenticated and has "admin" permission
if (1 == $s){
 $linkdata['id'] = $i;
 $copy = $linkdata;
 ask_del($copy);
}else{
 if (isset($yes)){
  delete($linkdata['id']);
  ?>Debug: <b>Deleted</b>. <a href="./trail.php?v=<?php print(base64_encode($linkdata['trail'])); ?>">Back</a> to trail<?php
 }else{
  ?>Debug: <b>Ignored</b>. <a href="./trail.php?v=<?php print(base64_encode($linkdata['trail'])); ?>">Back</a> to trail<?php
 }
 $s = 0;
 $sess->unregister("s");
 $sess->unregister("linkdata");
}
page_close();
?>
