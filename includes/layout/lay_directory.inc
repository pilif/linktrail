<?php
define("LAY_DIRECTORY_INC", true);
/*
  edit_directory.inc; 1.00
  
  Contains DataBase-Functions used to build the directory-tree
  Documentation is inline and in directory.inc.html when ready
  
  Written by Pilif and Jody
  
  Created:  Apr, 9th 2000
 
  © 2000 by Linktrail. All rights reserved  
 */
 
if (!defined("DIRECTORY_INC"))
 include("dbapi/directory.inc");


if (!defined("TOPEX_INC"))
 include("dbapi/topex.inc");

if (!defined("SLOTS_INC"))
 include("dbapi/slots.inc");
 
if (!defined("BUTTONS_INC"))
 include("layout/buttons.inc");

if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");

 if (!defined("TRAILUP_INC"))
 include("dbapi/trailup.inc");

if (!defined("COMMON_PERMISSIONS_INC"))
 include("commonapi/common_permissions.inc"); 

if (!defined("TRAILFUNCS_INC"))
 include("commonapi/trailfuncs.inc"); 

if (!defined("LAY_SLOTS_INC"))
 include("layout/lay_slots.inc"); 
 
function print_announcebox($id, $boxonly=false){
 $str = ($boxonly) ? "boxes" : "onlyboxes";
 $tpl = new Template("/home/linktrai/templates/directory/$str", "keep");
 $tpl->set_file(array("box" => "$id.html"));
 $tpl->parse("main", "box");

 return $tpl->get("main");
}
 

function print_limit_nav($type, $active, $current_index, $max_trail, $objectname="trails", $urlextra=""){
 global $PHP_SELF, $REQUEST_URI, $sess;
 
 if ($objectname == "trails") $urlextra = "#trails";
 
 $tpl = new Template("/home/linktrai/templates/directory/limitbox", "keep");
 $str = ($active) ? "active" : "inactive";
  switch ($type) {
     case 0: //back
         $new_index = $current_index - DIR_MAX_TRAILS;
         if ($new_index <= 0)
          $new_index = "";
         $tpl->set_file(array("main" => "prev_$str.html"));
         if ($active)
          $tpl->set_var("URL", $sess->url(alter_url($REQUEST_URI, 'ci', $new_index)).$urlextra);
         break;
     case 1: //all
         $new_index = ($active) ? "all" : "0";
         $tpl->set_file(array("main" => "all_$str.html"));
         $tpl->set_var("URL", $sess->url(alter_url($REQUEST_URI, 'ci', $new_index)).$urlextra);
         if (!$active)
          $tpl->set_var("LIMIT", DIR_MAX_TRAILS);
         break;
     case 2: //next
         $new_index = $current_index + DIR_MAX_TRAILS;
         if ($new_index <= 0)
          $new_index = "";
         $tpl->set_file(array("main" => "next_$str.html"));
         if ($active)
          $tpl->set_var("URL", alter_url($REQUEST_URI, 'ci', $new_index).$urlextra);
         break;
 }
 $tpl->set_var("OBJECT", $objectname);
 $tpl->parse("main", "main");
 return $tpl->get("main");  
}

function print_limitbox($currentindex, $maxtrail, $objectname="trails", $urlextra=""){
 if ($currentindex != -1){
  $lotrail = $currentindex + 1;
  $hitrail = $currentindex+DIR_MAX_TRAILS;
  if ($hitrail > $maxtrail) {
   $hitrail = $maxtrail;
  }
  $value['all'] = print_limit_nav(1, true, $currentindex, $maxtrail, $objectname, $urlextra);
  $value['previous'] = print_limit_nav(0, ($currentindex >0), $currentindex, $maxtrail, $objectname, $urlextra);
  $value['next'] = print_limit_nav(2, ( ($currentindex + DIR_MAX_TRAILS) < $maxtrail), $currentindex, $maxtrail, $objectname, $urlextra);

 }else{
  $lotrail = 1;
  $hitrail = $maxtrail;
  $value['all']      = print_limit_nav(1, false, $currentindex, $maxtrail, $objectname, $urlextra);
  $value['previous'] = print_limit_nav(0, false, $currentindex, $maxtrail, $objectname, $urlextra);;
  $value['next']     = print_limit_nav(2, false, $currentindex, $maxtrail, $objectname, $urlextra);
 }
 $tpl = new Template("/home/linktrai/templates/directory", "keep");
 $tpl->set_file(array("main" => "limitbox.html"));
 $tpl->set_var("LOWTRAIL", $lotrail);
 $tpl->set_var("HITRAIL", $hitrail);
 $tpl->set_var("MAXTRAIL", $maxtrail);
 $tpl->set_var("PREVTRAILS", $value['previous']);
 $tpl->set_var("ALLTRAILS", $value['all']);
 $tpl->set_var("NEXTTRAILS", $value['next']);
 $tpl->set_var("OBJECT", $objectname);
 $tpl->parse("main", "main");
 return $tpl->get("main");  
}
 
