<?php
 define("TOPEX_INC", true);

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

 if (!defined("SQL_TOPEX_INC"))
  include("dbapi/sql_topex.inc");

 if (!defined("DEFEXP_INC"))
  include("dbapi/defexp.inc");

class top_experts{
 var $experts      = Array();
 var $featured     = Array();
 var $node_id      = 0;
 
 function top_experts($trails, $node_id){
  $this->read_top_experts($trails, $node_id);
 }
 
 function build_featured($username){
  global $myDB;
  
  $def = get_def_expert($this->node_id);
   if (!is_array($def)){
    $query = FormatSQL(GET_DESCRIPTION, array(addslashes($username)));
    $myDB->query($query);
    $myDB->next_record();
    $this->featured['Name']  = $username;
    $this->featured['About'] = strip_tags($myDB->f("Description"));  
   }else{
    $this->featured['Name']  = $def['Name']; //I am still not sure whether PHP makes a copy or a refernce
    $this->featured['About'] = $def['About']; 
   } 
 }

 function read_top_experts($trails, $node){
  global $myDB;
  
  $this->node_id = $node;
  $myexperts = Array();
  $in_str = "(";
  foreach($trails as $trail){
   $myexperts[$trail['username']] = $myexperts[$trail['username']]+1; 
   $in_str .= $trail['id'].', ';
  }
  asort($myexperts);
  $myexperts = array_reverse($myexperts);
  $in_str = substr($in_str, 0, strlen($in_str)-2).')';
  if ($in_str != ")"){
   $query = FormatSQL(GET_SUBSCRIBERSU, array($in_str));
   $myDB->query($query);
   while($myDB->next_record()){
    if (! ($myexperts[$myDB->f("Username")]) )
     $myexperts[$myDB->f("Username")] = -1;
   }
  } 

  $this->build_flat_array($myexperts);
 }
 
 function build_flat_array($ass_experts){
  foreach($ass_experts as $key => $value){
   $this->experts[] = $key;
   if ($value >=0)
    $best[] = $key;
  }
  
  srand ((double)microtime()*1000000);
  $index = (count($best) > 1) ? rand(1,count($best))-1 : 0;
  $this->build_featured($best[$index]); 
 }
 
 function get_experts_count(){
  return count($this->experts);
 }
 
 function get_experts_view($current_index, $length){
  return array_slice($this->experts, $current_index, $length);
 }
 
 function get_experts(){
  return $this->experts;
 }
 
 function get_featured(){
  return $this->featured;
 }
 
}