<?php
/*
 Message_clases contains the class hirearchy for the different types of 
 Messages linktrail sends to an user.
 
 Also definied here is the assoziazion between a message-object and the
 message-id from the database.
 
 The parent-object of all those classes is defined in dbapi/messages.inc
*/
 define("MESSAGE_CLASSES_INC", true);

 if(!class_exists("Template"))
  include("template.inc");
 
global $message_functions;
global $myDB;

 if (!is_object($myDB)) {
  $myDB = new DB_Linktrail;
  include("dbapi/sql_strs.inc");
  include("dbapi/sql_util.inc");
 }

define("AD_SUBSCRIBED", 1);
define("AD_UNSUBSCRIBED", 2);

 
$message_functions[LTMSG_TRAILDEL] = "admin_modification";
$message_functions[LTMSG_TRAILMOV] = "admin_modification";
$message_functions[LTMSG_TRAILCHG] = "admin_modification";
$message_functions[LTMSG_LINKADD]  = "user_modification";
$message_functions[LTMSG_LINKMOVD] = "user_modification";
$message_functions[LTMSG_LINKCHGD] = "user_modification";
$message_functions[LTMSG_LINKDELE] = "user_modification";
$message_functions[LTMSG_SUGGESTN] = "trail_suggestion";
$message_functions[LTMSG_USERTALK] = "lms_message";
$message_functions[LTMSG_USERTRNS] = "lms_message";
$message_functions[LTMSG_FSHIPREQ] = "friendship_request";
$message_functions[LTMSG_FSHIPNOT] = "std_user_message";
$message_functions[LTMSG_LTRANNCE] = "std_message";
$message_functions[LTMSG_POLLSTRT] = "poll_started";
$message_functions[LTMSG_POLLFINI] = "std_message";
$message_functions[LTMSG_TRAILSLT] = "std_message";
$message_functions[LTMSG_SLOTTERM] = "std_message";
$message_functions[LTMSG_FEELECT]  = "std_message";
$message_functions[LTMSG_FETERMN]  = "std_message";
$message_functions[LTMSG_REPORTSN] = "std_message";
$message_functions[LTMSG_SUBNOTI]  = "std_user_message";

class ltrStdMessageMsg extends ltrMessage{
  function ltrStdMessageMsg($dataset){
   $this->ltrMessage($dataset); //call parent constructor
  }
  
  function print_simple($val){
   global $ltrstr, $sess, $PHP_SELF, $glob_userdata;
   
   switch ($this->type) {
    case LTMSG_FETERMN:
        $path = id2path($this->data['Ext_Node_Id']);
        $str = sprintf($ltrstr['Message_FE_end'], $sess->url(build_good_url($path)), friendlyname($path));
        break;
    case LTMSG_FEELECT:
        $path = id2path($this->data['Ext_Node_Id']);
        $str = sprintf($ltrstr['Message_FE_start'], $sess->url(build_good_url($path)), friendlyname($path));
        break;
    case LTMSG_SLOTTERM:
        $path = id2path($this->data['Ext_Node_Id']);
        $node_path = id2path($this->data['Node_ID']);
        $target = 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222);
        $str = sprintf($ltrstr['Message_slot_end'], $sess->url(build_good_url($path)), $target, $target, friendlyname($path), $sess->url(build_good_url($node_path)), friendlyname($node_path));
        break;
    case LTMSG_TRAILSLT:
        $path = id2path($this->data['Ext_Node_Id']);
        $node_path = id2path($this->data['Node_ID']);
        $target = 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222);
        $str = sprintf($ltrstr['Message_slot_start'], $sess->url(build_good_url($path)), $target, $target, friendlyname($path), $sess->url(build_good_url($node_path)), friendlyname($node_path));
        break;
   }
   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);

   $tpl = new Template(APPLICATION_HOME."/templates/messages/short", "keep");
   $tpl->set_file(array("simple" => "general_notifications.html"));
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $tpl->set_var("TEXT", $str);
   $tpl->set_var("DELURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=delete&viewtype=0");
   $tpl->set_var("READURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&viewtype=1");
   $tpl->set_var("DATE", $d);
   $tpl->set_var("MYID", $this->id);
   $tpl->parse("main", "simple");
   return $tpl->get("main");
  }

  function user_interaction(&$http_vars){
   if ($http_vars['action'] == "delete")
    $this->self_destruct();
  }
}






class ltrStdMessageUserMsg extends ltrMessage{
  function ltrStdMessageMsg($dataset){
   $this->ltrMessage($dataset); //call parent constructor
  }
  
