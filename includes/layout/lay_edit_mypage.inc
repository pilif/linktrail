<?php
define("LAY_EDIT_MYPAGE_INC", true);

if (!defined("DIRECTORY_INC"))
 include("dbapi/directory.inc");
 
if (!defined("BUTTONS_INC"))
 include("layout/buttons.inc");

if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");

 if (!defined("LAY_DIRECTORY_INC"))
 include("layout/lay_directory.inc");

 
if (!defined("COMMON_PERMISSIONS_INC"))
 include("commonapi/common_permissions.inc"); 

if (!defined("COMMON_PERMISSIONS_INC"))
 include("commonapi/common_permissions.inc"); 
 
if (!defined("COMMON_USER_INC"))
 include("commonapi/common_user.inc"); 

/*
 DO NOT call this strange code unless
  
  a) you know, what it does
  b) you know how helpful this is
  c) you know a better solution for coding it
  
 This strange code that is using eval will prevent me from about 100 lines that
 are mostly identical.
*/ 
function print_profile_sub(&$tpl, $fieldname, $dbname, $userdata){
if ($dbname != ""){
  $tpl->set_var("FIELD_$fieldname", "field_".strtolower($fieldname));
  $tpl->set_var("VALUE_$fieldname", br2nl($userdata[$dbname]));
}  
$tpl->set_var("FIELD_{$fieldname}_SHOW", "radio_show_".strtolower($fieldname));
eval('$checked["friends"] = show_var($userdata["ShowFriend"], SHOW_'.$fieldname .')  ? "checked" : "";');
eval('$checked["nobody"]  = show_var($userdata["ShowNobody"], SHOW_'.$fieldname .')  ? "checked" : "";');
eval('$checked["anybody"] = show_var($userdata["ShowAnyone"],SHOW_'.$fieldname .')  ? "checked" : "";');
$tpl->set_var("FIELD_{$fieldname}_ANYONE_CHECKED", $checked['anybody']);
$tpl->set_var("FIELD_{$fieldname}_FRIENDS_CHECKED", $checked['friends']);
$tpl->set_var("FIELD_{$fieldname}_NOBODY_CHECKED", $checked['nobody']); 
} 
 

