<?php
 define("TRAILS_INC", true);
/*
  trails.inc; 1.00
  
  Contains DataBase-Functions used to navigate through
  trails. You will find documentation in trails.inc.html
  
  Written by Pilif and Jody
  
  Created:  Mar,  9th 2000

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
 
 /*
  function get_links($path: String): array of hash;
  
   The function will take a Path (or an ID to save an extra-query) to
   a trail and will return an array of the following hash:
   
    $tmphash['url'] = "url";
    $tmphash['title'] = "title";
    $tmphash['description'] = "description";
   
   The array will be ordered to refect the positions of links that
   are stored within the datbase.
   
   The function returns -1 if there are no Links in the current
   trail.
*/
function get_links($path){
 global $myDB, $glob_language;
 
 if (is_integer($path)){
  $path = id2path($path);
  if ($path == -1) 
   return -1;
 }
  
 $query = FormatSQL(SQL_LINKS, array(addslashes($path), $glob_language));
 $myDB->query($query);
 if ($myDB->num_rows() <= 0) {
  return -1;
 }else{
  $idx = 0;
  while ($myDB->next_record()){
   $hsh['id']          = $myDB->f("Link_ID");
   $hsh['url']         = $myDB->f("Url");
   $hsh['title']       = $myDB->f("title");
   $hsh['description'] = $myDB->f("Description");
   $hsh['changedate']  = $myDB->f("ChangeDate");
   $hsh['adddate']     = $myDB->f("AddDate");
   $hsh['username']    = $myDB->f("username");
   $hsh['position']    = $myDB->f("Position");
   $hsh['uid']         = $myDB->f("user_id");
   $hsh['next']        = $myDB->f("Next");
   $arr[$idx] = $hsh;
   $idx++;
  }
  reset($arr);
  return $arr; 
 }  
}

function get_link_info($linkid){
 global $myDB;
 
 $query = FormatSQL(SQL_LINKINFO, (array)$linkid);

 $myDB->query($query);
 if ($myDB->num_rows() <= 0) {
  return -1;
 }else{
  $myDB->next_record();
  $hsh['id']          = $myDB->f("Link_ID");
  $hsh['url']         = $myDB->f("Url");
  $hsh['title']       = $myDB->f("title");
  $hsh['description'] = $myDB->f("Description");
  $hsh['changedate']  = $myDB->f("ChangeDate");
  $hsh['adddate']     = $myDB->f("AddDate");
  $hsh['username']    = $myDB->f("username");
  $hsh['uid']         = $myDB->f("user_id");
  $hsh['next']        = $myDB->f("Next");
  return $hsh; 
 }  
}

?>