<?php
 define("IWANTTO_INC", true);
 
 if (!class_exists("Template"))
  include("template.inc");
  
 if (!defined("LAY_DIRECTORY_INC"))
  include("layout/lay_directory.inc");

 if (!defined("COMMON_PERMISSIONS_INC"))
  include("commonapi/common_permissions.inc");
  
 global  $iw_action, $ltrstr, $nav_logged, $nav_unlogged, $auth;
 
 $iw_action['/^\/(.+)\/Messages/']         = 'print_actions_msgcenter';
 $iw_action['/^\/(.+)\/Settings/']         = 'print_actions_settings';
 $iw_action['/^\/Experts\/.*\/New_User$/'] = 'print_actions_register';
 $iw_action['/^\/Experts\/.*/']            = 'print_actions_mypage';
 $iw_action['/^\/Tour\/.*/']               = 'print_actions_tour';
 $iw_action['/^\/Experts\/.*/']            = 'print_actions_experts';
 $iw_action['/^\/About\/.*/']              = 'print_actions_about';
 $iw_action['/^\/.*/']                     = 'print_actions_directory';
 
 $nav_logged['/^\/(.+)\/Messages/']    = array('print_nav_messages_a', 'print_nav_messages_d', 3);
 $nav_logged['/^\/(.+)\/Settings/']    = array('print_nav_settings_a', 'print_nav_settings_d', 2);
 $nav_logged['/^\/$/']                          = array('print_nav_home_active', 'print_nav_home_inactive', 0);
 $nav_logged['/^\/Experts\/'.$auth->auth['uname'].'$/']   = array('print_nav_mypage_a', 'print_nav_mypage_d', 1);

