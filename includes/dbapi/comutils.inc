<?php
 define("COMUTILS_INC", true);
/*
  comutils.inc; 1.01
  
  Contains common-utility-functions
  Documentation is inline and in comutils.inc.html when ready
  
  Written by Pilif and Jody
  
  Created:  Feb, 20th 2000
  Modified: Mar,  5th 2000
 
  © 2000 by Linktrail. All rights reserved  
 */

 global $myDB;

 
 if (!is_object($myDB)) {
  $myDB = new DB_Linktrail;
  include("dbapi/sql_strs.inc");
  include("dbapi/sql_util.inc");
 }


function make_short($string, $len=24){
 if ( (! strchr($string, ' ')) and (strlen($string) >= $len))
  return substr($string, 0, $len-3).'...';
 else
  return $string;
} 
 
function remove_from_cache($filename){
/* if ( file_exists($filename) ){
  unlink($filename); 
 } */
}

function write_to_cache($filename, $contents){
 //create or owerwrite file
/* if ($fp = fopen ($filename, "w")){
  if ( flock($fp,2) ){
   $written = fputs ($fp, $contents); 
   fclose ($fp);
   //if the string could not be fully written, unlink the currupt file
   if ($written != strlen($contents))
    unlink($filename);
  }  
 } */
}
 
function hilight($string, $words){
 global $glob_userdata;
 if ($glob_userdata['HighlightSearch'] == "")
  $glob_userdata['HighlightSearch'] = true;
 if ( (!is_array($words)) or (!$glob_userdata['HighlightSearch'])) return $string;
 foreach($words as $word){
   if ($word != ""){
     $word = str_replace("/", "\/", preg_quote($word)); 
     if (preg_match("/<(.*".$word.".*)>/Ui", $string, $matches)){
      $tag = $matches[1];
      $string = preg_replace ("/<\/(.*".$word.".*)>/Ui", '|/|||', $string);
      $string = preg_replace ("/<(.*".$word.".*)>/Ui", '||||', $string);
     } 
     $string = preg_replace ("/([^|]*)(".$word.")([^|]*)/Ui", '\1<span style="background-color: '.SEARCH_HILIGHT.'; font-weight: bold;">\2</span>\3', $string);
     $string = str_replace('||||', "<$tag>", $string);
     $endtag = substr($tag, 0, strpos($tag, ' '));
     $string = str_replace('|/|||', "</$endtag>", $string);
   }  
 }
 return $string;   
} 

function extract_file_ext($filename){
 $index = strrpos($filename, '.');
 if ($index == 0) return "";
 return strtolower(substr($filename, $index));
}
 
function build_restriction_list($kat, $experts_first=false){
 $str = "";
$kat = ereg_replace('^(.*)Search$', '\\1', $kat);
if ( ereg('^/Experts/(.*)$', $kat, $erg) ){
  $tmp    = explode('/', $erg[1]);
  $erg[1] = $tmp[0];
 }elseif ( (! ereg('^/About', $kat)) and ($kat != "/")){
  $str .= '<option value="'.$kat.'">in this category</option>'."\n";
 }
 if ($experts_first){
  $str .= '<option value="/">in linktrail</option>'."\n";
  $str .= '<option value="/Experts/" selected>in experts</option>'."\n";
 }else{
  $str .= '<option value="/" selected>in linktrail</option>'."\n";
  $str .= '<option value="/Experts/">in experts</option>'."\n"; 
 } 
 return $str;
} 
 
function str_contains($str, $tokens){
 if (!is_array($tokens))
  return false;
 foreach($tokens as $token){
  if (strstr($str, $token)) return true;
 }
 return false; 
}
 
 /*
  Pascal would force me to have only one exit-point. Pascal is
  clearly the better langiuage... ;-)
 */
function str_is_in($str, $tokens){
 if (!is_array($tokens))
  return false;
 foreach($tokens as $token){
  if ($str == $token) return true;
 }
 return false; 
}
 