  function print_simple($val){
   global $ltrstr, $sess, $PHP_SELF, $glob_userdata;
   
   if ($this->type == LTMSG_FSHIPNOT){
       switch ($this->data['Type']) {
        case "accepted":
            $str = $ltrstr['accepted friendship'];
            break;
        case "term":
            $str = $ltrstr['terminated friendship'];
            break;
        case "ignore":
            $str = $ltrstr['ignored friendship'];
            break;
        case "cancel":
            $str = $ltrstr['canceled friendship'];
            break;
        case "accepted_self":
            $str = sprintf($ltrstr['accepted self'], $sess->url(build_good_url("/Experts/".$this->data['ExtraName'])), $this->data['ExtraName']);
            break;
        case "term_self":
            $str = sprintf($ltrstr['terminated self'], $sess->url(build_good_url("/Experts/".$this->data['ExtraName'])), $this->data['ExtraName']);
            break;
        case "ignore_self":
            $str = sprintf($ltrstr['ignored self'], $sess->url(build_good_url("/Experts/".$this->data['ExtraName'])), $this->data['ExtraName']);
            break;
        case "cancel_self":
            $str = sprintf($ltrstr['canceled self'], $sess->url(build_good_url("/Experts/".$this->data['ExtraName'])), $this->data['ExtraName']);
            break;
       }
   }elseif($this->type == LTMSG_SUBNOTI){
       $path = id2path($this->data['Trail']);
       $target = 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222);
       $str = ($path != -1) ? sprintf($ltrstr['common link to trail'], $sess->url(build_good_url($path)), $target, $target, friendlyname($path)): "<b>".$this->data['Title']."</b>";
       switch ($this->data['Type']) {
        case "subscribed":
            $str = sprintf($ltrstr['trail subscribed'], $str);
            break;
        case "unsubscribed":
            $str = sprintf($ltrstr['trail unsubscribed'], $str);
            break;
        case "accepted":
            $str = sprintf($ltrstr['recommendation accepted'], $str);
            break;
        case "declined":
            $str = sprintf($ltrstr['recommendation declined'], $str);
            break;
       }
   }
   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);

   $tpl = new Template(APPLICATION_HOME."/templates/messages/short", "keep");
   $tpl->set_file(array("simple" => "general_notifications_user.html"));
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $tpl->set_var("TEXT", $str);
   $tpl->set_var("USERURL", $sess->url(build_good_url("/Experts/".$this->sender)));
   $tpl->set_var("USERNAME", $this->sender);
   $tpl->set_var("DELURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=delete&viewtype=0");
   $tpl->set_var("READURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&viewtype=1");
   $tpl->set_var("DATE", $d);
   $tpl->set_var("MYID", $this->id);
   $tpl->parse("main", "simple");
   return $tpl->get("main");
  }

  function user_interaction(&$http_vars){
   if ($http_vars['action'] == "delete")
    $this->self_destruct();
  }
}






















class ltrAdminModificationMsg extends ltrMessage{
  function ltrAdminModificationMsg($dataset){
   $this->ltrMessage($dataset);
  }
  
  function print_simple($val){
   global $ltrstr, $PHP_SELF;

   $trailpath = id2path($this->data['Trail']);

   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);

   $tpl = new Template(APPLICATION_HOME."/templates/messages/short", "keep");
   $tpl->set_file(array("simple" => "admin_cn.html"));
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $target = 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222);
   $url    = build_good_url($trailpath);
   $name   = friendlyname($trailpath);
   $str = ($trailpath != -1) ? sprintf($ltrstr['trail_link'], $url , $target, $target, $name) : "<b>".$this->data['Title']."</b>";
   $tpl->set_var("TRAIL", $str);
   $tpl->set_var("TEXT", $ltrstr["Message_Text_".$this->type]);
   $tpl->set_var("DELURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=delete&viewtype=0");
   $tpl->set_var("READURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&viewtype=1");
   $tpl->set_var("MYID", $this->id);
   $tpl->set_var("DATE", $d);

   $b['begin'] = (!$this->is_read) ? '<b>'  : "";
   $b['end']   = (!$this->is_read) ? '</b>' : "";
   
   $tpl->set_var("BOLD_BEGIN", $b['begin']);
   $tpl->set_var("BOLD_END",   $b['end']);

   
   $tpl->parse("main", "simple");
   return $tpl->get("main");
  }
  
  function print_extended($val){
   global $ltrstr, $PHP_SELF;
   $this->set_read();
   $trailpath = id2path($this->data['Trail']);

   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);

   $tpl = new Template(APPLICATION_HOME."/templates/messages/long", "keep");
   $tpl->set_file(array("extended" => "admin_cn.html"));
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $str = ($trailpath != -1) ? sprintf($ltrstr['trail_link'], $url , $target, $target, $name) : "<b>".$this->data['Title']."</b>";
   $tpl->set_var("TRAIL", $str);
