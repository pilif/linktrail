<?php
define("LAY_TRAIL_INC", true);
/*
  lay_trail.inc; 1.00
  
  Some layout-tools about trails
  
  Written by Pilif and Jody
  
  Created:  Apr, 10th 2000
  Modified: Apr, 11th 2000
 
  © 2000 by Linktrail. All rights reserved  
 */
if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");
if (!defined("DIRECTORY_INC"))
 include("dbapi/directory.inc");
if (!defined("TRAILS_INC"))
 include("dbapi/trails.inc");
if (!defined("BUTTONS_INC"))
 include("layout/buttons.inc");
if (!defined("ACTIONS_INC"))
 include("layout/actions.inc");
if (!defined("TRAILFUNCS_INC"))
 include("commonapi/trailfuncs.inc");
if (!defined("COMMON_PERMISSIONS_INC"))
 include("commonapi/common_permissions.inc");
if (!defined("COMMON_SUBSCRIPTIONS_INC"))
 include("commonapi/common_subscriptions.inc");
if (!defined("PERMISSIONS_INC"))
 include("dbapi/permissions.inc");
if (!defined("SHARE_INC"))
 include("dbapi/share.inc");

function print_redirecting($url, $title){
  $tpl = new Template(APPLICATION_HOME."/templates/directory", "keep");
  $tpl->set_file(array("main" => "redirecting.html"));
  $tpl->set_var("SITEURL", $url);
  $tpl->set_var("SITENAME", $title);
  $tpl->parse("main", "main");
  return $tpl->get("main");  
}
  
 
function msg_box($title, $text, $current_trail, $backurl="", $reloadparent=0, $backtext=""){
  global $ltrstr, $sess;

  print script_fork($current_trail['path'], parent_path($current_trail['path']));
  flush();
  if ($backtext == "")
   $backtext = $ltrstr['Go Back'];
  if ($backtext == "-1")
   $backtext = "";

  $tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
  $tpl->set_file(array("main" => "msgbox.html"));
  $dh = friendlydate($current_trail['adddate']);
  $date = $dh['month'].'/'.$dh['day'].'/'.$dh['year'];
  
  if ($reloadparent == "1")
   $tpl->set_var("SCRIPT_RELOAD_PARENT", print_reload_parent_script());
  else
   $tpl->set_var("SCRIPT_RELOAD_PARENT", "");
   
  $tpl->set_var("TRAILNAME", str_replace("_", " ",friendlyname($current_trail['path'])));
  $tpl->set_var("TOWNERURL", $sess->url('/Experts/'.rawurlencode($current_trail['username'])));
  $tpl->set_var("TOWNER", $current_trail['username']);
  $tpl->set_var("TDATE", $date);
  $tpl->set_var("NODEURL", $sess->url(parent_path($current_trail['path'])));
  $tpl->set_var("NODE", friendlyname(parent_path($current_trail['path'])));
  $tpl->set_var("BACKURL", $sess->url(build_good_url($backurl)));
  $tpl->set_var("BACKTEXT", $backtext);
  $tpl->set_var("TITLE", $title);
  $tpl->set_var("TEXT", $text);
  $tpl->parse("main", "main");
  return $tpl->get("main");  
}

function ask_delete_trail($trailname, $action){
  global $sess;
  $tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
  $tpl->set_file(array("main" => "traildeltext.html"));
  $trl = get_node_info($trailname);
  $tpl->set_var("ACTION", $sess->url($action));
  $tpl->set_var("TRAILTITLE", friendlyname($trailname));
  $tpl->set_var("USER", $trl['username']);
  
  $tpl->parse("main", "main");
  $text = $tpl->get("main");
  
  return msg_box("Delete Trail...", $text, $trl);  
}