function print_autotrail($trail){
 global $sess, $current_autotrail;
 
  $current_autotrail = $trail;
  $sess->register("current_autotrail");
  $ok = false;
 
  $tpl = new Template("/home/linktrai/templates/directory", "keep");
  $tpl->set_file(array("main" => "autotrail.html"));
  $tpl->set_var("TRAILURL", $sess->url($trail."?noopen=1"));
  $tpl->parse("main", "main");
  return $tpl->get("main"); 
} 
 
 //
function has_fitting($array, $startlen, $maxlen, $startindex, $test=""){
 $tmp = false;
 for($x=$startindex; $x<count($array); $x++){
  if( ($startlen + strlen(friendlyname($array[$x]['name']))) <= $maxlen) {return $x;}
 }
 return false;
}
 

function print_one_subnode($nodeobj, $may_click=true){
 global $sess;
 $tpl = new Template("/home/linktrai/templates/directory", "keep");
 if (is_array($nodeobj['subs']) and (count($nodeobj['subs']) > 0)){
 $ci = 0;
 $cc = 0;
 $tpl->set_file(array("main" => "one_subnode_subs.html"));
 $tpl->set_block("main", "subitem", "subitems");
 do{
  $res = has_fitting($nodeobj['subs'], $cc, CAT_UP_MAX_CHARS, $ci, $nodeobj['name']);
  if ($res !== false){
   $ci = $res;
  }
  
  $url   = ($nodeobj['subs'][$ci]['linkto'] == "") ? $nodeobj['subs'][$ci]['name'] : $nodeobj['subs'][$ci]['linkto'];
  $marker = ($nodeobj['subs'][$ci]['linkto'] == "") ? "" : "@";
  $tpl->set_var("SUBURL", $sess->url($url));
  $tpl->set_var("LINK_NOTE", $marker);
  $tpl->set_var("SUBNAME", friendlyname($nodeobj['subs'][$ci]['name']));
  $cc += strlen(friendlyname($nodeobj['subs'][$ci]['name']));
  $ci++;
  if (has_fitting($nodeobj['subs'], $cc, CAT_UP_MAX_CHARS, $ci, $nodeobj['name']))
   $fillstr = ", ";
  else{
   $fillstr = "...";
   $res = false;
  }
  $tpl->set_var("FILLER", $fillstr);
  $tpl->parse("subitems", "subitem", true); 
 }while($res !== false);
 
 }else
  $tpl->set_file(array("main" => "one_subnode.html"));
 if ($nodeobj['linkto'] == ""){
  $tpl->set_var("MAIN_URL", $sess->url($nodeobj['name']));
  $tpl->set_var("MAIN_NAME", friendlyname($nodeobj['name']));
  $tpl->set_var("LINK_NOTE", "");
 }else{
  $tpl->set_var("MAIN_URL", $nodeobj['linkto']);
  $tpl->set_var("MAIN_NAME", friendlyname($nodeobj['name']));
  $tpl->set_var("LINK_NOTE", "@");
 }
 $tpl->parse("main", "main");
 return $tpl->get("main"); 
}

 
function print_subnodes($kat, $subnodes, $caps){
 global $glob_language;
 
 if (!is_array($subnodes) or (count($subnodes) == 0) )
  return "";
 
 $filename = CACHE_DIR.'/directory/subnodes/'.base64_encode($kat).'.'.$glob_language.'.html';
 if (file_exists($filename))
  return implode("\n", file($filename));
 
 $tpl = new Template("/home/linktrai/templates/directory", "keep");
 $tpl->set_file(array("nodes" => "subnodes.html"));
 
 $tpl->set_var("THISNODE", friendlyname($kat)); 

 $count       = count($subnodes);
 $colcount    = SUBCOLCOUNT; //read from config.inc
 $entpcol     = intval($count / $colcount)-1;
 if ($count % 2 != 0)
  $entpcol++;
  
 $tplid=0;
 $x = 0;
 
 if (!is_array($subnodes)){
 for ($x = 0; $x < $colcount +1; $x++)
  $tpl->set_var("SUBNODES_$x", "&nbsp;"); 
  $tpl->parse("nodes", "nodes");
  $contents = $tpl->get("nodes");
  write_to_cache($filename, $contents);
  return $contents;
 }
 
 foreach($subnodes as $key => $value){
  if ( (friendlyname($value['']) == "Regional") and (!has_caps($caps, CAP_SUPERUSER)) )
   $str[$tplid] .= print_one_subnode($value, false);
  else
   $str[$tplid] .= print_one_subnode($value);
  $tplid = ($x < ($entpcol)) ? 0 : 1;
  $x++;
 } 
 
 for ($x = 0; $x < $colcount +1; $x++)
  if ($str[$x] != "")
   $tpl->set_var("SUBNODES_$x", $str[$x]); 
  else
   $tpl->set_var("SUBNODES_$x", "&nbsp;"); 


 $tpl->parse("nodes", "nodes");
 $contents = $tpl->get("nodes");
 //this will create a cache-File containing the subnodes-layout
 write_to_cache($filename, $contents);
 return $contents;
}