/*    $tpl->set_var("TRAILURL", build_good_url($trailpath)); 
   $tpl->set_var("TRAILNAME", friendlyname($trailpath));*/
   $tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));
   $tpl->set_var("TEXT", $ltrstr["Message_Text_".$this->type]);
   $tpl->set_var("DELURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=delete&viewtype=0");
   $tpl->set_var("CLOSEURL", build_good_url($PHP_SELF));
   $tpl->set_var("EXTENDED_INFORMATION", $ltrstr['Admin Trail del']);
   $tpl->set_var("MYID", $this->id);
   $tpl->set_var("DATE", $d);
   $tpl->parse("main", "extended");
   return $tpl->get("main");
  }
  
  
  function user_interaction(&$http_vars){
   if ($http_vars['action'] == "delete")
    $this->self_destruct();
  }
}

class ltrSuggestionMsg extends ltrMessage{
  function ltrSuggestionMsg($dataset){
   $this->ltrMessage($dataset);
  }
  
  function get_new_rec_flag(){
   global $auth;
   if(!defined("SUBSCRIPTIONS_INC"))
    include("dbapi/subscriptions.inc");
   
   $res = 0;
   $nm = get_notification_method($auth->auth['uname'], $this->data['Trail']);
   if ($nm != -1){
    $res = AD_SUBSCRIBED;
   }else{
    $res = AD_UNSUBSCRIBED;
   }
   return $res;
  }
  
  
  
  function print_simple($val){
   global $sess, $glob_userdata, $auth, $ltrstr, $PHP_SELF;

   $this->nodeinfo = get_node_info($this->data['Trail']);
   $trailpath = $this->nodeinfo['path'];
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;

   $tpl = new Template(APPLICATION_HOME."/templates/messages/short", "keep");
   
   if ($this->is_inbox){
    $tpl->set_file(array("simple" => "recommendation.html"));
    $user = $this->sender;
    $verb = $ltrstr['recommends'];
    $tpl->set_var("VERB", $verb);
    $b['begin'] = (!$this->is_read) ? '<b>'  : "";
    $b['end']   = (!$this->is_read) ? '</b>' : "";
    $tpl->set_var("BOLD_BEGIN", $b['begin']);
    $tpl->set_var("BOLD_END",   $b['end']);
    $tpl->set_var("OPTIONLIST", $this->print_option_list($this->data['Trail'], false, true));
   }else{
    $tpl->set_file(array("simple" => "recommendation_out.html"));
    $user = $this->to_name;
    $tpl->set_var("DELURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=delete&viewtype=0");
   }
   
   $tpl->set_var("COLOR", $color);
   $tpl->set_var("MYID", $this->id);
   $tpl->set_var("USERURL", $sess->url(build_good_url("/Experts/".$user)));
   $tpl->set_var("USERNAME", $user);

   $tpl->set_var("TRAILURL", $sess->url(build_good_url($trailpath)));
   $tpl->set_var("TRAILNAME", friendlyname($trailpath));
   $tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));

   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);
   $tpl->set_var("DATE", $d);
   
   $tpl->parse("main", "simple");
   return $tpl->get("main");
  }

  function print_extended($val){
   global $sess, $glob_userdata, $ltrstr, $glob_userdata, $PHP_SELF, $auth;
   
   $this->set_read();

   $tpl = new Template(APPLICATION_HOME."/templates/messages/long", "keep");
   $tpl->set_file(array("extended" => "recommendation.html"));
   $trailpath = id2path($this->data['Trail']);
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $tpl->set_var("MYID", $this->id);
   $tpl->set_var("USERURL", $sess->url(build_good_url("/Experts/".$this->sender)));
   $tpl->set_var("USERNAME", $this->sender);

   if(!defined("SUBSCRIPTIONS_INC"))
    include("dbapi/subscriptions.inc");   

   $flag = $this->get_new_rec_flag();
   if ((isset($this->data['action_done'])) and ($flag != $this->data['action_done'])){
    $this->data['action_done'] = $flag;
    $this->update_data();
   }elseif((!isset($this->data['action_done'])) and ($flag == AD_SUBSCRIBED)){
    $this->data['action_done'] = $flag;
    $this->update_data();
   }
   $verb = $ltrstr['recommends'];
   $tpl->set_var("VERB", $verb);
   
   $tpl->set_var("TRAILURL", $sess->url(build_good_url($trailpath)));
   $tpl->set_var("TRAILNAME", friendlyname($trailpath));
   $tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));
   
   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);
   $tpl->set_var("DATE", $d);
   $tpl->set_var("OPTIONLIST", $this->print_option_list($this->data['Trail'], true, false));
   $tpl->set_var("USER_MESSAGE", nl2br(longwrap($this->data['message'])));
   
   if (!isset($this->data['action_done'])){ //recommendation still valid
     $str = sprintf($ltrstr['acceptnote'], build_good_url($PHP_SELF)."?objectid=".$this->id."&action=subscribe&viewtype=0");
   }elseif($this->data['action_done'] == AD_SUBSCRIBED){
     $str = $ltrstr['acceptednote'];
   }else{
     $str = $ltrstr['declinednote'];
   }
   
