<?php
 define("TRAILFUNCS_INC", true);
 
 class user_sorted_list{
 var $arr = Array();
 var $sql_str = "UPDATE ltrLinks SET Next=%s, ChangeDate = ChangeDate WHERE Link_ID=%s";
 
 function set_sql_str($str){
  $this->sql_str = $str;
 }
 
 function user_sorted_list($array = ""){
  $this->set_array($array);
 }
 
 function get_array(){
  return $this->arr;
 }
 
 function set_array($array){
  if (is_array($array)){
   $this->arr = $array;
   $this->sort_array($array);
  }
  else $this->arr = Array();
 }
 
 function add_element($element){
  if (count($this->arr) != 0)
   $this->arr[$this->get_index_from_field($this->arr, "", 'next')]['next'] = $element['id'];
  $element['next'] = "";
  $this->arr[] =($element);
 }
 
 function delete_element($id){
  $idx_vorg = $this->get_index_from_field($this->arr, $id, 'next');
  $idx_curr = $this->get_index_from_field($this->arr, $id, 'id');
  $idx_nach = $this->arr[$idx_curr]['next'];
  if ($idx_vorg != -1)
   $this->arr[$idx_vorg]['next'] = $idx_nach;
  array_splice($this->arr, $idx_curr, 1);
 }
 
 function sort_array(){
 if (count($this->arr) == 0) return;
 $fertig[] = $this->arr[$this->get_index_from_field($this->arr, "", 'next')];
 $continue = true;
 do{
   $arrr = $this->arr[$this->get_index_from_field($this->arr, $fertig[0]['id'], 'next')];
   if (is_array($arrr))
     array_splice($fertig, 0 , 0, array($arrr));
   else
     $continue = false;
 }while($continue);
 $this->arr = $fertig;
}

function get_index_from_field($array, $id, $fieldname='id'){
 $res = -1;
 for($x = 0; $x < count($array); $x++)
  if ($array[$x][$fieldname] == $id)
   $res = $x;
 return $res;  
}

function reposition($id, $position){
 $mypos = $this->get_index_from_field($this->arr, $id, 'id');

 if ($position == $mypos) return;
 $move_up = $position < $mypos;
 
 if ($move_up){
  $data['myid']    = $id;
  $data['newnach'] = $this->arr[$position]['id'];
  $data['oldvorg'] = $this->arr[$this->get_index_from_field($this->arr, $data['myid'], 'next')]['id'];
  $data['newvorg'] = $this->arr[$this->get_index_from_field($this->arr, $data['newnach'], 'next')]['id'];
  $data['oldnach']  = $this->arr[$mypos]['next'];

/*  printf("<p><tt>myid: %s<br>newnach: %s<br>oldvorg: %s<br>newvorg: %s<br>oldnach: %s (%d)</tt><p>",
         $data['myid'], $data['newnach'], $data['oldvorg'], $data['newvorg'], $data['oldnach'], $move_up); */
  
  if ($this->get_index_from_field($this->arr, $data['newvorg'], 'id') != -1)
   $this->arr[$this->get_index_from_field($this->arr, $data['newvorg'], 'id')]['next'] = $data['myid'];
  $this->arr[$mypos]['next'] = $data['newnach'];
  $this->arr[$this->get_index_from_field($this->arr, $data['oldvorg'], 'id')]['next'] = $data['oldnach'];
 }else{
  $data['myid']    = $id;
  $data['newnach'] = $this->arr[$position]['next'];
  $data['oldvorg'] = $this->arr[$this->get_index_from_field($this->arr, $data['myid'], 'next')]['id'];
  $data['newvorg'] = $this->arr[$this->get_index_from_field($this->arr, $data['newnach'], 'next')]['id'];
  $data['oldnach']  = $this->arr[$mypos]['next'];

/*  printf("<p><tt>myid: %s<br>newnach: %s<br>oldvorg: %s<br>newvorg: %s<br>oldnach: %s (%d)</tt><p>",
         $data['myid'], $data['newnach'], $data['oldvorg'], $data['newvorg'], $data['oldnach'], $move_up); */
  
  if ($this->get_index_from_field($this->arr, $data['oldvorg'], 'id') != -1)
   $this->arr[$this->get_index_from_field($this->arr, $data['oldvorg'], 'id')]['next'] = $data['oldnach'];
  $this->arr[$this->get_index_from_field($this->arr, $data['newvorg'], 'id')]['next'] = $data['myid'];
  $this->arr[$mypos]['next'] = $data['newnach'];
 }
 
 }

/*function reposition($id, $position){

 if ($position == 0){
  $data['myid']    = $id;
  $data['newnach'] = $this->arr[0]['id'];
  $data['oldvorg'] = $this->arr[$this->get_index_from_field($this->arr, $data['myid'], 'next')]['id'];
  $data['oldnach'] = $this->arr[$this->get_index_from_field($this->arr, $data['myid'], 'id')]['next'];
  $this->arr[$this->get_index_from_field($this->arr, $id, 'id')]['next'] = $data['newnach'];
  $this->arr[$this->get_index_from_field($this->arr, $data['myid'], 'next')]['next'] = $data['oldnach'];
 }elseif($this->arr[$position]['next'] == ""){
  $mypos = $this->get_index_from_field($this->arr, $id, 'id');
  $data['myid']    = $id;
  $data['newnach'] = $this->arr[$position]['next'];
  $data['oldvorg'] = $this->arr[$this->get_index_from_field($this->arr, $data['myid'], 'next')]['id'];
  $data['newvorg'] = $this->arr[$this->get_index_from_field($this->arr, $data['newnach'], 'next')]['id'];
  $data['mynext']  = $this->arr[$mypos]['next'];
  $this->arr[$this->get_index_from_field($this->arr, $data['myid'])]['next'] = $data['newnach'];
  $this->arr[$this->get_index_from_field($this->arr, $data['newvorg'])]['next'] = $data['myid'];
  if ($this->get_index_from_field($this->arr, $data['oldvorg']) != -1)
   $this->arr[$this->get_index_from_field($this->arr, $data['oldvorg'])]['next'] = $data['mynext'];
 }elseif(($mypos = $this->get_index_from_field($this->arr, $id, 'id')) == 0){
  $data['myid']    = $id;
  $data['newnach'] = $this->arr[$position]['next'];
  $data['oldvorg'] = "";
  $data['newvorg'] = $this->arr[$this->get_index_from_field($this->arr, $data['newnach'], 'next')]['id'];
  $data['oldnach'] = $this->arr[$this->get_index_from_field($this->arr, $id, 'id')]['next'];
  $data['name'] = $this->arr[$mypos]['title'];
 // printf("<p><tt>(%s) oldvorg: %s<br>myid: %s<br>oldnach: %s<br>newnach: %s<br>newvorg: %s</tt><p>", $data['name'], $data['oldvorg'], $data['myid'], $data['oldnach'], $data['newnach'], $data['newvorg']);
  $this->arr[$mypos]['next']    = $this->arr[$position]['next'];
  $this->arr[$position]['next'] = $data['myid'];
 }else{
  $data['myid']    = $id;
  $data['oldvorg'] = $this->arr[$this->get_index_from_field($this->arr, $id, 'next')]['id'];
  $data['oldnach'] = $this->arr[$this->get_index_from_field($this->arr, $id, 'id')]['next'];
  $data['newnach'] = $this->arr[$position+1]['id']; //die( $data['newnach'] );
  $data['newvorg'] = $this->arr[$this->get_index_from_field($this->arr, $data['newnach'], 'next')]['id'];
//  printf("<p><tt>oldvorg: %s<br>myid: %s<br>oldnach: %s<br>newnach: %s<br>newvorg: %s</tt><p>", $data['oldvorg'], $data['myid'], $data['oldnach'], $data['newnach'], $data['newvorg']);
  $this->arr[$this->get_index_from_field($this->arr, $data['oldvorg'], 'id')]['next'] = $data['oldnach'];
  $this->arr[$this->get_index_from_field($this->arr, $data['myid'],    'id')]['next'] = $data['newnach'];
  $this->arr[$this->get_index_from_field($this->arr, $data['newvorg'], 'id')]['next'] = $data['myid']; 
 }
}
*/

function get_sql_diff($array){
 $sql_strs = array();
 $this->sort_array($this->arr);
 for($x=0; $x < count($this->arr); $x++){
  for($y=0; $y < count($array); $y++){
   if ($this->arr[$x]['id'] == $array[$y]['id']){
    if($this->arr[$x]['next'] != $array[$y]['next']){
     $nextstr = ($this->arr[$x]['next'] == "") ? "NULL" : $this->arr[$x]['next'];
     $sql_strs[] = sprintf($this->sql_str, $nextstr, $this->arr[$x]['id']);
    } 
   }
  }
 }
 return $sql_strs;
}

} 

 
/*
  This function takes an array of hashes like the ones
  returned from get_links and sorts it regarding the
  $hsh['position']-Number. Non-tagged elements are sorted
  as they are appearing in the array. Same-tagged elements
  are sorted as they are appearing at the position of the tag.
*/
   
 function sort_link_array($links){
  if (!is_array($links)) return false;
  
 $fertig=Array(); //if all positions are set, the following loop will not create an array
                  //and array_splice will cry. So let's assure, we have an array...
 for ($x = 0; $x < count($links); $x++){
  if ( ($links[$x]['position'] === "") or (!isset($links[$x]['position'])) ){
   $fertig[] = $links[$x];
   }
  }
 
 for ($x = 0; $x < count($links); $x++){
  if (($links[$x]['position'] !== "") and (isset($links[$x]['position'])) ){
   array_splice($fertig, $links[$x]['position'], 0 , array($links[$x]));
  } 
 }

 return $fertig;
 }

function sort_link_array_ex($links){
 $obj = new user_sorted_list($links);
 return $obj->get_array();
}

?>