function print_trail_move_link($trail_id){
  global $REQUEST_URI, $HTTP_GET_VARS, $QUERY_STRING, $sess;
  $url = $REQUEST_URI;
  if ($QUERY_STRING != 'movetrail='.$HTTP_GET_VARS['movetrail'])
    $url .=  (strstr($url, "?")) ? "&" : "?";
  if (strstr($url, 'movetrail='))
   $url = str_replace('movetrail='.$HTTP_GET_VARS['movetrail'], 'movetrail='.$trail_id, $url);
  else
   $url .= 'movetrail='.$trail_id;
  $tpl = new Template("/home/linktrai/templates/directory/trails", "keep");
  $tpl->set_file(array("main" => "trailmove.html"));
  $tpl->set_var("MOVEURL", $sess->url($url));
  $tpl->parse("main", "main");
  return $tpl->get("main"); 
}

function print_climbed($path){
 global $sess;
 
 $tpl = new Template("/home/linktrai/templates/directory/trails", "keep");
 $tpl->set_file(array("sub" => "uptrail-sign.html"));
 $tpl->set_var("FROMURL", $sess->url(build_good_url($path)));
 $tpl->set_var("FROMNAME", friendlyname($path));
 $tpl->parse("sub", "sub");
 return $tpl->get("sub");     
}

function print_trail($data, $caps, $node_id){
     global $sess, $auth, $HTTP_GET_VARS, $movetrail, $glob_userdata, $ltrstr, $randval;

 $tpl = new Template("/home/linktrai/templates/directory/trails", "keep");
 $dh = friendlydate($data['changedate']);
 $date = $dh['month'].'/'.$dh['day'].'/'.$dh['year']; 
 $permmission = relevant_perms($data, $auth->auth['uid'], $caps);
 $tpl->set_file(array("trails" => "trailstd.html"));
 $tpl->set_var("TRAILURL", $sess->url(build_good_url($data['path'])));
 $tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));
 $tpl->set_var("TRAILTITLE", str_replace("_", " ", friendlyname($data['path'])));
 $tpl->set_var("POINTS", $data['points']);
 $tpl->set_var("DESCRIPTION", $data['description']);
 $tpl->set_var("OWNER", $data['username']);
 $tpl->set_var("OWNERURL", $sess->url('/Experts/'.rawurlencode($data['username'])));
 $cdate = unixdate(friendlydate($data['changedate']));
 $adate = unixdate(friendlydate($data['adddate']));
 $udate = unixdate(friendlydate($glob_userdata['LastOnline']));
 $str = "";
 if ($udate != -1){
  if ($cdate > $udate)
   $str = $ltrstr['Updated'];
  if ($adate > $udate) //added has priority before changed
   $str = $ltrstr['New'];
  if ($str != "")
   $str = str_replace('[FLAG]', $str, DFLAG_FORMAT);
 }
 $tpl->set_var("UPDATED", $str); 
 if ( (has_caps($permmission, PERM_MOVE_TRAIL)) and ($HTTP_GET_VARS['movetrail'] != $data['id']) and ($movetrail != $data['id']))
   $tpl->set_var("MOVE", print_trail_move_link($data['id']));
 else 
   $tpl->set_var("MOVE", "");
 $tpl->set_var("DATE", $date);
 $tpl->set_var("TRAILIMG", $test);
 $str = ($data['from'] != $node_id) ? print_climbed( parent_path($data['path']) ) : "";
 $tpl->set_var("UPSIGN", $str); 
 $tpl->parse("trails", "trails");

 return $tpl->get("trails");     
}

function print_add_slot($kat){
 global $sess;
 
 $tpl = new Template("/home/linktrai/templates/directory", "keep");
 $tpl->set_file(array("addslot" => "addslot.html"));
 $tpl->set_var("URL", $sess->url($kat.'Slots/Add_Slot'));
 $tpl->parse("addslot", "addslot");
 return $tpl->get("addslot");
}

function print_trailtitle($name){
 $tpl = new Template("/home/linktrai/templates/directory/trails", "keep");
 $tpl->set_file(array("title" => "trailtitle.html"));
 $tpl->set_var("THISNODE", $name); 
 $tpl->parse("title", "title");
 return $tpl->get("title");
}