function br2nl($str){
 return str_replace("<br>", "\n", $str);
} 
 
function plattform_nl2br($str){
 $str = str_replace("\r\n", "\n", $str);
 $str = str_replace("\r", "\n", $str);
 return nl2br($str);
}
 
function make_hidden($name, $value){
 return sprintf('<input type="hidden" name="%s" value="%s">%s', $name, $value, "\n");
} 
 
function html_attribute($str, $convert_space=true){
 if($convert_space)
  $str = str_replace("_", " ", $str);
 $str = str_replace('"','&quot;', $str);
 return $str;
}

function alter_url($url, $parameter_name, $newvalue, $oldvalue=""){
  global $HTTP_GET_VARS;
  if(strstr($url, $parameter_name.'=')){
   // Parameter is already present in the url. Replace it
    if ($oldvalue == "")
     $oldvalue=$HTTP_GET_VARS[$parameter_name];
   $newstr = ($newvalue == "") ? "" : $parameter_name.'='.$newvalue;
   $url = str_replace($parameter_name.'='.$oldvalue, $newstr, $url);
  }else{
   //Parameter is not present. Check if GET-Vars are attached at all
   if (strstr($url, '?')){
    //Get-Vars are attached
    $url = $url.'&'.$parameter_name.'='.$newvalue;
   }else{
    $url = $url.'?'.$parameter_name.'='.$newvalue;
   }
  }
  if (substr($url, -1) == "&")
   $url = substr($url, 0, strlen($url) - 1);
  return $url;
}
 
 function sql_level_string($path){
   if (is_trail($path)){
    $idxdif = 1;
    $cdif   = 2;
    $qdif   = 1;
   }else{
    $idxdif = 2;
    $cdif   = 3;
    $qdif   = 0;
   }
   $array = explode("/", $path);
   $endname = $array[count($array) - $idxdif];
   $str = "";
   for($x=0; ($x < count($array) - $cdif); $x++)
    $str .= "'$x',";
   $str .= "'$endname'";
   $test = "(FIELD('$endname', $str) -$qdif)";
   return $test;
 }
 
 
function short_url($url){
 if ($url == "http://")
  return "--no url--";
 $comps = parse_url($url);
 $res = $comps['scheme']."://".$comps['host'];
 if ($comps['port'] != '')
  $res = $res.":".$comps['port'];

 $res = $res.'/.../'.basename($comps['path']);
 if ($comps['query'] != '')
  $res = $res."?".$comps['query'];
 
 $res = str_replace("http//", "", $res);
 return $comps['host'];
} 
 
if (!function_exists("validate_email")){ 
function validate_email($email){
   return (ereg('^[-!#$%&\'*+\\./0-9=?A-Z^_`a-z{|}~]+'.
                '@'.
                '[-!#$%&\'*+\\/0-9=?A-Z^_`a-z{|}~]+\.'.
                '[-!#$%&\'*+\\./0-9=?A-Z^_`a-z{|}~]+$',
                $email));
 }
}
 
function randomString($len) {
   $possible="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
   $str="";
   while(strlen($str)<$len) {
      $str.=substr($possible,(rand()%(strlen($possible))),1);
   }
   return($str);
} 

function sql_date($hsh){
 $res = sprintf('%s-%s-%s %s:%s:%s', $hsh['year'], $hsh['month'], $hsh['day'], $hsh['hour'], $hsh['min'], $hsh['sec']);
 return $res;
}

function unixdate($hsh){
$res = mktime ($hsh['hour'], $hsh['min'], $hsh['sec'], $hsh['month'], $hsh['day'], $hsh['year']);
return $res;
}

function unix2friendly($timestamp){
  $tarray = localtime($timestamp, true);
  $hsh['year']  = sprintf("%02d", (1900+ $tarray['tm_year'])-2000);
  $hsh['month'] = sprintf("%02d", $tarray['tm_mon']+1);
  $hsh['day']   = sprintf("%02d", $tarray['tm_mday']);
  $hsh['hour']  = sprintf("%02d", $tarray['tm_hour']);
  $hsh['min']   = sprintf("%02d", $tarray['tm_min']);
  $hsh['sec']   = sprintf("%02d", $tarray['tm_sec']);
}