function fill_profile_template(&$userdata, &$tpl){
global $sess, $PHP_SELF, $ltrstr;
 
 $countries = Array("af","al","dz","as","ad","ao","ai","aq","ag","ar","am","aw","au","at","az","bs",
                    "bh","bd","bb","by","be","bz","bj","bm","bt","bo","ba","bw","bv","br","io","vg","bn",
                    "bg","bf","bi","kh","cm","ca","cv","ky","cf","td","cl","cn","cx","cc","co","km","cg",
                    "ck","cr","hr","cu","cy","cz","dk","dj","dm","do","tp","ec","eg","sv","gq","er","ee",
                    "et","fk","fo","fj","fi","fr","gf","pf","tf","ga","gm","ge","de","gh","gi","gr","gl",
                    "gd","gp","gu","gt","gn","gw","gy","ht","hm","hn","hk","hu","is","in","id","ir","iq",
                    "ie","il","it","ci","jm","jp","jo","kz","ke","ki","kp","kr","kw","kg","la","lv","lb",
                    "ls","lr","ly","li","lt","lu","mo","mk","mg","mw","my","mv","ml","mt","mh","mq","mr",
                    "mu","yt","mx","fm","md","mc","mn","ms","ma","mz","mm","na","nr","np","nl","an","nc",
                    "nz","ni","ne","ng","nu","nf","mp","no","om","pk","pw","pa","pg","py","pe","ph","pn",
                    "pl","pt","pr","qa","re","ro","ru","rw","gs","kn","lc","vc","ws","sm","st","sa","sn",
                    "sc","sl","sg","sk","si","so","za","es","lk","sh","pm","sd","sr","sj","sz","se","ch",
                    "sy","tw","tj","tz","th","tg","tk","to","tt","tn","tr","tm","tc","tv","um","ug","ua",
                    "ae","uk","us","uy","uz","vu","va","ve","vn","vi","wf","eh","ye","yu","zr","zm","zw");
 
$tpl->set_var("ACTION_PROF", build_good_url($PHP_SELF));

$tpl->set_var("COLOR1", BACK_COLOR1);
$tpl->set_var("COLOR2", BACK_COLOR2);
$tpl->set_var("COLOR3", BACK_COLOR3);
$tpl->set_var("USERNAME", $userdata['Username']);
$tpl->set_var("MYPAGEURL", $sess->url("/Experts/".$userdata['Username']));
$tpl->set_var("VALUE_ANYONE", "a");
$tpl->set_var("VALUE_FRIENDS", "f");
$tpl->set_var("VALUE_NOBODY", "n");
$tpl->set_var("BOUNCENOTE",  print_bounce($userdata['BounceFlag'], $location="profile"));
$tpl->set_var("FIELD_FIRST_NAME", "field_firstname");
$tpl->set_var("VALUE_FIRST_NAME", $userdata['FirstName']);
$tpl->set_var("FIELD_LAST_NAME", "field_lastname");
$tpl->set_var("VALUE_LAST_NAME", $userdata['LastName']);
print_profile_sub($tpl, 'NAME', "", $userdata);
print_profile_sub($tpl, 'EMAIL', "Email", $userdata);
$tpl->set_var("FIELD_GENDER", "field_gender");
$checked["male"]   = ($userdata['Gender'] == "m")  ? "selected" : "";
$checked["female"] = ($userdata['Gender'] == "f")  ? "selected" : "";
$checked["other"]  = ($userdata['Gender'] == "o")  ? "selected" : "";
$tpl->set_var("GENDER_MALE_SELECTED",   $checked['male']);
$tpl->set_var("GENDER_FEMALE_SELECTED", $checked['female']);
$tpl->set_var("GENDER_OTHER_SELECTED",  $checked['other']);
if ($userdata['Country'] == "")
 $tpl->set_var("FIELD_COUNTRY_EMPTY_SELECTED", "selected");
else
 $tpl->set_var("FIELD_COUNTRY_EMPTY_SELECTED", ""); 
for ($x=0; $x < count($countries); $x++){
 if ($countries[$x] == $userdata['Country']){
   $tpl->set_var("FIELD_COUNTRY_".strtoupper($countries[$x])."_SELECTED", " selected");  
 }else{
   $tpl->set_var("FIELD_COUNTRY_".strtoupper($countries[$x])."_SELECTED", "");   
 }
}
print_profile_sub($tpl, 'GENDER', "", $userdata);
print_profile_sub($tpl, 'AGE', "Age", $userdata);
print_profile_sub($tpl, 'ABOUT', "Description", $userdata);
print_profile_sub($tpl, 'COUNTRY', "Country", $userdata);
print_profile_sub($tpl, 'PIC', "PhotoUrl", $userdata);
}







function print_pwquery_error($text){
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/edit", "keep");
 $tpl->set_file(array("edit" => "error_pwchange.html"));
 $tpl->set_var("TEXT", $text);
 $tpl->parse("edit", "edit");
 return $tpl->get("edit");
}


function fill_password_template(&$userdata, &$tpl, $nomatch=false){
global $sess, $PHP_SELF, $ltrstr;
 
$tpl->set_var("ACTION_PASSWD", build_good_url($PHP_SELF));

$notes = ($nomatch)  ? sprintf('<font color="%s"><b>%s</b></font>: %s<p>%s', ERR_COLOR, $ltrstr['Error'], $ltrstr['Pass_Nomatch'], $ltrstr['Passnote']) : $ltrstr['Passnote'];

if ($notes != "")
 $tpl->set_var("NOTES_AND_ERROR", print_pwquery_error($notes));
else
 $tpl->set_var("NOTES_AND_ERROR", "");


$tpl->set_var("COLOR1", BACK_COLOR1);
$tpl->set_var("COLOR2", BACK_COLOR2);
$tpl->set_var("USERNAME", $userdata['Username']);
$tpl->set_var("MYPAGEURL", $sess->url("/Experts/".$userdata['Username']));

$tpl->set_var("FIELD_PASSWORD",  "field_password");
$tpl->set_var("FIELD_PASSWORD2", "field_password2");

}