function print_int_link($id){
 global $language_host;
 
 $tpl = new Template("/home/linktrai/templates/directory", "keep");
 $tpl->set_file(array("link" => "intlink.html"));
 $tpl->set_var("URL", 'http://'.$language_host[1].build_good_url(id2path($id)));
 $tpl->parse("link", "link");
 return $tpl->get("link");
}

function print_bottomline($print_icon, &$nodeinfo){
 $tpl = new Template("/home/linktrai/templates/directory/trails", "keep");
 $tpl->set_file(array("bottom" => "bottomline.html"));
 if ($nodeinfo['intnode'] != "")
  $tpl->set_var("INTNODE", print_int_link($nodeinfo['intnode']));
 else
  $tpl->set_var("INTNODE", "");  
 $tpl->set_var("ADDSLOT", ($print_icon) ? print_add_slot($nodeinfo['path']) : "");
 $tpl->parse("bottom", "bottom");
 return $tpl->get("bottom");
}

function print_trailgroup(&$nodeinfo, $caps=0, $current_index=0){
 global $sess, $top_experts;

 $kat = $nodeinfo['path'];
 $id  = $nodeinfo['id'];

// $tpl = new Template("/home/linktrai/templates/directory", "keep");
// $tpl->set_file(array("trails" => "trails.html"));
 include("directory/trails/1.html");
 print(print_trailtitle(friendlyname($kat)));
 include("directory/trails/2.html");
 
 $boxtype=0;
 $slots = get_slots($id);
 if (is_array($slots)){
  $slots  = sort_link_array_ex($slots);
  print(print_slots($slots, has_caps($caps, CAP_EDIT_SLOTS)));
  $boxtype= 1;
 }
 if (file_exists('/home/linktrai/templates/directory/onlyboxes/'.$id.'.html') ){
  print(print_announcebox($id));
  $boxtype=2;
 }
 if (file_exists('/home/linktrai/templates/directory/boxes/'.$id.'.html') ){
  print(print_announcebox($id));
  $boxtype=1;
 }
 include("directory/trails/3.html");
 
// $max_trail_count = get_trail_count($kat); //todo: port to the new system! // done.
 
 if (($boxtype==0) or ($boxtype==1)){
   $trails = read_trails($nodeinfo, $current_index);
   $max_trail_count = $trails['count'];
  $top_experts = new top_experts($trails['nodes'], $id);
  
  for($x=0; $x < count($trails['nodes']); $x++){
   print(print_trail($trails['nodes'][$x], $caps, $id));
  }
 }else
 print("&nbsp");
 include("directory/trails/4.html");

 print(print_bottomline(has_caps($caps, CAP_EDIT_SLOTS), $nodeinfo));
 
 //$addslot = (has_caps($caps, CAP_EDIT_SLOTS)) ? print_add_slot($kat) : "";
// $tpl->set_var("ADDSLOT", $addslot);
 include("directory/trails/5.html");
 
 if ($boxtype == 0)
  print(print_limitbox($current_index, $max_trail_count)); 
 include("directory/trails/6.html");
}

function print_kat_error($kat){
 $tpl = new Template("/home/linktrai/templates/directory", "keep");
 $tpl->set_file(array("error" => "err_nokat.html"));
 $tpl->set_var("KAT", build_pathlist($kat, false));
 $tpl->parse("main", "error");

 return $tpl->get("main");
}

function print_didyouknow($id){
 $count = 0;
 if(file_exists('/home/linktrai/templates/directory/didyouknow/'.$id)){
  $d = dir('/home/linktrai/templates/directory/didyouknow/'.$id);
  while($entry=$d->read()) {
    if(preg_match('/([0-9]+)\.html/', $entry, $idx)) {
     $files[$count] = $idx[1];
     $count++;
    }
  }
  $d->close();
 }else return "";
 if ($count == 0) return "";
 srand ((double)microtime()*1000000);
 $randval = ($count>1) ? rand(0, $count-1): 0;
 if (file_exists('/home/linktrai/templates/directory/didyouknow/'.$id.'/'.$files[$randval].'.html')){
  $tpl = new Template('/home/linktrai/templates/directory/', "keep");
  $tpl->set_file(array("featured" => 'didyouknow.html'));
  $tpl->set_var("TEXT", implode("\n", file('/home/linktrai/templates/directory/didyouknow/'.$id.'/'.$files[$randval].'.html')));
  $tpl->parse("featured", "featured");
  return $tpl->get("featured");
 }else
  return "";
}