function print_permission_form($current_friend=12, $current_user=4){
 global $PHP_SELF;
 
 $tpl = new Template(APPLICATION_HOME."/templates/trail/edit", "keep");
 $tpl->set_file(array("main" => "edit_perms.html"));
 
 $tpl->set_var("ACTION", $PHP_SELF);

 $friend_checked['add'] = has_caps($current_friend, PERM_ADD)  ? "checked" : "";
 $friend_checked['del'] = has_caps($current_friend, PERM_DEL)  ? "checked" : "";
 $friend_checked['edt'] = has_caps($current_friend, PERM_EDIT) ? "checked" : "";
 $friend_checked['mov'] = has_caps($current_friend, PERM_MOVE) ? "checked" : "";

 $other_checked['add']  = has_caps($current_user, PERM_ADD)  ? "checked" : "";
 $other_checked['del']  = has_caps($current_user, PERM_DEL)  ? "checked" : "";
 $other_checked['edt']  = has_caps($current_user, PERM_EDIT) ? "checked" : "";
 $other_checked['mov']  = has_caps($current_user, PERM_MOVE) ? "checked" : "";
 
 $tpl->set_var(
  Array(
   "NAME_CHECK_EDIT_FRIEND" => "friend_edit",
   "CHECKED_EDIT_FRIEND"    => $friend_checked['edt'],
   "NAME_CHECK_DEL_FRIEND"  => "friend_del",
   "CHECKED_DEL_FRIEND"     => $friend_checked['del'],
   "NAME_CHECK_ADD_FRIEND"  => "friend_add",
   "CHECKED_ADD_FRIEND"     =>  $friend_checked['add'],
   "NAME_CHECK_MOVE_FRIEND" => "friend_move",
   "CHECKED_MOVE_FRIEND"    => $friend_checked['mov'],
   
   "NAME_CHECK_EDIT_OTHER"  => "other_edit",
   "CHECKED_EDIT_OTHER"     => $other_checked['edt'],
   "NAME_CHECK_DEL_OTHER"   => "other_del",
   "CHECKED_DEL_OTHER"      => $other_checked['del'],
   "NAME_CHECK_ADD_OTHER"   => "other_add",
   "CHECKED_ADD_OTHER"      => $other_checked['add'],
   "NAME_CHECK_MOVE_OTHER"  => "other_move",
   "CHECKED_MOVE_OTHER"     => $other_checked['mov']
  )
 );
 $tpl->parse("out", "main");
 return $tpl->get("out");
}

function print_share_friendlist($user, $uid, $trail_id){
global $sess;

$friends = get_friends($uid, FT_ALL);
if (!is_array($friends))
 return "";
get_allowed_users($trail_id, $friends);
if (count($friends) == 0)
 return "";
$tpl = new Template(APPLICATION_HOME."/templates/trail/share", "keep");
$tpl->set_file(array("main" => "friendlist.html"));
$tpl->set_block("main", "friend", "friends");
$x=0;
foreach($friends as $friend){
 $x++;
 $fname = $friend['username'];
 $tpl->set_var("FRIENDNAME", $fname);
 $tpl->set_var("UI", $x);
 $tpl->set_var("FRIENDID", $friend['userid'].'|'.$fname);
 $tpl->parse("friends", "friend", true);
}
$tpl->parse("out", "main");
return $tpl->get("out");
}

function print_share_nobody(){
 return implode("\n", file(TEMPLATE_ROOT.'trail/share/unlogged.html'));
}

function print_share_form($trail, $current_user){
 global $PHP_SELF, $auth;
 
 $tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
 $tpl->set_file(array("main" => "share_main.html"));
 $tpl->set_var("ACTION", build_good_url($PHP_SELF));
 $tpl->set_var('FRIENDLIST', print_share_friendlist($auth->auth['uname'], $auth->auth['uid'], $trail['id']));
 if ($current_user == ""){
  $tpl->set_var("UNLOGGED_FIELDS", print_share_nobody());
 }else{
  $tpl->set_var("UNLOGGED_FIELDS", "");
 }
  
 $tpl->parse("out", "main");
 return $tpl->get("out");
}