function fill_preferences_template(&$userdata, &$tpl){
global $sess, $PHP_SELF, $ltrstr;
 
$tpl->set_var("USERNAME", $userdata['Username']);
$tpl->set_var("MYPAGEURL", $sess->url("/Experts/".$userdata['Username']));

$tpl->set_var("ACTION_PREFS", build_good_url($PHP_SELF));
$tpl->set_var("COLOR1", BACK_COLOR1);
$tpl->set_var("COLOR2", BACK_COLOR2);

$tpl->set_var("FIELD_POPUP_POS", "field_popup_pos");
unset($checked);
$checked["left"]   = ($userdata['PopupPos'] == "l")  ? "checked" : "";
$checked["right"]  = ($userdata['PopupPos'] == "r")  ? "checked" : "";
$tpl->set_var("FIELD_POPUP_POS_LEFT_CHECKED", $checked['left']);
$tpl->set_var("FIELD_POPUP_POS_RIGHT_CHECKED", $checked['right']);

unset($checked);
$checked   = ($userdata['NS6Sidebar'])  ? "checked" : "";
$tpl->set_var("FIELD_NSSIDE", "field_nsside");
$tpl->set_var("FIELD_NSSIDE_CHECKED", $checked);

unset($checked);
$checked   = ($userdata['NewWindow'])  ? "checked" : "";
$tpl->set_var("FIELD_NEWWINDOW", "field_newwindow");
$tpl->set_var("FIELD_NEWWINDOW_CHECKED", $checked);

unset($checked);
$checked   = ($userdata['MypageAfterLogon'])  ? "checked" : "";
$tpl->set_var("FIELD_MYPAGE_AFTER_LOGIN", "field_mypage_after_login");
$tpl->set_var("MYPAGE_AFTER_LOGIN_CHECKED", $checked);

$tpl->set_var("FIELD_COUNT_SEARCH_RESULTS", "field_count_search_results");
unset($checked);
$checked["c10"]  = ($userdata['ResPerPage'] == "l0")  ? "selected" : "";
$checked["c15"]  = ($userdata['ResPerPage'] == "15")  ? "selected" : "";
$checked["c20"]  = ($userdata['ResPerPage'] == "20")  ? "selected" : "";
$checked["c30"]  = ($userdata['ResPerPage'] == "30")  ? "selected" : "";
$checked["c50"]  = ($userdata['ResPerPage'] == "50")  ? "selected" : "";
$checked["c100"] = ($userdata['ResPerPage'] == "l00") ? "selected" : "";
$tpl->set_var("COUNT_SEARCH_RESULTS_10_SELECTED",  $checked['c10']);
$tpl->set_var("COUNT_SEARCH_RESULTS_15_SELECTED",  $checked['c15']);
$tpl->set_var("COUNT_SEARCH_RESULTS_20_SELECTED",  $checked['c20']);
$tpl->set_var("COUNT_SEARCH_RESULTS_30_SELECTED",  $checked['c30']);
$tpl->set_var("COUNT_SEARCH_RESULTS_50_SELECTED",  $checked['c50']);
$tpl->set_var("COUNT_SEARCH_RESULTS_100_SELECTED", $checked['c100']);

unset($checked);
$checked   = ($userdata['HighlightSearch'])  ? "checked" : "";
$tpl->set_var("FIELD_HIGHLIGHT_RESULTS", "field_highlight_results");
$tpl->set_var("HIGHLIGHT_RESULTS_CHECKED", $checked);

$tpl->set_var("FIELD_NOTIFICATION_METHOD", "field_notification_method");
unset($checked);
$checked["always"]  = ($userdata['NotificationStyle'] == "1")  ? "selected" : "";
$checked["weekly"]  = ($userdata['NotificationStyle'] == "2")  ? "selected" : "";
$checked["2weekl"]  = ($userdata['NotificationStyle'] == "3")  ? "selected" : "";
$checked["3weekl"]  = ($userdata['NotificationStyle'] == "4")  ? "selected" : "";
$checked["monthl"]  = ($userdata['NotificationStyle'] == "5")  ? "selected" : "";
$checked["2month"]  = ($userdata['NotificationStyle'] == "6")  ? "selected" : "";

$tpl->set_var("NOTIFICATION_METHOD_ALWAYS_SELECTED", $checked['always']);
$tpl->set_var("NOTIFICATION_METHOD_WEEKLY_SELECTED", $checked['weekly']);
$tpl->set_var("NOTIFICATION_METHOD_2WEEKL_SELECTED", $checked['2weekl']);
$tpl->set_var("NOTIFICATION_METHOD_3WEEKL_SELECTED", $checked['3weekl']);
$tpl->set_var("NOTIFICATION_METHOD_MONTHL_SELECTED", $checked['monthl']);
$tpl->set_var("NOTIFICATION_METHOD_2MONTH_SELECTED", $checked['2month']);

unset($checked);
$checked   = ($userdata['Vacation'])  ? "checked" : "";
$tpl->set_var("FIELD_VACATION_MODE", "field_vacation_mode");
$tpl->set_var("VACATION_MODE_CHECKED", $checked);
$tpl->set_var("BOUNCENOTE",  print_bounce($userdata['BounceFlag'], $location="settings"));
}