function print_fexpertdata($exp){
 global $sess, $ltrstr;
 if ($exp['Name'] == "") return "";
 $tpl = new Template("/home/linktrai/templates/directory/featured", "keep");
 $tpl->set_file(array("featured" => "expert.html"));
 $tpl->set_var("EXPURL", $sess->url( build_good_url('/Experts/'.$exp['Name']) ));
 $tpl->set_var("EXPNAME", $exp['Name']);
 if ($exp['About'] == "")
  $exp['About'] = $ltrstr['No User Desc'];
 $tpl->set_var("ABOUT",  $exp['About']);
 $tpl->parse("featured", "featured");
 return $tpl->get("featured");
}

function print_fexpeditlink(){
 global $REQUEST_URI;
 
 $url = alter_url($REQUEST_URI, 'defexp', '1');
 
 $tpl = new Template("/home/linktrai/templates/directory/featured", "keep");
 $tpl->set_file(array("featured" => "editlink.html"));
 $tpl->set_var("EDITURL", $url);
 $tpl->parse("featured", "featured");
 return $tpl->get("featured");
}

function print_featured($kat, $caps=0){
 global $top_experts;
 
 if (! is_object($top_experts) ) return "";
 
 $exp = $top_experts->get_featured();
 if (($exp['Name'] != "") or (has_caps($caps, CAP_EDIT_EXPERTS))){
  $tpl = new Template("/home/linktrai/templates/directory", "keep");
  $tpl->set_file(array("featured" => "featured.html"));
  $tpl->set_var("FEATURED", print_fexpertdata($exp));
  if (has_caps($caps, CAP_EDIT_EXPERTS))
   $tpl->set_var("EDITLINK", print_fexpeditlink($exp));
  else
   $tpl->set_var("EDITLINK", "");

  $tpl->parse("featured", "featured");
  return $tpl->get("featured");
 }else
  return "";
}

function print_one_expert($name){
 global $sess;
 
 $tpl = new Template("/home/linktrai/templates/directory/topex", "keep");
 $tpl->set_file(array("featured" => "one_expert.html"));
 $tpl->set_var("EXPURL", $sess->url(build_good_url('/Experts/'.$name)));
 $tpl->set_var("EXPNAME", $name);
 $tpl->parse("featured", "featured");
 return $tpl->get("featured");
}

function print_topex($kat){
 global $top_experts;

 if (! is_object($top_experts) ) return "";
 
 $experts = $top_experts->get_experts_view(0, EXP_VIEW_MAX_TOPEX);
 $str = "";
 if (count($experts) > 0){
  for($x = 0; $x < count($experts); $x++){
   $str .= print_one_expert($experts[$x]);
  }
  $tpl = new Template("/home/linktrai/templates/directory", "keep");
  $tpl->set_file(array("featured" => "topex.html"));
  $tpl->set_var("TOPEX", $str);
  $tpl->parse("featured", "featured");
  return $tpl->get("featured");
 }else{
  return ""; 
 }
}

function print_iwantto_mypage(){
 global $auth, $sess;
 
 $tpl = new Template("/home/linktrai/templates/directory/iwantto", "keep");
 $tpl->set_file(array("main" => "iw_mypage.html"));
 $tpl->set_var("ACTIONURL", $sess->url("/Experts/".rawurlencode($auth->auth['uname'])));
 $tpl->parse("main", "main");
 return $tpl->get("main");

}

function print_iwantto_report_bug(){
 global $auth, $sess;
 
 $tpl = new Template("/home/linktrai/templates/directory/iwantto", "keep");
 $tpl->set_file(array("main" => "iw_bugreport.html"));
 $tpl->set_var("ACTIONURL", $sess->url("/Help/Report_a_bug"));
 $tpl->parse("main", "main");
 return $tpl->get("main");

}


function print_iwantto_readmsgs(){
 global $auth, $sess, $glob_userdata;
 
 if (!defined("MESSAGES_INC"))
  include("dbapi/messages.inc");
 $tpl = new Template("/home/linktrai/templates/directory/iwantto", "keep");
 $tpl->set_file(array("main" => "iw_readmsgs.html"));
 $tpl->set_var("ACTIONURL", $sess->url(build_good_url("/Experts/".$auth->auth['uname']."/Messages")));
 if ($glob_userdata['LastReadStamp'] == "")
  $glob_userdata['LastReadStamp'] == strtotime($glob_userdata['LastOnline']);
 $count = count_new_messages($auth->auth['uid'], $glob_userdata['LastReadStamp']);
 $tpl->set_var("NEWCOUNT", $count);
 $tpl->parse("main", "main");
 return $tpl->get("main");

}