//   $str = (!$subscribed) ?  : "";
   $tpl->set_var("ACCEPTNOTE",  $str);
   $tpl->parse("main", "extended");
   return $tpl->get("main");
  }
  
  
  function print_option_list($trail_id, $open, $do_db=true){
   global $auth, $ltrstr, $PHP_SELF;

   if ($do_db){
    $flag = $this->get_new_rec_flag();
   if ((isset($this->data['action_done'])) and ($flag != $this->data['action_done'])){
     $this->data['action_done'] = $flag;
     $this->update_data();
    }elseif((!isset($this->data['action_done'])) and ($flag == AD_SUBSCRIBED)){
     $this->data['action_done'] = $flag;
     $this->update_data();
    }
   }
    
   $done = (isset($this->data['action_done']));
   
   if ($open)
    $str = sprintf($ltrstr['close_link'], build_good_url($PHP_SELF)).' | ';
   else
    $str = sprintf($ltrstr['read_link'], build_good_url($PHP_SELF)."?objectid=".$this->id."&viewtype=1").' | ';
    
   if ($done){
    $str .= sprintf($ltrstr['delete_link'], build_good_url($PHP_SELF)."?objectid=".$this->id."&action=delete&viewtype=0");
   }else{
    $str .= sprintf($ltrstr['subscribe_link'], build_good_url($PHP_SELF)."?objectid=".$this->id."&action=subscribe&viewtype=0").' | ';
    $str .= sprintf($ltrstr['decline_link'], build_good_url($PHP_SELF)."?objectid=".$this->id."&action=delete&viewtype=0");
   }
   return $str;
  }
  
  function user_interaction(&$http_vars){
   global $auth;

   $this->nodeinfo = get_node_info($this->data['Trail']);
   if (!$this->is_read)
    $this->set_read();
   if ($http_vars['action'] == "delete"){
    if (!isset($this->data['action_done'])){
     $this->data['action_done'] = AD_UNSUBSCRIBED;
     $this->update_data();
    }else{
     $this->self_destruct();
    }
   }elseif ($http_vars['action'] == "subscribe"){
    if(!defined("SUBSCRIPTOINS.INC"))
     include("dbapi/subscriptions.inc");
    subscribe($auth->auth['uname'], $this->data['Trail'], 999); //TODO: New notification-method-sheme
   /* $this->data['action_done'] = AD_SUBSCRIBED;
    $this->update_data(); */
   } 
  }
}

class ltrLMSMessageMsg extends ltrMessage{
  function ltrLMSMessageMsg($dataset){
   $this->ltrMessage($dataset);
  }
  
  function print_simple($val){
   global $sess, $PHP_SELF;
   
   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);
   $tpl = new Template(APPLICATION_HOME."/templates/messages/short", "keep");
   $tpl->set_file(array("simple" => "user_message.html"));
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $user = ($this->is_inbox) ? $this->sender : $this->to_name;
   $tpl->set_var("USERURL", $sess->url(build_good_url("/Experts/".$user)));
   $tpl->set_var("USERNAME", $user);
   $str = (strlen($this->data['subject']) > 20) ? substr($this->data['subject'], 0, 20)."..." : $this->data['subject'];
   $tpl->set_var("SHORTDESC", $str); 
   $tpl->set_var("DELURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=delete&viewtype=0");
   $tpl->set_var("READURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&viewtype=1");
   $tpl->set_var("MYID", $this->id);
   $tpl->set_var("DATE", $d);
   
