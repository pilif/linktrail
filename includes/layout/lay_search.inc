<?php
define("LAY_SEARCH_INC", true);
/*
  lay_search.inc; 1.00
 */
 

if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");

function print_search_error($err){
 global $ltrstr;
 
 $tpl  = new Template(APPLICATION_HOME."/templates/search", "keep");
 $tpl->set_file(array("main" => "error.html")); 
// die($err['errcode']);
 $desc = ($ltrstr['Search_Error'][$err['errcode']]) ? $ltrstr['Search_Error'][$err['errcode']] : $ltrstr['Search_Error']['unknown'];
 $tpl->set_var("DESCRIPTION", $desc);
 $tpl->parse("main", "main");
 return $tpl->get("main");
}

function on_page($current, $page_count, $size){
 return ( ($current >= $page_count * $size) and ($current < $page_count * ($size+1) ) );
} 
 
function print_limit_element($type, $active, $str, $iddata, $url_marker, $anchor){
 global $REQUEST_URI, $ltrstr;
 $ret = "";
 if ($type != 2){
   $activestr = ($active) ? "active" : "inactive";
   $strs      = Array("prev", "browselist", "next");
   $tpl       = new Template(APPLICATION_HOME."/templates/search/limitbox", "keep");
   $filename  = $strs[$type-1]."_".$activestr;
   $new_index = ($type == 1) ? ($iddata['ci'] - $iddata['increment']) : ($iddata['ci'] + $iddata['increment']);
   if ($new_index < 0) $new_index = 0;
   if ($new_index >= ($iddata['max']) ) $new_index = $iddata['max'] - $iddata['increment'];
   $tpl->set_file(array("main" => "$filename.html")); 
   $tpl->set_var("TEXT", $ltrstr[$strs[$type-1]]." ".$str);
   $tpl->set_var("URL", alter_url($REQUEST_URI, $url_marker, $new_index, $iddata['ci']).$anchor);
   $tpl->parse("main", "main");
   $ret = $tpl->get("main");
 }else{
   $pagecount = ceil($iddata['max'] / $iddata['increment']);
   for ($x=0; $x < $pagecount; $x++){
    if ( ($iddata['ci'] >= $x * $iddata['increment']) and ($iddata['ci'] < ($x+1) * ($iddata['increment']) ) ) {
     $on_page = $x;
     break;
    }
   }
   
   $lbound = ($on_page - 9) >= 0 ? ($on_page - 5) : 0;
   $max    = ($lbound + 9) < $pagecount ? $lbound + 9 : $pagecount;
   
   for ($x=$lbound; $x < $max; $x++){
    $new_index = $x * $iddata['increment'];
    if ( ($iddata['ci'] >= $x * $iddata['increment']) and ($iddata['ci'] < ($x+1) * ($iddata['increment']) ) ) {
     $ret .= sprintf(' %d ', intval($new_index / $iddata['increment'])+1);
    }else{
     $ret .= sprintf(' <a href="%s" class="hi">%s</a> ', alter_url($REQUEST_URI, $url_marker, $new_index, $iddata['ci']).$anchor, intval($new_index / $iddata['increment'])+1);
    }
   }
   if ($max < $pagecount) $ret .= ' ..';
 }
 return $ret;
}

function limit_box_ex($ci, $display, $max, $element_description, $url_marker, $anchor=""){
 $tpl = new Template(APPLICATION_HOME."/templates/search/limitbox", "keep");
 $tpl->set_file(array("main" => "limitbox_common.html"));
 $endstr = ($ci+$display < $max) ? ($ci + $display) : $max;
 $tpl->set_var("STAT_START", $ci+1);
 $tpl->set_var("STAT_END", $endstr);
 $tpl->set_var("STAT_COUNT", $max);
 $tpl->set_var("WHAT", $element_description);
 $id_data['increment'] = $display;
 $id_data['ci']        = $ci;
 $id_data['max']       = $max;
 if ( ($ci <= 0) and ($ci + $display >= $max) ){
  $tpl->set_var("PREVLINK", print_limit_element(1, false, $element_description, $id_data, $url_marker, $anchor));
  $tpl->set_var("BROWSELINKS", print_limit_element(2, true, $element_description, $id_data, $url_marker, $anchor));
  $tpl->set_var("NEXTLINK", print_limit_element(3, false, $element_description, $id_data, $url_marker, $anchor));
 }elseif( ($ci <= 0) and ($ci + $display < $max) ){
  $tpl->set_var("PREVLINK", print_limit_element(1, false, $element_description, $id_data, $url_marker, $anchor));
  $tpl->set_var("BROWSELINKS", print_limit_element(2, true, $element_description, $id_data, $url_marker, $anchor));
  $tpl->set_var("NEXTLINK", print_limit_element(3, true, $element_description, $id_data, $url_marker, $anchor));
 }elseif( ($ci > 0) and ($ci + $display < $max) ){
  $tpl->set_var("PREVLINK", print_limit_element(1, true, $element_description, $id_data, $url_marker, $anchor));
  $tpl->set_var("BROWSELINKS", print_limit_element(2, true, $element_description, $id_data, $url_marker, $anchor));
  $tpl->set_var("NEXTLINK", print_limit_element(3, true, $element_description, $id_data, $url_marker, $anchor));
 }elseif( ($ci > 0) and ($ci + $display >= $max) ){
  $tpl->set_var("PREVLINK", print_limit_element(1, true, $element_description, $id_data, $url_marker, $anchor));
  $tpl->set_var("BROWSELINKS", print_limit_element(2, true, $element_description, $id_data, $url_marker, $anchor));
  $tpl->set_var("NEXTLINK", print_limit_element(3, false, $element_description, $id_data, $url_marker, $anchor));
 }

 $tpl->parse("main", "main");
 return $tpl->get("main"); 
}