function print_msg_frame($counter, $str){
$tpl = new Template(APPLICATION_HOME."/templates/mypage/msc", "keep");
$tpl->set_file(array("msc" => "msg_frame.html"));
$color = ($counter % 2 == 0) ? BACK_COLOR1 : BACK_COLOR2;
$tpl->set_var("COLOR", $color);
$tpl->set_var("DATA", $str);
$tpl->parse("main", "msc");
return $tpl->get("main");
}

function print_bounce($bounce, $location="profile"){
 global $sess;
 if ($bounce == BOUNCE_OK) return "";
 
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/edit", "keep");

 if (($bounce == BOUNCE_ONCE) or ($bounce == BOUNCE_CHECK))
  $tpl->set_file(array("bw" => "warn_bounce_$location.html"));
 if ($bounce == BOUNCE_FAIL)
  $tpl->set_file(array("bw" => "fail_bounce_$location.html"));
 
 if ($location != "profile")
  $tpl->set_var("PROFILE_URL", 'Profile');
 
 $tpl->parse("main", "bw");
 return $tpl->get("main");
}

function print_msg_error_string($str){
$tpl = new Template(APPLICATION_HOME."/templates/mypage/msc", "keep");
$tpl->set_file(array("mses" => "msg_error.html"));

$tpl->set_var("ERRORSTR", $str);

$tpl->parse("main", "mses");
return $tpl->get("main");
}