   $b['begin'] = ((!$this->is_read and ($this->is_inbox))) ? '<b>'  : "";
   $b['end']   = ((!$this->is_read and ($this->is_inbox))) ? '</b>' : "";
   
   $tpl->set_var("BOLD_BEGIN", $b['begin']);
   $tpl->set_var("BOLD_END",   $b['end']);
   
   $tpl->parse("main", "simple");
   return $tpl->get("main");
  }
  
  function print_extended($val, $error=false){
   global $sess, $PHP_SELF, $ltrstr, $sess;
   echo("Id: {$this->id}<br>");
   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);
   $tpl = new Template(APPLICATION_HOME."/templates/messages/long", "keep");
   $tpl->set_file(array("long" => "user_message.html"));
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $user = ($this->is_inbox) ? $this->sender : $this->to_name;
   $tpl->set_var("USERURL", $sess->url(build_good_url("/Experts/".$user)));
   $tpl->set_var("USERNAME", $user);
   $str = (strlen($this->data['subject']) > 20) ? substr($this->data['subject'], 0, 20)."..." : $this->data['subject'];
   $tpl->set_var("SHORTDESC", $str); 
   $tpl->set_var("DELURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=delete&viewtype=0");
   $tpl->set_var("CLOSEURL", $PHP_SELF);
   $tpl->set_var("DATE", $d);
   
   $str = "";
   if ($this->data['trail']){
    $trailpath = id2path($this->data['trail']);
    if ($trailpath != -1){
     $target = 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222);
     $str = sprintf($ltrstr['Message sent in trail'], $sess->url(build_good_url($trailpath)), $target, $target, friendlyname($trailpath));
    }
   }
   $str ='<i>'.$str.'</i>';
   $message = nl2br(longwrap($this->data['message']));
   
   $tpl->set_var("TRAILINFO", $str);
   if ($this->is_inbox){
    $this->set_read();
    $tpl->set_var("FORM", $this->print_re_form($error, $color));
    if ($this->data['quoted_reply'] != '') 
     $message .= sprintf($ltrstr['In Reply to'], $this->data['quoted_reply']);
   }else{
    $tpl->set_var("FORM", "");
   } 
   $tpl->set_var("ACTION", build_good_url($PHP_SELF));
   $tpl->set_var("MYID", $this->id);
   $tpl->set_var("MESSAGE", $message);
   $tpl->parse("main", "long");
   return $tpl->get("main");
  }
  
  function print_re_form($error, $color){
   $tpl = new Template(APPLICATION_HOME."/templates/messages/long", "keep");
   $tpl->set_file(array("long" => "re_form.html"));
   $tpl->set_var("COLOR", $color);
   if ($error){
    $tpl->set_var("ERRORSTRING", $ltrstr['Subject missing'].'<p>');
    $tpl->set_var("WARN_SUBJECT", $ltrstr['Form Error Mark']);
    $tpl->set_var("VALUE_SUBJECT", '');
   }else{
    $tpl->set_var("ERRORSTRING", "");
    $tpl->set_var("WARN_SUBJECT", "");
    $str = (substr($this->data['subject'], 0, 4) == "Re: ") ? $this->data['subject'] : 'Re: '.$this->data['subject'];
    $tpl->set_var("VALUE_SUBJECT", $str);
   } 
   $tpl->parse("main", "long");
   return $tpl->get("main");
  }
  
  function print_error($val){
   return $this->print_extended($val, true);
  }
  
  function user_interaction(&$http_vars){
   global $PHP_SELF, $auth;
   if ($http_vars['action'] == "delete"){
    $this->self_destruct();
    return "";
   }
   if ($http_vars['action'] == "answer"){
    if ( ($http_vars['field_subject'] == "") and ($http_vars['field_message'] == "") )
     return build_good_url($PHP_SELF).'?objectid='.$this->id.'&viewtype=2';
  
    if (!defined("TALK_INC"))
     include("messages/talk.inc");
    if (!defined("USER_INC"))
     include("dbapi/user.inc");
    
    $user = get_user_from_name($this->sender);
    $user = $user['User_ID'];
//    die("sende:".$this->sender);
    talk($auth->auth['uname'], $user, $http_vars['field_subject'], $http_vars['field_message'], -1, $this->data['message']);
    return "";
   }
  }
  
  
}

class ltrFriendshipRequestMsg extends ltrMessage{
  function ltrFriendshipRequestMsg($dataset){
   $this->ltrMessage($dataset);
  }
  