function print_subscription_form($current_notification, $formtype=SFT_SUBSCRIBE, $rlp=false){
 global $PHP_SELF, $auth, $sess;
 
//define("SFT_SUBSCRIBE", 1);
//define("SFT_CHSUBS", 2);
//define("SFT_CHNOT", 3);

 $tpl = new Template(APPLICATION_HOME."/templates/trail/edit", "keep");
 
 switch ($formtype) {
     case SFT_SUBSCRIBE:
		 $tpl->set_file(array("main" => "subscribe.html"));
         break;
     case SFT_CHSUBS:
          $tpl->set_file(array("main" => "change_subscription.html"));
         break;
 }

 $tpl->set_var("ACTION", build_good_url($PHP_SELF));
 $tpl->set_var("RLP", ($rlp ? "1" : "0"));
 $tpl->set_var("MSGCENTER", $sess->url('/Experts/'.rawurlencode($auth->auth['uname']).'/Messages'));
 $tpl->parse("out", "main");
 return $tpl->get("out");
}


function print_edit_element($type, $linkinfo, $permission, $postext="", $is_superuser=false){
 global $sess, $auth;
 $tpl = new Template(APPLICATION_HOME."/templates/trail/edit", "keep");
 $do_parse=false;
 switch ($type) {
     case 0:
         if ($linkinfo['title'] == "") $linkinfo['title'] = $linkinfo['url']; 
         if ( (has_caps($permission, PERM_EDIT)) or ($linkinfo['uid'] == $auth->auth['uid'])){
		  $tpl->set_file(array("main" => "perm_edit.html"));
          $tpl->set_var("LINKURL", $linkinfo['url']);
         }else{
		  $tpl->set_file(array("main" => "perm_noedit.html"));
          $tpl->set_var("LINKURL", short_url($linkinfo['url']));
         }
         $tpl->set_var("LINKTITLE", html_attribute($linkinfo['title']));
         $tpl->set_var("LDESCRIPTION", html_attribute($linkinfo['description']));         
		 $do_parse=true;
         break;
         
     case 1:
         if ( has_caps($permission, PERM_MOVE) ){
		   $tpl->set_file(array("main" => "perm_position.html"));
		   $tpl->set_var("LINKPOS", $postext);                  
		   $do_parse = true;
		 }else{
		   $do_parse = false;
		 }  
         break;

     case 2:
         if ( (has_caps($permission, PERM_DEL)) or (($linkinfo['uid'] == $auth->auth['uid'])) ){
          $tpl->set_file(array("main" => "perm_delete.html"));
          $tpl->set_var("DELBUTTON", get_button(0, "", "middle", "", 'onClick="if (confirm(\'You are deleting this link!\nProceed?\')) document.editform.submit(); else return false;"'));
		  $do_parse = true;
	     }else{
		   $do_parse = false;
		 }  
         break;
     case 3:
         if ( $is_superuser ){
          $tpl->set_file(array("main" => "perm_do_as_admin.html"));
		  $do_parse = true;
	     }else{
		   $do_parse = false;
		 }  
         break;
         
         
 }
 if ($do_parse){
  $tpl->parse("main", "main");
  return $tpl->get("main");
 }else
  return "";
}

function print_link_devider(){
 $tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
 $tpl->set_file(array("main" => "linkdevider.html"));
 $tpl->parse("main", "main");
 return $tpl->get("main"); 
}

