<?php
define("LAY_SLOTS_INC", true);

if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");
if (!defined("DIRECTORY_INC"))
 include("dbapi/directory.inc");
if (!defined("TRAILS_INC"))
 include("dbapi/trails.inc");
if (!defined("SLOTS_INC"))
 include("dbapi/slots.inc");

function print_slot_list($katid, $this_id){
 global $ltrstr;
 $slots = get_slots($katid);
 $this_printed = false;
 $str = "";
 if (is_array($slots)){
  $slots  = sort_link_array_ex($slots);
  foreach($slots as $slot){
   if ($slot['id'] == $this_id){
    $this_printed = true;
    $str .= "<option value=\"x\" selected>".$ltrstr['This slot']."</option>\n";
   }else{
    $str .= "<option value=\"".$slot['id']."\">Slot (".$slot['id'].")</option>";
   }
  }
 }
 if (!$this_printed)
  $str .= "<option value=\"x\" selected>".$ltrstr['This slot']."</option>\n";
return $str;
} 

function print_slot_edit_link($id){
 global $sess;
 
 $tpl = new Template(APPLICATION_HOME."/templates/directory/slots", "keep");
 $tpl->set_file(array("editlink" => "editlink.html"));
 $tpl->set_var("EDITURL", $sess->url("Slots/Edit_Slot?id=".$id));
 $tpl->parse("main", "editlink");
 return $tpl->get("main");
 }
 
function print_slot_form($nodeinfo, $slot_id=-1, $errstr=""){
 global $PHP_SELF;
 if (is_array($slot_id)){
  $myslot = $slot_id;
  $method = "Correct Slot";
  $hiddens = '<input type="hidden" name="id" value="'.$myslot['id'].'">';
 }elseif ($slot_id == -1){
  $myslot = create_empty_slot($nodeinfo['id']);
  $method = "Add Slot";
  $hiddens = "";
 }else{
  $myslot = get_slot_info($slot_id);
  $method = "Edit Slot";
  $hiddens = '<input type="hidden" name="id" value="'.$slot_id.'">';
 } 
 $tpl = new Template(APPLICATION_HOME."/templates/directory/slots", "keep");
 $tpl->set_file(array("form" => "addedit.html"));
 $tpl->set_var("ACTION", $PHP_SELF);
 $tpl->set_var("HIDDENS", $hiddens);
 $tpl->set_var("ERRORS", $errstr);
 $tpl->set_var("METHOD", $method);
 $tpl->set_var("SLOT_ID", $myslot['id']);
 $tpl->set_var("VALUE_TITLE_1", $myslot['trail_1_text']);
 $tpl->set_var("VALUE_TRAIL_1", $myslot['trail_1_path']);
 $tpl->set_var("VALUE_TITLE_2", $myslot['trail_2_text']);
 $tpl->set_var("VALUE_TRAIL_2", $myslot['trail_2_path']);
 $tpl->set_var("VALUE_DESCRIPTION", $myslot['description']);
 $tpl->set_var("SLOTLIST", print_slot_list($nodeinfo['id'], $myslot['id']));
 $tpl->set_var("BACKURL", build_good_url($nodeinfo['path']));
 $tpl->parse("main", "form");
 return $tpl->get("main");
} 

function print_slot_image($id, $url, $target){
 $file = "";
 $valid_exts=Array(".gif", ".jpg", ".png", "");
 foreach($valid_exts as $ext){
  $probe = APPLICATION_HOME.'/html/img/slots/'.$id.$ext;
  if (file_exists($probe)){
   $file = $id.$ext;
   break;
  }
 }
 if ($file == "") return "";
 $tpl = new Template(APPLICATION_HOME."/templates/directory/slots", "keep");
 $tpl->set_file(array("image" => "image.html"));
 $arr = getimagesize(APPLICATION_HOME.'/html/img/slots/'.$file);
 $tpl->set_var("LINKURL", $url);
 $tpl->set_var("TARGET", $target);
 $tpl->set_var("IMGURL", '/img/slots/'.$file);
 $tpl->set_var("IMGWIDTH", $arr[0]);
 $tpl->set_var("IMGHEIGHT", $arr[1]);
 $tpl->parse("main", "image");
 return $tpl->get("main");
  
}

 
function print_one_slot($links, $id, $description, $allow_edit=false){
 global $sess, $randval, $glob_userdata;
 if (!is_array($links)) return "";
 $rowspan = (count($links) == 2) ? 3 : 2;
 
 $tpl = new Template(APPLICATION_HOME."/templates/directory/slots", "keep");
 $tpl->set_file(array("slot" => "main.html"));
 $url    = $sess->url(build_good_url($links[0]['url']));
 $target = 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222);
 $tpl->set_var("LINKURL", $url);
 $tpl->set_var("TARGET", $target);
 $tpl->set_var("LINKTITLE", $links[0]['title']);
 $tpl->set_var("ROWSPAN", $rowspan);
 $tpl->set_var("IMAGE", print_slot_image($id, $url, $target));
 $tpl->set_var("DESCRIPTION", nl2br($description));
 if ($rowspan == 3)
  $tpl->set_var("SECONDLINK", print_2nd_link($links[1]));
 else
  $tpl->set_var("SECONDLINK", "");
  
 if ($allow_edit)
  $tpl->set_var("EDITSLOT", print_slot_edit_link($id));
 else
  $tpl->set_var("EDITSLOT", "");
  
 $tpl->parse("main", "slot");
 return $tpl->get("main");
}

function print_2nd_link($link){
 global $randval, $sess, $glob_userdata;
 
 $tpl = new Template(APPLICATION_HOME."/templates/directory/slots", "keep");
 $tpl->set_file(array("slot" => "2ndlink.html"));
 $tpl->set_var("LINKURL", $sess->url(build_good_url($link['url'])));
 $tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));
 $tpl->set_var("LINKTITLE", $link['title']);
 $tpl->parse("main", "slot");
 return $tpl->get("main");
}

function print_slots($slots, $editmode=false){
 $str = "";
 foreach($slots as $slot){
  $links = "";
  $links[0]['url']   = $slot['trail_1_path'];
  $links[0]['title'] = $slot['trail_1_text'];
  if ($slot['trail_2_path']){
    $links[1]['url']   = $slot['trail_2_path'];
    $links[1]['title'] = $slot['trail_2_text'];
  }
  $str .= print_one_slot($links, $slot['id'], $slot['description'], $editmode);
 }
 return $str;
}/**/
?>