  function print_simple($val){
   global $sess, $PHP_SELF;

   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);
   $tpl = new Template(APPLICATION_HOME."/templates/messages/short", "keep");
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   
   if ($this->is_inbox){
    $tpl->set_file(array("simple" => "friendship_request.html"));
    $user = $this->sender;
    $tpl->set_var("READURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&viewtype=1");
    $tpl->set_var("ACCEPTURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=accept&viewtype=0");
    $b['begin'] = (!$this->is_read) ? '<b>'  : "";
    $b['end']   = (!$this->is_read) ? '</b>' : "";
   
    $tpl->set_var("BOLD_BEGIN", $b['begin']);
    $tpl->set_var("BOLD_END",   $b['end']);
   }else{
    $tpl->set_file(array("simple" => "friendship_request_out.html"));
    $user = $this->to_name;
   }
   
   $tpl->set_var("COLOR", $color);
   $tpl->set_var("USERURL", $sess->url(build_good_url("/Experts/".$user)));
   $tpl->set_var("USERNAME", $user);
   $tpl->set_var("DATE", $d);
   $tpl->set_var("MYID", $this->id);
   $tpl->set_var("DELURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=delete&viewtype=0");

   $tpl->parse("main", "simple");
   return $tpl->get("main");
  }

  function print_extended($val){
   global $sess, $PHP_SELF;

   $this->set_read();
   
   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);
   $tpl = new Template(APPLICATION_HOME."/templates/messages/long", "keep");
   $tpl->set_file(array("simple" => "friendship_request.html"));
   
   
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $tpl->set_var("USERURL", $sess->url(build_good_url("/Experts/".$this->sender)));
   $tpl->set_var("USERNAME", $this->sender);
   $tpl->set_var("DATE", $d);
   $tpl->set_var("MYID", $this->id);
   $tpl->set_var("USERMESSAGE", nl2br(longwrap($this->data['message'])));
   $tpl->set_var("DELURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=delete&viewtype=0");
   $tpl->set_var("ACCEPTURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=accept&viewtype=0");
   $tpl->set_var("CLOSEURL", build_good_url($PHP_SELF));

   $tpl->parse("main", "simple");
   return $tpl->get("main");
  }
  
  function user_interaction(&$http_vars){
   global $auth;
   if ($http_vars['action'] == "delete"){
    $this->self_destruct();
    if ($this->is_inbox){
     if (!defined("MSG_FRIENDSHIPS_INC"))
      include("messages/friendships.inc");
     $user = get_user_from_name($this->sender);
     $user = $user['User_ID']; 
     friendship_notification($auth->auth['uname'], $user, "ignore");
     friendship_notification($auth->auth['uname'], $auth->auth['uid'], "ignore_self", $this->sender);
     if (!$this->is_read)
      $this->set_read();
    } 
   }
   if ($http_vars['action'] == "accept"){
    if (!defined("MSG_FRIENDSHIPS_INC"))
     include("messages/friendships.inc");
    if (!defined("PERMISSIONS_INC"))
     include("dbapi/permissions.inc");

    $user = get_user_from_name($this->sender);
    $user = $user['User_ID']; 
    create_friendship($user, $this->target);
    friendship_notification($auth->auth['uname'], $user, "accepted");
    friendship_notification($auth->auth['uname'], $auth->auth['uid'], "accepted_self", $this->sender);
    if (!$this->is_read)
     $this->set_read();
    $this->self_destruct();
   }
  }
  
}

class ltrPollStartedMsg extends ltrMessage{
  function ltrPollStartedMsg($dataset){
   $this->ltrMessage($dataset);
  }

  function user_interaction(&$http_vars){
   if ($http_vars['action'] == "delete")
    $this->self_destruct();
  }
}


function &std_message($dataset){
 return new ltrStdMessageMsg($dataset);
}

function &std_user_message($dataset){
 return new ltrStdMessageUserMsg($dataset);
}

function &admin_modification($dataset){
 return new ltrAdminModificationMsg($dataset);
}

function &user_modification($dataset){
  return new ltrUserModificationMsg($dataset);
}

function &trail_suggestion($dataset){
 return new ltrSuggestionMsg($dataset);
}

function &lms_message($dataset){
 return new ltrLMSMessageMsg($dataset);
}

function &friendship_request($dataset){
 return new ltrFriendshipRequestMsg($dataset);
}

function &poll_started($dataset){
 return new ltrPollStartedMsg($dataset);
}
?>