function print_link($link, $editmode=false, $nodeinfo="", $hilight_words="", $superuser=false){
  global $PHP_SELF, $trailperms, $glob_userdata, $ltrstr, $sess, $auth;
  $tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
   
  if ($editmode){
   $tpl->set_file(array("main" => "linkedit.html"));
   $tpl->set_var("ACTION", build_good_url($PHP_SELF));
//   $tpl->set_var("ACTION", "/test.php");
   $tpl->set_var("HIDDENS", '<input type="hidden" name="action" value="edit"><input type="hidden" name="i" value="'.$link['id'].'">');
   $tpl->set_var("EDIT", print_edit_element(0, $link, $trailperms, $link));
  	
   $all_links = get_links($nodeinfo['path']);
   if (is_array($all_links)){
    $all_links = sort_link_array_ex($all_links);
	for ($x=0; $x < count($all_links); $x++){
     $one_link = $all_links[$x];
     if (strlen($one_link['title']) > 25){
      $one_link['title'] = substr($one_link['title'], 0, 22);
      $one_link['title'] .= '...';
     }
     if ($one_link['id'] == $link['id'])
      $postext .= '<option value="x" selected>&lt;&lt;&lt; this link &gt;&gt;&gt;';
     else
      $postext .= "<option value=\"{$one_link['id']}\">".$one_link['title']; 
    } //for
   }else
      $postext .= '<option value="x" selected>&lt;&lt;&lt; this link &gt;&gt;&gt;';
   $tpl->set_var("POSITION", print_edit_element(1, $link, $trailperms, $postext)); 
   $tpl->set_var("DELETE", print_edit_element(2, $link, $trailperms));
   $tpl->set_var("DO_AS_ADMIN", print_edit_element(3, $link, $trailperm, "", $superuser));
  }else{
   $dh = friendlydate($link['changedate']);
   $date = $dh['month'].'/'.$dh['day'].'/'.$dh['year'];
   $tpl->set_file(array("main" => "link.html"));
   $tpl->set_var("LINKURL", $sess->url('/GoTo/'.$link['id']));
   $tpl->set_var("LINKURL_FULL", $link['url']);
   if ($link['title'] == "") $link['title'] = short_url($link['url']); 
   $tpl->set_var("LINKTITLE", hilight($link['title'], $hilight_words));
   $tpl->set_var("LDESCRIPTION", hilight($link['description'], $hilight_words));
   $tpl->set_var("LDATE", $date);
   if ( (has_caps($trailperms, PERM_EDIT)) or ( has_caps($trailperms, PERM_EDIT) ) or (has_caps($trailperms, PERM_DEL) ) or ( has_caps($trailperms, PERM_MOVE) or ($link['uid'] == $auth->auth['uid']) ) ) 
    $buttons .= get_button(7,"/trailedit/editlink.php".build_good_url($nodeinfo['path'])."?i=".$link['id']."#".$link['id'], "right", "_self");
  }
  
   $tpl->set_var("LINKID", $link['id']);
   $tpl->set_var("LOWNERURL", $sess->url('/Experts/'.rawurlencode($link['username'])));
   $tpl->set_var("LOWNER", $link['username']);
   
  // die($link['adddate']);
   
   $cdate = unixdate(friendlydate($link['changedate']));
   $adate = unixdate(friendlydate($link['adddate']));
   $udate = unixdate(friendlydate($glob_userdata['LastOnline']));
   $str = "";
   if ($udate != -1){
    if ($cdate > $udate)
     $str = $ltrstr['Updated'];
    if ($adate >= $udate) //added has priority before changed
     $str = $ltrstr['New'];
    if ($str != "")
     $str = str_replace('[FLAG]', $str, DFLAG_FORMAT);
   }
   //die("adate: $adate; udate: $udate");
   $tpl->set_var("ISNEW", $str); 
   if ($buttons != "")
    $buttons .= " | ";
   $buttons .= get_button(6,"JavaScript: alert('Not available at this time. Please check back later.')", "right");
   $tpl->set_var("BUTTONS", $buttons); 

   $tpl->parse("main", "main");
   return $tpl->get("main");
} 

function print_trail_direct($links, $nodeinfo, $trailperms, $edit=-1, $predef="", $superuser=false){
global $reloadparent, $auth, $sess, $hilight_words;

$fulltrail = true;
if (!is_array($links) or (count($links) == 0)){
 $fulltrail = false;
}

print script_fork($nodeinfo['path'], parent_path($nodeinfo['path']));

include("trail/view/1.html");
if ($reloadparent == "1")
 print(print_reload_parent_script());
include("trail/view/2.html");
print(print_traildata($nodeinfo));
include("trail/view/3.html");
if ($fulltrail){
 include("trail/view/3a.html");
  print(build_action_list($nodeinfo, $trailperms, $auth->auth['uname']));
  include("trail/view/4.html");
  print(print_linkstitle($nodeinfo));
  include("trail/view/5.html");
  $links = sort_link_array_ex($links);
  $count = count($links);
  for ($idx = 0; $idx < $count; $idx++){
   $c = $links[$idx];
   print(print_link($c, ($edit == $c['id']), $nodeinfo, $hilight_words, $superuser));
   if ($idx < ($count -1)) 
    print(print_link_devider());
  }
  include("trail/view/6.html");
}
if (has_caps($trailperms, PERM_ADD)){
 print(print_addform('<input type="hidden" name="action" value="add">', $nodeinfo['path'], $predef));
}
include("trail/view/7.html");

}