function print_found_categories($cats){
global $glob_search_words, $glob_userdata;

if (count($cats) == 0) return "";

$tpl = new Template(APPLICATION_HOME."/templates/search", "keep");
$tpl->set_file(array("main" => "categories.html"));
$str = "<ul>";
foreach($cats['results'] as $kat)
 $str .= "<li>".build_pathlist($kat['name'], true, false, true, $glob_search_words);
$str .= '</ul>'; 
$tpl->set_var("CATEGORIES", $str); 
$tpl->set_var("CAT_START", $cats['resinfo']['reqested']+1); 
$tpl->set_var("CAT_END", $cats['resinfo']['reqested']+$cats['resinfo']['rescount']); 
$tpl->set_var("CAT_COUNT", $cats['resinfo']['max_resl']); 

if ($glob_userdata['ResPerPage'] < $cats['resinfo']['max_resl'])
 $tpl->set_var("LIMITBOX", limit_box_ex($cats['resinfo']['reqested'], $glob_userdata['ResPerPage'], $cats['resinfo']['max_resl'], "", 'cic')); 
else
 $tpl->set_var("LIMITBOX", "");
 
$tpl->parse("main", "main");
return $tpl->get("main"); 
}

function print_found_experts($experts){
global $sess;

if (count($experts) == 0) return "";

$tpl = new Template(APPLICATION_HOME."/templates/search", "keep");
$tpl->set_file(array("main" => "experts.html"));
foreach($experts['results'] as $exp)
 $str .= sprintf('<a href="%s">%s</a><br>'."\n", $sess->url('/Experts/'.rawurlencode($exp['name'])), $exp['name']);
$tpl->set_var("EXPERTLIST", $str); 

$tpl->parse("main", "main");
return $tpl->get("main"); 
}


function print_found_trails($trails){
 global $glob_userdata;
 
if (count($trails) == 0) return "";

$tpl = new Template(APPLICATION_HOME."/templates/search", "keep");
$tpl->set_file(array("main" => "trails.html"));
$str = "";
foreach($trails['results'] as $trail)
 $str .= print_one_found_trail($trail);
$tpl->set_var("TRAILS", $str);

$tpl->set_var("TRAIL_START", $trails['resinfo']['reqested']+1); 
$tpl->set_var("TRAIL_END", $trails['resinfo']['reqested']+$trails['resinfo']['rescount']); 
$tpl->set_var("TRAIL_COUNT", $trails['resinfo']['max_resl']); 

$tpl->set_var("LIMITBOX", limit_box_ex($trails['resinfo']['reqested'], $glob_userdata['ResPerPage'], $trails['resinfo']['max_resl'], "", 'cit', "#trails")); 
$tpl->parse("main", "main");
return $tpl->get("main"); 
}

function print_one_found_trail($traildata){
global $glob_search_words, $sess, $randval;

$tpl = new Template(APPLICATION_HOME."/templates/search", "keep");
$tpl->set_file(array("main" => "trail.html"));
$tpl->set_var("TRAILURL", $sess->url(build_good_url($traildata['name'])."?hilight=".base64_encode(implode("|",$glob_search_words)))); //pretty urls...
$tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));
$tpl->set_var("TRAILTITLE", hilight(friendlyname($traildata['name']), $glob_search_words));
$tpl->set_var("OWNERURL", $sess->url('/Experts/'.rawurlencode(trim($traildata['username']))));
$tpl->set_var("OWNERNAME", $traildata['username']);
$dh = friendlydate($traildata['date']);
$date = $dh['month'].'/'.$dh['day'].'/'.$dh['year'];
$tpl->set_var("ADDDATE", $date);
$tpl->set_var("NODEURL", $sess->url(parent_path($traildata['name']))); //pretty urls...
$tpl->set_var("NODENAME", friendlyname(parent_path($traildata['name'])));
$tpl->set_var("DESCRIPTION", hilight($traildata['description'], $glob_search_words)); 
$tpl->parse("main", "main");
return $tpl->get("main"); 
}

function print_found_users($users){
 global $glob_userdata;
if (count($users) == 0) return "";

$tpl = new Template(APPLICATION_HOME."/templates/search", "keep");
$tpl->set_file(array("main" => "users.html"));
$str = "";
foreach($users['results'] as $user)
 $str .= print_one_found_user($user);
$tpl->set_var("USERS", $str);
$tpl->set_var("USER_FROM", $users['resinfo']['reqested']+1); 
$tpl->set_var("USER_TO", $users['resinfo']['reqested']+$users['resinfo']['rescount']); 
$tpl->set_var("USER_COUNT", $users['resinfo']['max_resl']); 

if ($glob_userdata['ResPerPage'] < $users['resinfo']['max_resl'])
 $tpl->set_var("LIMITBOX", limit_box_ex($users['resinfo']['reqested'], $glob_userdata['ResPerPage'], $users['resinfo']['max_resl'], "", 'ciu')); 
else
 $tpl->set_var("LIMITBOX", "");

$tpl->parse("main", "main");
return $tpl->get("main"); 
}

function print_one_found_user($user){
global $sess;

$tpl = new Template(APPLICATION_HOME."/templates/search", "keep");
$tpl->set_file(array("user" => "user.html"));
$tpl->set_var("USERURL", $sess->url('/Experts/'.rawurlencode(trim($user['username']))));
$tpl->set_var("USERNAME", trim($user['username']));
$tpl->set_var("ABOUT", trim($user['about'])); 
$tpl->parse("main", "user");
return $tpl->get("main"); 
}

?>