function print_iwantto_subcategory(){
 global $REQUEST_URI, $editmode, $PHP_SELF;
 $tpl = new Template("/home/linktrai/templates/directory/iwantto", "keep");
 $tpl->set_file(array("main" => "subcategory.html"));
 if ($editmode)
  $url = $PHP_SELF;
 else
  $url = $PHP_SELF."?editmode=1";
 
/* if (strstr($REQUEST_URI, "?"))
  $url = $REQUEST_URI."&editmode=1";
 else
  $url = $REQUEST_URI."?editmode=1"; */
 $tpl->set_var("EDITURL", $url);  
 $tpl->parse("main", "main");
 return $tpl->get("main");

}

function print_iwantto_trailadd($kat){
 global $sess, $glob_userdata;
 $tpl = new Template("/home/linktrai/templates/directory/iwantto", "keep");
 $tpl->set_file(array("main" => "trailadd.html"));
 $tpl->set_var("ADDURL", $sess->url("/diredit/edittrail.php".build_good_url($kat)."?action=addform"));
 $tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));

 $tpl->parse("main", "main");
 return $tpl->get("main");

}

function print_iwantto_traildrop($trailid){
 global $sess, $PHP_SELF;
 $tpl = new Template("/home/linktrai/templates/directory/iwantto", "keep");
 $tpl->set_file(array("main" => "movetrail.html"));
 $test = id2path($trailid);
// die("Test: ".$test);
 $tpl->set_var("DROPURL", $PHP_SELF.'?drop=1');
 $tpl->set_var("TRALINAME", str_replace("_", " ", friendlyname($test)));
 $tpl->parse("main", "main");
 return $tpl->get("main");

}

function print_iwantto_catdrop($nodeid, $nodename){
 global $sess, $PHP_SELF;
 $tpl = new Template("/home/linktrai/templates/directory/iwantto", "keep");
 $tpl->set_file(array("main" => "movenode.html"));
 $tpl->set_var("DROPURL", $PHP_SELF.'?nodedrop=1');
 $tpl->set_var("NODENAME", str_replace("_", " ", friendlyname($nodename)));
 $tpl->parse("main", "main");
 return $tpl->get("main");

}

function print_iwantto_linkdrop($nodeid, $nodename){
 global $sess, $PHP_SELF;
 $tpl = new Template("/home/linktrai/templates/directory/iwantto", "keep");
 $tpl->set_file(array("main" => "linknode.html"));
 $tpl->set_var("DROPURL", $PHP_SELF.'?linkdrop=1');
 $tpl->set_var("NODENAME", str_replace("_", " ", friendlyname($nodename)));
 $tpl->parse("main", "main");
 return $tpl->get("main");
}


function print_iwantto_member(){
 global $sess;
 
 $tpl = new Template("/home/linktrai/templates/directory/iwantto", "keep");
 $tpl->set_file(array("main" => "nobody_member.html"));
 $tpl->set_var("ACTIONURL", $sess->url("/Experts/New_User"));
 $tpl->parse("main", "main");
 return $tpl->get("main");
}

function print_iwantto_login(){
 global $PHP_SELF;
 
 $tpl = new Template("/home/linktrai/templates/directory/iwantto", "keep");
 $tpl->set_file(array("main" => "nobody_login.html"));
 $tpl->set_var("ACTIONURL", $PHP_SELF."?dologin=1");
 $tpl->parse("main", "main");
 return $tpl->get("main");
}

function print_iwantto_logout(){
 global $PHP_SELF;
 
 $tpl = new Template("/home/linktrai/templates/directory/iwantto", "keep");
 $tpl->set_file(array("main" => "logout.html"));
 $tpl->set_var("ACTIONURL", $PHP_SELF."?dologout=1");
 $tpl->parse("main", "main");
 return $tpl->get("main");
}

function print_iwantto_tour(){
 global $sess;
 
 $tpl = new Template("/home/linktrai/templates/directory/iwantto", "keep");
 $tpl->set_file(array("main" => "nobody_tour.html"));
 $tpl->set_var("TOURURL", $sess->url("/Tour/"));
 $tpl->parse("main", "main");
 return $tpl->get("main");
}