function friendlydate($mysqldate){
 
 //is the date a MySQL-Timestamp (ChangeDate)?
// die($mysqldate." | ".intval($mysqldate));
 if (!strstr($mysqldate, "-")){
  $hsh['year']  = substr($mysqldate, 0, 2);
  $hsh['month'] = substr($mysqldate, 2, 2);
  $hsh['day']   = substr($mysqldate, 4, 2);
  $hsh['hour']  = substr($mysqldate, 6, 2);
  $hsh['min']   = substr($mysqldate, 8, 2);
  $hsh['sec']   = substr($mysqldate, 10, 2);
 } else {
  $test  = strtotime($mysqldate);
  $tarray = localtime($test, true);
  $hsh['year']  = sprintf("%02d", (1900+ $tarray['tm_year'])-2000);
  $hsh['month'] = sprintf("%02d", $tarray['tm_mon']+1);
  $hsh['day']   = sprintf("%02d", $tarray['tm_mday']);
  $hsh['hour']  = sprintf("%02d", $tarray['tm_hour']);
  $hsh['min']   = sprintf("%02d", $tarray['tm_min']);
  $hsh['sec']   = sprintf("%02d", $tarray['tm_sec']);
 }
 return $hsh;
}

function lock_table($tablename){
 global $myDB;
 
 $query = FormatSQL(SQL_LOCK, array($tablename));
 $myDB->query($query);
 return true;
}

function unlock_table($tablename=""){
 global $myDB;

 $query = SQL_UNLOCK;
 $myDB->query($query);
 return true;
}

function build_good_url($path){
 if ($path == "/") return "/";
 if (is_trail($path)){
  $path=substr($path,0,strlen($path));
  $parts = explode("/", $path);
  $res = '';
  for ($x=1; $x < count($parts); $x++){
      $res = $res.'/'.rawurlencode($parts[$x]);
  }
 }else{
  $path=substr($path,1,strlen($path)-2);
  $parts = explode("/", $path);
  $res = '/';
  for ($x=0; $x < count($parts); $x++){
	  $res = $res.rawurlencode($parts[$x]).'/';
      //$old = $old.urlencode($parts[$x]).'/';
  } 
 } 
 return ($res);
}
 
function build_pathlist($path, $formated=false, $logout=false, $link_all=false, $mark_words=""){
 global $sess, $auth;
 $add=0;
 $path = trim($path);
 if (is_trail($path))
  $add = 1;
 $path=substr($path,0,(strlen($path)-1)+$add);
 $parts = explode("/", $path);
 
 if ($logout)
  $logstr="?dologout=1";
 else
  $logstr="";
 
 if ($formated){
  $url = $sess->url('/'.$logstr);
  $res = '<a href="'.$url.'">Home</a>';
  $old = "/";
  for ($x=1; $x < count($parts)-1; $x++){
      if ( ($parts[$x] == "Experts") or ($parts[$x] == "Help") or ( ($parts[$x] == "Settings") and ($auth->auth['uname'] == "")) )
	   $res = $res.' &gt; '.$parts[$x].'</a>';
	  else
	   $res = $res.' &gt; <a href="'.$sess->url($old.urlencode($parts[$x])).'/'.$logstr.'">'.hilight(str_replace("_", " ", $parts[$x]), $mark_words).'</a>';
      $old = $old.$parts[$x].'/';
  }
  if (!$link_all)
   $res = $res.' > <font color="'.CURR_COLOR.'">'.str_replace("_", " ", $parts[count($parts)-1]).'</font>';  
  else
   $res = $res.' &gt; <a href="'.$sess->url($path).'/">'.hilight(str_replace("_", " ", $parts[count($parts)-1]), $mark_words).'</a>';
 }else{ //unformated
  $res="Home".str_replace("/", " > ", str_replace("_", " ", $path));
 } 
 return($res);
}