function print_my_lms($userdata, $ci=0, $order_by="d", $error="", $expand_id="", $viewtype=1, $extension=""){ 
global $PHP_SELF, $sess, $ltrstr, $HTTP_GET_VARS;

if (!defined("MESSAGES_INC"))
 include("dbapi/messages.inc");

switch ($order_by) {
    case "o":
        $selected['sender']  = true;
        $selected['date']    = false;
        $bytext              = "Sender";
        break;
    case "d":
        $selected['sender']  = false;
        $selected['date']    = true;
        $bytext              = "Date";
        break;
    default:
        $selected['sender']  = false;
        $selected['date']    = true;
        $bytext              = "Date";
}
$list = (($extension == "Inbox") or ($extension == "Outbox"));
$tpl = new Template(APPLICATION_HOME."/templates/mypage", "keep");
if ($list){
 $inbox = ($extension == "Inbox");
 

 $tpl->set_file(array("msc" => "msg_center.html"));
 
 $objs = read_messages_ex(($inbox) ? $userdata['User_ID'] : $userdata['Username'], $inbox, $ci, $bytext);


 $str = "";
 if (($viewtype == 1) or ($viewtype == 2)){
  $colorx = 0;
  for($x=0; $x<count($objs['arr']); $x++){
   if (($objs['arr'][$x]->get_id() == $expand_id) and ($viewtype == 1)){
    $str .= $objs['arr'][$x]->print_extended(($colorx % 2 == 0));
    $colorx++;
   }elseif (($objs['arr'][$x]->get_id() == $expand_id) and ($viewtype == 2)){
    $str .= $objs['arr'][$x]->print_error(($colorx % 2 == 0));
    $colorx++;
   }
  }
 }
 for($x=0; $x<count($objs['arr']); $x++){
  if (($objs['arr'][$x]->get_id() == $expand_id) and ( ($viewtype == 1) or ($viewtype == 2)) ){
   //  nothing to do
  }else{
   $str .= $objs['arr'][$x]->print_simple(($colorx % 2 == 0));
   $colorx++;
  }
 }
 //die(count($objs));
 $tpl->set_var("MESSAGES", $str);
 $tpl->set_var("PAGENAME", $ltrstr[strtolower($extension)]);
 $tpl->set_var("CURRENT_INDEX", $ci);
 $tpl->set_var("SORTACTION", build_good_url($PHP_SELF));
 $tpl->set_var("TO_BY", ($inbox) ? "by" : "to");
 
 if(!defined("LAY_DIRECTORY_INC"))
  include("dbapi/lay_directory.inc");
  
 if ($objs['count'] > MSG_MAX_MSGS){
  $tpl->set_var("LIMITBOX", print_limitbox($ci, $objs['count'], "msg.", "", MSG_MAX_MSGS));
 }else{
  $tpl->set_var("LIMITBOX", "");
 }
 $str = "";
// echo($HTTP_GET_VARS['ctype']);
 if (isset($HTTP_GET_VARS['ctype']))
  $str = $ltrstr['lms comfirmation'][$HTTP_GET_VARS['ctype']].'<br>';
 $tpl->set_var("STATUS_LINE", $str.sprintf($ltrstr['Displaying messages'], (count($objs['arr'])) ));
 
 foreach($selected as $key => $value){
  $str = $value ? "selected" : "";
  $tpl->set_var("SELECTED_OB_".strtoupper($key), $str);
}
}else{
 $tpl->set_file(array("msc" => "msg_compose.html"));
 $friends = get_friends($userdata['User_ID'], FT_ALL);
// $str = '<option value="-1">(use textfield)'."\n";
 for ($x=0; $x < count($friends); $x++)
  $str .= '<option value="'.$friends[$x]['username'].'">'.$friends[$x]['username']."\n";
 $tpl->set_var("FRIENDLIST", $str);

 if ($error == ""){ 
  $tpl->set_var("ERRORSTR", "");
  $tpl->set_var("WARN_SUBJECT", "");
  $tpl->set_var("WARN_USERNAME", "");
  $tpl->set_var("PREDEF_SUBJECT", "");
  $tpl->set_var("PREDEF_MESSAGE", "");
  $tpl->set_var("PREDEF_USER", "");
 }elseif ($error == 999){
  $tpl->set_var("ERRORSTR", print_msg_error_string($ltrstr['Message sent']));
  $tpl->set_var("WARN_SUBJECT", "");
  $tpl->set_var("WARN_USERNAME", "");  
  $tpl->set_var("PREDEF_SUBJECT", "");
  $tpl->set_var("PREDEF_MESSAGE", "");
  $tpl->set_var("PREDEF_USER", "");
 }else{
  $tpl->set_var("ERRORSTR", print_msg_error_string($error['msg']));
  $tpl->set_var("WARN_SUBJECT",  ($error['subject'] )  ? $ltrstr['Form Error Mark'] : "" );
  $tpl->set_var("WARN_USERNAME", ($error['username'] ) ? $ltrstr['Form Error Mark'] : "" );
  $tpl->set_var("PREDEF_SUBJECT", html_attribute($error['predef_subject'], false));
  $tpl->set_var("PREDEF_MESSAGE", html_attribute($error['predef_message'], false));
  $tpl->set_var("PREDEF_USER", html_attribute($error['predef_user'], false));
 }
}

$tpl->set_var("USERNAME", $userdata['Username']);
$tpl->set_var("ACTION", build_good_url($PHP_SELF));
$tpl->set_var("MYPAGEURL", $sess->url(build_good_url("/Experts/".$userdata['Username'])));


$tpl->parse("main", "msc");
return $tpl->get("main");
}