function print_iwantto($kat, $capabilities){
 global $iwantto_order, $movetrail, $movenode, $linknode;
 
 if (has_caps($capabilities, CAP_ADD_TRAIL) and ($movetrail == ""))
  $capsgui['trailadd'] = print_iwantto_trailadd($kat);

 if (has_caps($capabilities, CAP_ADD_TRAIL) and ($movetrail != ""))
  $capsgui['trailadd'] = print_iwantto_traildrop($movetrail);

if (has_caps($capabilities, CAP_ADMIN_DIRECTORY) and ($movenode != "")){
  $nodename = id2path($movenode);
  if ($nodename != "-1"){
   if ( !strstr($kat, $nodename) )
    $capsgui['catdrop'] = print_iwantto_catdrop($movenode, $nodename);
  }  
}

if (has_caps($capabilities, CAP_ADMIN_DIRECTORY) and ($linknode != "")){
  $nodename = id2path($linknode);
  if ($nodename != "-1"){
    $capsgui['catdrop'] = print_iwantto_linkdrop($linknode, $nodename);
  }  
}


 if (has_caps($capabilities, CAP_VIEW_MYPAGE))
  $capsgui['mypage'] = print_iwantto_mypage();

 if (has_caps($capabilities, CAP_VIEW_MESSAGES))
   $capsgui['readmsg'] = print_iwantto_readmsgs();

 if (has_caps($capabilities, CAP_REGISTER))
   $capsgui['register'] = print_iwantto_member();
   
 if (has_caps($capabilities, CAP_LOGON))
   $capsgui['logon'] = print_iwantto_login();
   
 if (has_caps($capabilities, CAP_LOGOFF))
   $capsgui['logoff'] = print_iwantto_logout();
 
 if (has_caps($capabilities, CAP_DOTOUR))
   $capsgui['dotour'] = print_iwantto_tour();
  
 if (has_caps($capabilities, CAP_ADMIN_DIRECTORY))
   $capsgui['admin'] = print_iwantto_subcategory();
 
 if (has_caps($capabilities, CAP_REPORT_BUG))
   $capsgui['bugreport'] = print_iwantto_report_bug();  


 $tpl = new Template("/home/linktrai/templates/directory", "keep");
 $tpl->set_file(array("featured" => "iwantto.html"));
  
 $tpl->set_block("featured", "iwanttoitem", "iwanttos");
 foreach ($iwantto_order as $item) {
  if ( isset($capsgui[$item]) ){
   $tpl->set_var("ITEM", $capsgui[$item]);
   $tpl->parse("iwanttos", "iwanttoitem", true);
  } 
 }
 $tpl->parse("featured", "featured");

 return $tpl->get("featured");
}

function print_one_new_trail($nodeinfo){
 global $glob_userdata, $ltrstr, $sess, $randval;

 $tpl = new Template("/home/linktrai/templates/directory/newtrail", "keep");
 $tpl->set_file(array("featured" => "newtrail.html"));

 $cdate = unixdate(friendlydate($nodeinfo['changedate']));
 $adate = unixdate(friendlydate($nodeinfo['adddate']));
 $udate = unixdate(friendlydate($glob_userdata['LastOnline']));
 $str = "";
 if ($udate != -1){
  if ($cdate > $udate)
   $str = $ltrstr['Updated'];
  if ($adate > $udate) //added has priority before changed
   $str = $ltrstr['New'];
  if ($str != "")
    $str = str_replace('[FLAG]', $str, DFLAG_FORMAT);
 }
 $tpl->set_var("TRAILURL", $sess->url(build_good_url($nodeinfo['path'])));
 $tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));
 $tpl->set_var("TRAILNAME", make_short(str_replace ("_", " ", friendlyname($nodeinfo['path']))));
 $tpl->set_var("TRAILFLAG", $str);
 $tpl->parse("featured", "featured");
 return $tpl->get("featured");
}

function print_newtrails(&$nodeinfo){
 global $glob_userdata;
 $trails = -1;
 $trails = get_trails_by_date($nodeinfo['id'], friendlydate($glob_userdata['LastOnline']));
 if ($trails == -1)
  return "";
 
 $tpl = new Template("/home/linktrai/templates/directory", "keep");
 $tpl->set_file(array("featured" => "newtrails.html"));
 $trailstr = "";
 for ($x=0; $x < count($trails); $x++){
  $trailstr .= print_one_new_trail($trails[$x]);
 }
 $tpl->set_var("MORELINK", "");
 $tpl->set_var("NEWTRAILS", $trailstr);
 $tpl->parse("featured", "featured");
 return $tpl->get("featured");
}

function trail_addhead($path){
 $tpl = new Template("/home/linktrai/templates/directory/edit", "keep");
 $tpl->set_file(array("main" => "addhead.html"));
  $tpl->set_var("NODEURL", build_good_url($path));
  $tpl->set_var("NODENAME", friendlyname($path));
 $tpl->parse("main", "main");
 return $tpl->get("main");
}