function clean_html($str){
global $glob_allowed_tags;

foreach($glob_allowed_tags as $tag)
 $astr .= '<'.$tag.'>,';

$astr = substr ($astr, 0, strlen($astr)-1); 
return strip_tags($str, $astr);
} 
 
 function cleanup_string($str, $html=false, $br=false){
  $temp = addslashes($str);
  if ($br) $temp = nl2br($temp);
  if (!$html) $temp = clean_html($temp);
  $temp = trim($temp);
  return $temp;
 }
 
 /*
  functon is_trail($path: String): boolean;
  
  Will take a path to an element and return true if it's 
  a trail. If it's a node, it will return false.
  
*/

 function is_trail($path){
  if (strrpos($path, '/') == (strlen($path)-1))
   return false;
  else
   return true;
 }
 
 function parent_path($path){
  return( substr($path, 0, strrpos(substr($path,0, strlen($path)-1), '/') ).'/' ); 
 }
 /*
  functon level_count($path: String): integer;
  
  Will take a path to an element and return it's Level
  in the Directory in a form that can be used in a WHERE-clause
  
*/

 function level_count($path){
   return( count(explode("/", $path)) - 2);
 }

 /*
  function path2id($node: string, type: Integer): integer;
  
  Translates a Path of a   
*/
 
function path2id($path){
  global $myDB, $glob_language;
    
  $query = FormatSQL(SQL_PATH2ID, array(addslashes($path), $glob_language));
  //die("Friendly: $friendly; parent: $parent<p>$query"); 
  $myDB->query($query);
  if ($myDB->num_rows() <= 0) {
   return -1;
  }else{
   $myDB->next_record(); 
   return $myDB->f("Node_ID");
  }
}

function path2id_ex($path, $requested_language){
  global $myDB;
    
  $query = FormatSQL(SQL_PATH2ID, array(addslashes($path), $requested_language));
  $myDB->query($query);
  if ($myDB->num_rows() <= 0) {
   return -1;
  }else{
   $myDB->next_record(); 
   return $myDB->f("Node_ID");
  }
}


 /*
  function id2path($node: id): string;
  
  Translates a Path of a   
*/
 
function id2path($id){
  global $myDB;
  if (intval($id) != $id) return -1;
  $query = FormatSQL(SQL_ID2PATH, array($id));
  //die("Friendly: $friendly; parent: $parent<p>$query"); 
  $myDB->query($query);
  if ($myDB->num_rows() <= 0) {
   return -1;
  }else{
   $myDB->next_record(); 
   return $myDB->f("Name");
  }
}
/*
  function id2path($node: id): string;
  
  Translates a Path of a   
*/
 
function get_parent($id){
  global $myDB;
  
  $query = FormatSQL(SQL_GETPARENT, (array)$id); 
  $myDB->query($query);
  if ($myDB->num_rows() <= 0) {
   return -1;
  }else{
   $myDB->next_record(); 
   return $myDB->f("Parent");
  }
}

function friendlyname($path){
 if (is_trail($path))
 return(str_replace("_", " ",basename($path)));
 else{
  if ($path == "/")
   return ("Home");
  else
   return(str_replace("_", " ", basename( substr($path, 0, strlen($path)-1))));
 } 
}

function longwrap($text, $wrap_at = 45){
 $arr = explode(' ', $text);
 for($x=0; $x < count($arr); $x++){
  $y = 1;
//  echo("Test: ".$arr[$x]."<br>"); flush();
  if(strlen($arr[$x]) > $wrap_at)
   while (($y * $wrap_at) < strlen($arr[$x]) ){
//    printf("y: %d; y*wrap: %d; strlen: %d<br>", $y, ($y * $wrap_at), strlen($arr[$x])); flush; usleep(1000);
    $arr[$x] = substr_replace ($arr[$x], ' ', $y * $wrap_at, 0);
    $y++;
   }
 }
 return implode(' ', $arr);
}
 ?>