//die('/^\/Experts\/'.$auth->auth['uname'].'$/');
 $nav_unlogged['/^\/$/']       =    array('print_nav_home_active', 'print_nav_home_inactive', 0);
 
 function print_nav_messages_a(){
  global $auth, $sess, $glob_userdata, $ltrstr;
  if (!defined("MESSAGES_INC"))
   include("dbapi/messages.inc");
  $count = count_new_messages($auth->auth['uid'], $glob_userdata['LastReadStamp']);
  return print_iwantto_item_ex($ltrstr['iw_readmsgs'], sprintf($ltrstr['iw_readmsgs_sub'], $count), $sess->url(build_good_url("/Experts/".$auth->auth['uname'].'/Messages/')));
 }
 
 function print_nav_messages_d(){
  global $auth, $sess, $glob_userdata, $ltrstr;
  return print_iwantto_item_ex($ltrstr['iw_readmsgs']);
 }
 
 function print_nav_home_inactive(){
  global $ltrstr;
  return print_iwantto_item_ex($ltrstr['Home']);
 } 
 
 function print_nav_mypage_a(){
  global $auth, $sess, $ltrstr;
  return print_iwantto_item_ex($ltrstr['Mypage'], "", $sess->url(build_good_url("/Experts/".$auth->auth['uname'])));
 }

 function print_nav_mypage_d(){
  global $auth, $sess, $ltrstr;
  return print_iwantto_item_ex($ltrstr['Mypage']);
 }
 
 function print_nav_home_active(){
  global $ltrstr, $sess, $ltrstr;
  return print_iwantto_item_ex($ltrstr['Home'], "", $sess->url("/"));
 } 

 function print_nav_settings_a(){
  global $auth, $sess, $ltrstr;
  return print_iwantto_item_ex($ltrstr['Settings'], "", $sess->url(build_good_url("/Experts/".$auth->auth['uname'].'/Settings')));
 }

 function print_nav_settings_d(){
  global $auth, $sess, $ltrstr;
  return print_iwantto_item_ex($ltrstr['Settings']);
 }


 function print_actions_msgcenter($kat){
  global $sess, $ltrstr, $auth, $PHP_SELF;
  $str = "";
  preg_match('/^\/(.+)\/Messages\/(.*)$/', $kat, $found);
  $expert = $found[1];
  $method = $found[2];
  if ($method == "") $method = "Inbox";
//  echo("<hr>Exp: $expert<br>Method: $method<hr>");
  $str .= ($method == "Inbox") ? print_iwantto_item_ex($ltrstr['Inbox']) : print_iwantto_item_ex($ltrstr['Inbox'], "", 'Inbox');
  $str .= ($method == "Outbox") ? print_iwantto_item_ex($ltrstr['Outbox']) : print_iwantto_item_ex($ltrstr['Outbox'], "", 'Outbox');
  $str .= ($method == "Composer") ? print_iwantto_item_ex($ltrstr['Composer']) : print_iwantto_item_ex($ltrstr['Composer'], "", 'Composer');
//  $str .= print_iwantto_item_ex($ltrstr['iw_register'], "", $sess->url('/Experts/New_User'));
  $str .= print_iwantto_item_ex(sprintf($ltrstr['iw_logout'], $auth->auth['uname']), "", $sess->url(build_good_url($PHP_SELF).'?dologout=1'));
  return $str;
 }
 
 function print_actions_experts($kat){
  global $auth, $sess, $PHP_SELF, $ltrstr;
  
  if(!defined("COMMON_PERMISSIONS_INC"))
   include("commonapi/common_permissions.inc");
  if(!defined("USER_INC"))
   include("dbapi/user.inc");
   
  preg_match('/^\/Experts\/(.+)$/', $kat, $found);
  $expert = $found[1];
  $method = $found[2];
  $str = "";
  $expert_data = get_user_from_name($expert);
  /* 
   What you can see below is reutilisation of existing code. The whole if-clause is 
   already-written code. I have ony changed the code under the existing interface
   and tried to reuse the currentely existing interface.
   
   BTW: I love it to code when I'm completely drunk as I am right now. I LOVE the
   italian food.
  */
  if ($expert_data != -1){
   if ( ($expert != $auth->auth['uname']) and ($auth->auth['uid'] != "nobody")){
    $res = is_friendship($expert_data['User_ID'], $auth->auth['uid']);
    if ($res === true){
     $str .= print_friendstuff($expert_data['Username'], $expert_data['User_ID'], "quit");
    }elseif ($res === false){
     $str .= print_friendstuff($expert_data['Username'], $expert_data['User_ID'], "make");
    }else{
     if ($res['friend_requesting'] == $expert_data['User_ID'] ){
      $str .= print_friendstuff($expert_data['Username'], $expert_data['User_ID'], "comfirm");
     }else{
      $str .= print_friendstuff($expert_data['Username'], $expert_data['User_ID'], "cancel");
     }
    }
   }
  } //expert_data != -1   
  if (($expert != $auth->auth['uname']) and ($expert != "New_User") ){
   $str .=  print_iwantto_item_ex(sprintf($ltrstr['Contact the author'], $expert), "", $sess->url(build_good_url('/Experts/'.$expert.'/Send_Message')));
  }
  
  $str .= print_iwantto_item_ex($ltrstr['iw_register'], "", $sess->url('/Experts/New_User'));
  $str .= (($auth->auth['uid'] != "nobody") and ($auth->auth['uname'] != "")) ? print_iwantto_item_ex(sprintf($ltrstr['iw_logout'], $auth->auth['uname']), "", $sess->url(build_good_url($PHP_SELF).'?dologout=1')) : print_iwantto_item_ex($ltrstr['iw_login'], "", $sess->url(build_good_url($PHP_SELF).'?dologin=1'));
  return $str;
 }
 
 function print_actions_settings($kat){
  global $sess, $ltrstr, $auth;
  $str = "";
  preg_match('/^\/(.+)\/Settings\/(.*)$/', $kat, $found);
  $expert = $found[1];
  $method = $found[2];
  if ($method == "") $method = "Profile";
//  echo("Exp: $expert<br>Method: $method");
  $str .= (($method == "Profile")) ? print_iwantto_item_ex($ltrstr['Profile']) : print_iwantto_item_ex($ltrstr['Profile'], "", 'Profile');
  $str .= ($method == "Password") ? print_iwantto_item_ex($ltrstr['Security Preferences']) : print_iwantto_item_ex($ltrstr['Security Preferences'], "", 'Password');
  $str .= ($method == "Preferences") ? print_iwantto_item_ex($ltrstr['Preferences']) : print_iwantto_item_ex($ltrstr['Preferences'], "", 'Preferences');
  $str .= print_iwantto_item_ex(sprintf($ltrstr['iw_logout'], $auth->auth['uname']), "", $sess->url(build_good_url($kat).'?dologout=1'));
  return $str;
 }

 function cmp ($a, $b) {   
   if ($a['pos'] == $b['pos']) return 0;
   return ($a['pos'] < $b['pos']) ? -1 : 1;
 }

  
 function print_navigation($kat){
  global $auth, $nav_unlogged, $nav_logged;
//  echo($kat);
  $str = "";
  $arr = Array();
  if (($auth->auth['uname'] == "")){
   foreach($nav_unlogged as $regexp => $function){
    if (preg_match($regexp, $kat)){
     eval('$arr[] = array("str" => '.$function[1].'($kat), "pos" => $function[2]);'); 
    }else{
     eval('$arr[] = array("str" => '.$function[0].'($kat), "pos" => $function[2]);'); 
    }
   }
  }else{
   foreach($nav_logged as $regexp => $function){
//   echo("blepp");
    if (preg_match($regexp, $kat)){
     eval('$arr[] = array("str" => '.$function[1].'($kat), "pos" => $function[2]);'); 
    }else{
     eval('$arr[] = array("str" => '.$function[0].'($kat), "pos" => $function[2]);'); 
    }
   }
  }
  uasort ($arr, cmp);
//  echo("count2: ".count($nav_logged));
  foreach($arr as $element)
   $str .= $element['str']."\n";
  $tpl = new Template(APPLICATION_HOME."/templates/iwantto/", "keep");
  $tpl->set_file(array("nav" => "navigation.html"));
  $tpl->set_var("ITEMS", $str);
  $tpl->parse("main", "nav");
  return $tpl->get("main"); 
 }

 function print_actions($kat){
  global $auth, $iw_action;
  $str = "";
  foreach($iw_action as $regexp => $function){
   if (preg_match($regexp, $kat)){
    eval('$str .= '.$function.'($kat);');
    break;
   }
  }
  $tpl = new Template(APPLICATION_HOME."/templates/iwantto/", "keep");
  $tpl->set_file(array("nav" => "actions.html"));
  $tpl->set_var("ITEMS", $str);
  $tpl->parse("main", "nav");
  return $tpl->get("main"); 
 }
 
 function print_actions_directory($kat){
  global $auth, $sess, $PATH_INFO, $ltrstr, $PHP_SELF, $perm, $movetrail;

  if(!defined("COMMON_PERMISSIONS_INC"))
   include("commonapi/permissions.inc");
   
  $capabilities = get_caps($perm, $kat); // this is used at many places

  $kat = (ereg_replace('^/([^-\]*-[^/]*)(.*)', '\\2', $PHP_SELF));

  $str = "";
  if ($auth->auth['uname'] == ""){
   if ($auth->auth['uid'] == "nobody")  $str .= print_iwantto_item_ex($ltrstr['iw_login'], "", $sess->url(build_good_url($kat).'?dologin=1'));
   $str .= print_iwantto_item_ex($ltrstr['iw_register'], "", $sess->url('/Experts/New_User'));
  }else{
   $str .= print_iwantto_item_ex(sprintf($ltrstr['iw_logout'], $auth->auth['uname']), "", $sess->url(build_good_url($kat).'?dologout=1'));
  }
  $str .= (($kat != '') and ($kat != '/') and (!substr('/About/', $kat) )) ? print_iwantto_item_ex($ltrstr['create a new trail'], "", $sess->url(build_good_url('/diredit/edittrail.php/'.$kat).'?action=addform'), true) : "";
//  die($kat);
// die($ltrstr['iw_editmode']);
  if (has_caps($capabilities, CAP_ADMIN_DIRECTORY))
   $str .= print_iwantto_item_ex($ltrstr['iw_editmode'], "", $sess->url(build_good_url($PHP_SELF).'?editmode=1'));
  
  if (has_caps($capabilities, CAP_ADD_TRAIL) and ($movetrail != "")){
   $name = id2path($movetrail);
   $str .= print_iwantto_item_ex(sprintf($ltrstr['iw_traildrop'], friendlyname($name)), "", build_good_url($PHP_SELF).'?drop=1');
  }
  return $str;
 }

 function print_iwantto_item_ex($text, $moretext="", $url="", $intrail=false){
  global $glob_userdata;
  
  $tpl = new Template(APPLICATION_HOME."/templates/iwantto/", "keep");
//  echo("Url: |$url|; Text: $text<br>");
  if ($url != ""){
//   echo("Url: |$url| ist nicht leer<br>");
   if ($intrail){
    $tpl->set_file(array("item" => "iwantto_action_item_trail.html"));
    $tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));
   }else{
    $tpl->set_file(array("item" => "iwantto_action_item.html"));
   }
//   echo("Setze: $url<p>");
   $tpl->set_var("URL", $url);
  }else{
//    echo("Url: |$url| ist leer<br>");
    $tpl->set_file(array("item" => "iwantto_inactive_item.html"));
  }
  $tpl->set_var("TEXT", $text);
  $tpl->set_var("MORETEXT", $moretext);
  $tpl->parse("main", "item");
  return $tpl->get("main"); 
 }

function print_friendstuff($target_user, $target_uid, $type){
 global $auth, $sess, $ltrstr;
// die($type);
 if ($type == "make")
  $str = print_iwantto_item_ex(sprintf($ltrstr['make_friendship'], $target_user), "", $sess->url(build_good_url("/Experts/$target_user/Make_friendship")));
 else{
  $str = print_iwantto_item_ex(sprintf($ltrstr["{$type}_friendship"], $target_user), "", $sess->url(sprintf("/diredit/dofriend.php?expert=%s&target=%s&action=%s", urlencode($auth->auth['uid']), urlencode($target_uid), $type)));
 }
 return $str; 
}
 
?>