function print_trail_addform($kat, $predef, $errfile=""){
 global $PHP_SELF, $ltrstr, $glob_language_name;
 
 if ((file_exists("/home/linktrai/templates/".$glob_language_name."/trail/edit/$errfile")) and ($errfile != "")){
  $str = implode("\n", file("/home/linktrai/templates/".$glob_language_name."/trail/edit/$errfile"));
 }else{
  $str = "";
 }
 $tpl = new Template("/home/linktrai/templates/directory/edit", "keep");
 $tpl->set_file(array("add" => "trailadd.html"));
 $tpl->set_var("ACTION", build_good_url($PHP_SELF));
 $tpl->set_var("ERRORSTR", $str);
 $tpl->set_var("HIDDENS", '<input type="hidden" name="action" value="doit"><input type="hidden" name="kat" value="'.$kat.'">');
 $tpl->set_var("TITLE", html_attribute($predef['title']));
 $tpl->set_var("DESCRIPTION", html_attribute($predef['description']));
 $tpl->set_var("DELETE", "");
 $tpl->set_var("HEADTEXT", trail_addhead($kat));
 $tpl->set_var("FOOTTEXT", "");
 $tpl->set_var("TRAIL_TITLE", $ltrstr['Add Trail title']);
 $tpl->set_var("TRAIL_DESCRIPTION", $ltrstr['Add Trail description']);
 $tpl->set_var("FOOTTEXT", implode("\n", file("/home/linktrai/templates/".$glob_language_name."/directory/edit/addfoot.html")));
 $tpl->set_var("DO_AS_ADMIN", "");
 $tpl->set_var("BUTTONS", '<input type="submit" name="create" value="'.$ltrstr['Create this Trail'].'">');
 $tpl->parse("add", "add");
 return $tpl->get("add");

} 

function print_trail_editform($trail, $predef, $is_superuser=false, $errfile=""){
 global $PHP_SELF, $ltrstr, $auth, $glob_language_name;
 
 if ((file_exists("/home/linktrai/templates/".$glob_language_name."/trail/edit/$errfile")) and ($errfile != "")){
  $str = implode("\n", file("/home/linktrai/templates/".$glob_language_name."/trail/edit/$errfile"));
 }else{
  $str = "";
 }

 $tpl = new Template("/home/linktrai/templates/directory/edit", "keep");
 $tpl->set_file(array("add" => "trailadd.html"));
 $tpl->set_var("ACTION", build_good_url($PHP_SELF));
 $tpl->set_var("ERRORSTR", $str);
 
 $title       = ($predef['title'])       ? $predef['title']       : html_attribute(friendlyname($trail['path']));
 $description = ($predef['description']) ? $predef['description'] : html_attribute($trail['description']);
 
 $tpl->set_var("HIDDENS", '<input type="hidden" name="action" value="doit">');
 $tpl->set_var("TITLE", $title);
 $tpl->set_var("DESCRIPTION", $description);
 $tpl->set_var("DELETE", print_trail_delform($trail['path']));
 $str = ($is_superuser) ? print_trail_adminform() : "";
 $tpl->set_var("DO_AS_ADMIN", $str);
 $tpl->set_var("BUTTONS", print_trail_editbuttons());
 $tpl->set_var("HEADTEXT", implode("\n", file("/home/linktrai/templates/".$glob_language_name."/directory/edit/edithead.html")));
 $tpl->set_var("FOOTTEXT", "");
 $tpl->set_var("TRAIL_TITLE", $ltrstr['Edit Trail title']);
 $tpl->set_var("TRAIL_DESCRIPTION", $ltrstr['Edit Trail description']);
 
 $tpl->parse("add", "add");
 return $tpl->get("add");

} 

function print_badlang($trailinfo){
 global $PHP_SELF;
 
 $tpl = new Template("/home/linktrai/templates/trail/edit", "keep");
 $tpl->set_file(array("view" => "err_badlang.html"));
 $tpl->set_var("ACTION", build_good_url($PHP_SELF));
 $tpl->set_var("HIDDENS", '<input type="hidden" name="action" value="langanswer"><input type="hidden" name="trailinfo" value="'.$trailinfo.'">');
 $tpl->parse("main", "view");
 return $tpl->get("main");
}


function print_trail_delform(){
 
 $tpl = new Template("/home/linktrai/templates/directory/edit", "keep");
 $tpl->set_file(array("del" => "traildel.html"));
// $tpl->set_var("DELBUTTON", get_button(0, "", "middle", "", 'onClick="if (confirm(\'You are deleting this trail and all the links it is contining!\nProceed?\')) document.editform.submit(); else return false;"') );
 $tpl->parse("del", "del");
 return $tpl->get("del");
}

function print_trail_adminform(){
 global $glob_language_name;
 
 return implode("\n", file("/home/linktrai/templates/$glob_language_name/directory/edit/do_admin.html"));
}


function print_trail_editbuttons($formname="editform"){
 $tpl = new Template("/home/linktrai/templates/directory/edit", "keep");
 $tpl->set_file(array("eb" => "editbuttons.html"));
 $tpl->set_var("FORMNAME", $formname);
 $tpl->parse("eb", "eb");
 return $tpl->get("eb");
 
}

?>