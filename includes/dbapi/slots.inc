<?php
 define("SLOTS_INC", true);

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
 
function get_slots($id){
 global $myDB;
 
 $query = FormatSQL(SQL_GET_SLOTS, (array)$id);

 $myDB->query($query);
 if ($myDB->num_rows() <= 0) {
  return -1;
 }else{
  $idx = 0;
  while ($myDB->next_record()){
   $hsh['id']           = $myDB->f("Slot_ID");
   $hsh['node']         = $myDB->f("Node_ID");
   $hsh['trail_1_id']   = $myDB->f("Trail_1_ID");
   $hsh['trail_1_text'] = $myDB->f("Trail_1_Text");
   $hsh['trail_1_path'] = $myDB->f("Path1");
   $hsh['trail_2_id']   = $myDB->f("Trail_2_ID");
   $hsh['trail_2_text'] = $myDB->f("Trail_2_Text");
   $hsh['trail_2_path'] = $myDB->f("Path2");
   $hsh['description']  = $myDB->f("Description");
   $hsh['islive']       = $myDB->f("IsLive");
   $hsh['next']         = $myDB->f("Next");
   $arr[$idx] = $hsh;
   $idx++;
  }
  reset($arr);
  return $arr; 
 }  
}

function get_slot_info($id){
 global $myDB;
 
 $query = FormatSQL(SQL_GET_SLOT_INFO, (array)$id);

 $myDB->query($query);
 if ($myDB->num_rows() <= 0) {
  return -1;
 }else{
  $myDB->next_record();
  $hsh['id']           = $myDB->f("Slot_ID");
  $hsh['node']         = $myDB->f("Node_ID");
  $hsh['trail_1_id']   = $myDB->f("Trail_1_ID");
  $hsh['trail_1_text'] = $myDB->f("Trail_1_Text");
  $hsh['trail_1_path'] = $myDB->f("Path1");
  $hsh['trail_2_id']   = $myDB->f("Trail_2_ID");
  $hsh['trail_2_text'] = $myDB->f("Trail_2_Text");
  $hsh['trail_2_path'] = $myDB->f("Path2");
  $hsh['description']  = $myDB->f("Description");
  $hsh['islive']       = $myDB->f("IsLive");
  $hsh['next']         = $myDB->f("Next");
  reset($hsh);
  return $hsh; 
 }  
}

function create_empty_slot($node_id){
  $hsh['id']           = "&lt;&lt;new&gt;&gt;";
  $hsh['node']         = $node_id;
  $hsh['trail_1_id']   = "";
  $hsh['trail_1_text'] = "";
  $hsh['trail_1_path'] = "";
  $hsh['trail_2_id']   = "";
  $hsh['trail_2_text'] = "";
  $hsh['trail_2_path'] = "";
  $hsh['description']  = "";
  $hsh['islive']       = "n";
  $hsh['next']         = "";
  reset($hsh);
  return $hsh; 
}

function rm_slot_ex($obj){
 global $myDB;

 $current_slots = get_slots((integer)$obj['node']);
 $object = new user_sorted_list($current_slots);
 $object->set_sql_str("UPDATE ltrSlots SET Next=%s WHERE Slot_ID=%s");
 $object->delete_element($obj['id']);
 $sql_strs = $object->get_sql_diff($current_slots);
// lock_table('ltrSlots');
 foreach($sql_strs as $sql)
  $myDB->query($sql);
 $query = FormatSQL(SQL_SLOT_DEL, array($obj['id']));
 $myDB->query($query);
// unlock_table('ltrSlots');
 return true;
}

function add_slot_ex($obj){
 global $myDB;

 if (is_array($obj))
  foreach($obj as $key => $val) $obj[$key] = cleanup_string($obj[$key]);
  
 $current_slots = get_slots($obj['node']);

 $object = new user_sorted_list($current_slots);
 $object->set_sql_str("UPDATE ltrSlots SET Next=%s WHERE Slot_ID=%s");
 
// lock_table('ltrSlots');
 $t2['id']   = ($obj['trail_2_id'] != '') ? $obj['trail_2_id'] : 'NULL';
 $t2['text'] = ($obj['trail_2_id'] != '') ? "'".$obj['trail_2_text']."'" : 'NULL';
 
 $query = FormatSQL(SQL_INSSLOT_EX, array($obj['node'], $obj['trail_1_id'], $obj['trail_1_text'],
                                          $t2['id'], $t2['text'], $obj['description'], $obj['islive']));
 $myDB->query($query);
// echo("<pre>$query</pre><br>");

 $id = mysql_insert_id();
 $obj['id'] = $id;
 $object->add_element($obj);
 $sql_strs = $object->get_sql_diff($current_slots);

 foreach($sql_strs as $sql){
  $myDB->query($sql);
 }
 //unlock_table('ltrslots');
 return $id;
}

function edit_slot_ex($id, $obj){
 global $myDB;

 if (is_array($obj))
  foreach($obj as $key => $val) $obj[$key] = cleanup_string($obj[$key]);


 $t2['id']   = ($obj['trail_2_id'] != '') ? $obj['trail_2_id'] : 'NULL';
 $t2['text'] = ($obj['trail_2_id'] != '') ? "'".$obj['trail_2_text']."'" : 'NULL';
 
 $query = FormatSQL(SQL_EDIT_SLOT_EX, array($obj['trail_1_id'], $obj['trail_1_text'],
                                          $t2['id'], $t2['text'], $obj['description'], $obj['islive'], $id));
                                          
 $myDB->query($query);

// update_dir_changedate($obj['trail']); 
 return true;
}

function reposition_slot($obj, $newpos){
 global $myDB;
// die($newpos);
// lock_table('ltrSlots WRITE, ltrDirectory as tbl1 READ, ltrDirectory as tbl2');
 $current_slots = get_slots((integer)$obj['node']);
 $linkid = $obj['id'];
 $object = new user_sorted_list($current_slots);
// foreach($object->arr as $element)
//  printf('<font color="teal"><tt>id: %s next: %s</tt></font><br>', $element['id'], $element['next']);
// printf("<hr>");
 $object->set_sql_str("UPDATE ltrSlots SET Next=%s WHERE Slot_ID=%s");
 $object->reposition($linkid, $newpos);
// foreach($object->arr as $element)
//  printf('<font color="red"><tt>id: %s next: %s</tt></font><br>', $element['id'], $element['next']);
// printf("<hr>");
 $sql_strs = $object->get_sql_diff($current_slots);
 foreach($sql_strs as $sql){
  $myDB->query($sql);
//  echo("<pre>$sql</pre><br>");
 }
// unlock_table();
// die("feritg repos");
}

?>