function print_addform($hiddens, $trailpath, $predef=""){
  global $sess;

  $tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
  $tpl->set_file(array("main" => "addform.html"));
  $tpl->set_var("ACTIONURL", $sess->url("/trailedit/editlink.php".build_good_url($trailpath)));
  $tpl->set_var("HIDDENS", $hiddens);
  $tpl->set_var("TITLE", html_attribute($predef['title']));
  $tpl->set_var("URL", html_attribute($predef['url']));
  $tpl->set_var("DESCRIPTION", html_attribute($predef['description']));
  $tpl->set_var("ADDBUTTON", get_button(2));
  $tpl->parse("main", "main");
  return $tpl->get("main");  
}                                        

function print_position($url, $trail="", $link=""){
  $tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
  if ("" == $trail){
    $tpl->set_file(array("main" => "postext.html"));
    $tpl->set_var("URL", $url);
    $tpl->parse("main", "main");
    return $tpl->get("main");
  } else {
    $tpl->set_file(array("main" => "posform.html"));
    if ("" == $link)
     $text = "<<this link>>";
    else
     $text = $link;
    $links = get_links($trail);
     
  }
} 

function script_fork($path, $parent_path){
 global $glob_userdata, $sess, $randval;
 
 $tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
 $tpl->set_file(array("fork" => "script_fork.html"));
 $tpl->set_var("TRAILURL", $sess->url(addslashes($path)));
 $tpl->set_var("WINDOWNAME", "trail".($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));
 $tpl->set_var("NODEURL", $sess->url(addslashes($parent_path)));
 $ot = ($glob_userdata['PopupPos'] != "") ? $glob_userdata['PopupPos'] : "l";
 $tpl->set_var("OPEN_TYPE", $ot);
 $tpl->parse("main", "fork");
 $tpl->p("main");
}

function print_reload_parent_script(){
$tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
$tpl->set_file(array("reload" => "reload_parent.html"));
$tpl->parse("main", "reload");
$tpl->p("main");
} 
 
function print_traildata($nodeinfo){
 global $auth, $caps, $sess;
 
$tpl = new Template(APPLICATION_HOME."/templates/trail/view", "keep");
$tpl->set_file(array("view" => "traildata.html"));
if ( ($auth->auth['uname'] == $nodeinfo['username']) or (has_caps($caps, CAP_SUPERUSER)) ) 
 $tpl->set_var("TEDITBUTTON", '['.get_button(7, "/diredit/edittrail.php".build_good_url($nodeinfo['path']), $align="right", $target="_self").']');
else
 $tpl->set_var("TEDITBUTTON", "");
$tpl->set_var("TRAILNAME", make_short(str_replace("_", " ", friendlyname($nodeinfo['path']))));
$tpl->set_var("TOWNERURL", $sess->url('/Experts/'.rawurlencode($nodeinfo['username'])));
$tpl->set_var("TOWNER", $nodeinfo['username']);
$tpl->set_var("PARENTURL", $sess->url(parent_path($nodeinfo['path'])));
$tpl->set_var("NODE", friendlyname(parent_path($nodeinfo['path'])));
$tpl->set_var("NODEURL", $sess->url(parent_path($nodeinfo['path'])) );
$dh = friendlydate($nodeinfo['adddate']);
$date = $dh['month'].'/'.$dh['day'].'/'.$dh['year'];
$tpl->set_var("TADATE", $date);
$tpl->parse("main", "view");
return $tpl->get("main");
}

function print_linkstitle($nodeinfo){
 global $auth, $caps;
 
$tpl = new Template(APPLICATION_HOME."/templates/trail/view", "keep");
$tpl->set_file(array("view" => "linkstitlebar.html"));

$dh = friendlydate($nodeinfo['changedate']);
$date = $dh['month'].'/'.$dh['day'].'/'.$dh['year'];
$tpl->set_var("TCDATE", $date);

$tpl->parse("main", "view");
return $tpl->get("main");
}
 