function print_message_send_form($expert, $error=false){
 global $ltrstr, $sess, $PHP_SELF;
 
$tpl = new Template(APPLICATION_HOME."/templates/mypage", "keep");
$tpl->set_file(array("msc" => "send_message.html"));
$tpl->set_var("ACTION", build_good_url($PHP_SELF));
$tpl->set_var("USERNAME", $expert);
if ($error){
 $tpl->set_var("ERRORSTR", '<font face="verdana, arial, helvetica" size="2">'.$ltrstr['Subject missing'].'</font>');
 $tpl->set_var("WARN_SUBJECT", $ltrstr['Form Error Mark']);
}else{
 $tpl->set_var("ERRORSTR", "");
 $tpl->set_var("WARN_SUBJECT", "");
}
$tpl->set_var("USERMYPAGE", $sess->url(build_good_url("/Experts/".$expert)));
$tpl->parse("main", "msc");
return $tpl->get("main");
}

function print_make_friendship($expert){
 global $PHP_SELF, $sess;
 
$tpl = new Template(APPLICATION_HOME."/templates/mypage", "keep");
$tpl->set_file(array("msc" => "make_friendship.html"));
$tpl->set_var("USERNAME", $expert);
$tpl->set_var("ACTION", build_good_url($PHP_SELF));
$tpl->set_var("USERMYPAGE", $sess->url(build_good_url("/Experts/".$expert)));
$tpl->parse("main", "msc");
return $tpl->get("main");

}

function print_all_settings_form($ext, $userdata, $passerror, $posconf){
 global $glob_language_name;

 $tpl = new Template(APPLICATION_HOME."/templates/mypage/edit", "keep");

 if ($posconf){
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/edit", "keep");
 $tpl->set_file(array("settings" => "settings_bottom.html"));
 $tpl->parse("main", "settings");
 $str .= '<p>'. $tpl->get("main");
 }

/* $tpl->set_file(array("settings" => "settings_top.html"));
 $tpl->set_var("USERNAME", $userdata['Username']);
 $tpl->parse("main", "settings");
 $str = $tpl->get("main");*/
 $extl = strtolower($ext);
 if (file_exists(APPLICATION_HOME."/templates/$glob_language_name/mypage/edit/$extl.html")){
  $tpl = new Template(APPLICATION_HOME."/templates/mypage/edit", "keep");
  $tpl->set_file(array("settings" => "$extl.html"));
  eval("fill_".$extl.'_template($userdata, $tpl, $passerror);');
  $tpl->parse("main", "settings");
  $str .='<p>'.$tpl->get("main");
 }
/* if ($posconf){
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/edit", "keep");
 $tpl->set_file(array("settings" => "settings_bottom.html"));
 $tpl->parse("main", "settings");
 $str .= '<p>'. $tpl->get("main");
 }*/
 return $str;
}

function print_mypage_subnav($title, $items, $active_index){
 if (!is_array($items)) return "";
 if (!defined("LAY_TOUR_INC"))
  include("layout/lay_tour.inc");
  
 $tpl = new Template(APPLICATION_HOME."/templates/mypage/", "keep");
 $tpl->set_file(array("nav" => "edit_subnav.html"));
  $str = "";
  $tpl->set_var("TITLE", $title);
  for($x=0; $x < count($items); $x++)
   $str .= print_one_tour_link($x != $active_index, $items[$x]['title'], $items[$x]['url']);
 $tpl->set_var("NAVIGATION", $str); 
 $tpl->parse("main", "nav");
 return $tpl->get("main");
}
?>