/* 
 There are many windows with the same common trail-Layout. The
 following function takes a trail (a hash returned from get_node_info)
 and the content to print between the "framework". This will be
 placed at the position of the marker {LINK}
 
 If $add is false, no addlink-form will be displayed
*/

function print_trail_layout($nodeinfo, $content, $add=true, $auto=false){
global $reloadparent, $trailperms, $auth, $sess, $caps;



$tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
if ($content != "") 
 $tpl->set_file(array("view" => "view.html"));
else
 $tpl->set_file(array("view" => "view_newtrail.html"));

print script_fork($nodeinfo['path'], parent_path($nodeinfo['path']));
flush();
 
if ($reloadparent == "1")
 $tpl->set_var("SCRIPT_RELOAD_PARENT", print_reload_parent_script());
else
 $tpl->set_var("SCRIPT_RELOAD_PARENT", "");

/*if ($auto)
 $tpl->set_var("OPENPARENT", "true");
else
 $tpl->set_var("OPENPARENT", "false");*/

if ( ($auth->auth['uname'] == $nodeinfo['username']) or (has_caps($caps, CAP_SUPERUSER)) ) 
 $tpl->set_var("TEDITBUTTON", get_button(1, "/diredit/edittrail.php".build_good_url($nodeinfo['path']), $align="right", $target="_self"));
else
 $tpl->set_var("TEDITBUTTON", "");

$tpl->set_var("TRAILNAME", make_short(str_replace("_", " ", friendlyname($nodeinfo['path']))));
$tpl->set_var("TOWNERURL", $sess->url('/Experts/'.rawurlencode($nodeinfo['username'])));
$tpl->set_var("TOWNER", $nodeinfo['username']);
$tpl->set_var("PARENTURL", $sess->url(parent_path($nodeinfo['path'])));
$tpl->set_var("NODE", friendlyname(parent_path($nodeinfo['path'])));
$tpl->set_var("NODEURL", $sess->url(parent_path($nodeinfo['path'])) );
if ($content != "") 
 $tpl->set_var("OPTIONS", build_action_list($nodeinfo, $trailperms, $auth->auth['uname']));

$dh = friendlydate($nodeinfo['changedate']);
$date = $dh['month'].'/'.$dh['day'].'/'.$dh['year'];
$tpl->set_var("TCDATE", $date);

$dh = friendlydate($nodeinfo['adddate']);
$date = $dh['month'].'/'.$dh['day'].'/'.$dh['year'];
$tpl->set_var("TADATE", $date);


if ($content != "") 
 $tpl->set_var("LINK", $content);

if ($add)
 $tpl->set_var("ADDFORM", print_addform('<input type="hidden" name="action" value="add">', $nodeinfo['path']));
else
 $tpl->set_var("ADDFORM", "");

$tpl->set_var("FULLPATH", $sess->url($nodeinfo['path']));
 
$tpl->parse("main", "view");
$tpl->p("main");
}

function print_badlang_link($linkdata){
 global $PHP_SELF;
 
 $tpl = new Template(APPLICATION_HOME."/templates/trail/edit", "keep");
 $tpl->set_file(array("view" => "err_badlang_link.html"));
 $tpl->set_var("ACTION", build_good_url($PHP_SELF));
 $tpl->set_var("HIDDENS", '<input type="hidden" name="action" value="langanswer"><input type="hidden" name="linkdata" value="'.$linkdata.'">');
 $tpl->parse("main", "view");
 return $tpl->get("main");
}

function print_send_message_form($error){
 global $PHP_SELF, $ltrstr;
 
 $tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
 $tpl->set_file(array("view" => "send_message.html"));
 $tpl->set_var("ACTION", build_good_url($PHP_SELF));
 if ($error){
  $tpl->set_var("ERRORSTRING", $ltrstr['Subject missing'] );
  $tpl->set_var("WARN_SUBJECT", $ltrstr['Form Error Mark']);
 }else{
  $tpl->set_var("ERRORSTRING", "");
  $tpl->set_var("WARN_SUBJECT", "");
 }
 $tpl->parse("main", "view");
 return $tpl->get("main");
}
?>