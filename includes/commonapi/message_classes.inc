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

   $tpl = new Template("/home/linktrai/templates/messages/short", "keep");
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

   $tpl = new Template("/home/linktrai/templates/messages/short", "keep");
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

   $tpl = new Template("/home/linktrai/templates/messages/short", "keep");
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

   $tpl = new Template("/home/linktrai/templates/messages/long", "keep");
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

class ltrUserModificationMsg extends ltrMessage{

  function ltrUserModificationMsg($dataset){
   $this->ltrMessage($dataset);
   $this->trail = $dataset['Trail'];
  }
  
  function get_subs(){
   global $myDB;
    
   $arr = Array();
    
   if (!defined("SQL_MESSAGES"))
    include("dbapi/sql_messages.inc");
    
   $query = FormatSQL(MSG_READ_CHILD, array($this->target, $this->trail, $this->id));
//   die($query);
   $myDB->query($query);
   while($myDB->next_record()){
    $arr[] = $myDB->Record;
   }
   return $arr;
  }
 
  function print_simple($val){
   global $PHP_SELF, $glob_userdata;
   
//  die($this->test);
   $tpl = new Template("/home/linktrai/templates/messages/short", "keep");
   $tpl->set_file(array("simple" => "user_cn.html"));
   $trailpath = id2path($this->data['Trail']);
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $tpl->set_var("TRAILURL", build_good_url($trailpath));
   $tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));
   $tpl->set_var("TRAILNAME", friendlyname($trailpath));
   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);
   $tpl->set_var("MYID", $this->id);
   $tpl->set_var("DATE", $d);
   $tpl->set_var("DELURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=delete&viewtype=0");
   $tpl->set_var("READURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&viewtype=1");

   $b['begin'] = (!$this->is_read) ? '<b>'  : "";
   $b['end']   = (!$this->is_read) ? '</b>' : "";
   
   $tpl->set_var("BOLD_BEGIN", $b['begin']);
   $tpl->set_var("BOLD_END",   $b['end']);

   $tpl->parse("main", "simple");
   return $tpl->get("main");
  }
  

  function print_extended($val){
   global $PHP_SELF, $sess, $ltrstr;

   $this->set_read();

   $tpl = new Template("/home/linktrai/templates/messages/long", "keep");
   $tpl->set_file(array("extended" => "user_cn.html"));
   $trailpath = id2path($this->data['Trail']);
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $tpl->set_var("TRAILURL", build_good_url($trailpath));
   $tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));
   $tpl->set_var("TRAILNAME", friendlyname($trailpath));
   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);
   $tpl->set_var("TOPDATE", $d);
   $tpl->set_var("DELURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=delete&viewtype=0");
   $tpl->set_var("CLOSEURL", build_good_url($PHP_SELF));
   
   $tpl->set_block("extended", "child", "children");
   //Firstly: let's get the Message that created this object:
   $tpl->set_var("INITIATORURL", $sess->url(build_good_url('/Experts/'.$this->sender)));
   $tpl->set_var("INITIATOR", $this->sender);
   $tpl->set_var("ACTION", $ltrstr['User_Action_'.$this->type]);
   $tpl->set_var("LINKTITLE", "(".$this->data['Link_Title'].")");
   $tpl->set_var("SUBDATE", $d);
   $tpl->parse("children", "child", true);
   
   //now to the children:
//   die(count($childmessages)."##".$this->id);
   $childmessages = $this->get_subs();
   for($x=0; $x < count($childmessages); $x++) {
    $data_arr = unserialize($childmessages[$x]['Data']);
    $tpl->set_var("INITIATORURL", $sess->url(build_good_url('/Experts/'.$childmessages[$x]['Sender'])));
    $tpl->set_var("INITIATOR", $childmessages[$x]['Sender']);
    $tpl->set_var("ACTION", $ltrstr['User_Action_'.$childmessages[$x]['MessageType']]);
    $tpl->set_var("LINKTITLE", "(".$data_arr['Link_Title'].")");
    $dh = friendlydate($childmessages[$x]['Date']);
    $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);
    $tpl->set_var("DATE", $d);
    $tpl->parse("children", "child", true);
   }
   $tpl->set_var("MYID", $this->id);
   $tpl->parse("main", "extended");
   return $tpl->get("main");
  }
  
  
  function user_interaction(&$http_vars){
   if ($http_vars['action'] == "delete"){
    $this->remove_children(); //remove all the sub-messages
    $this->self_destruct();
    return "";
   }
  }
  
  function remove_children(){
   global $myDB;
   
   $query = FormatSQL(MSG_KILL_CHILDREN, array($this->target, $this->trail));
   $myDB->query($query);
  }
}

class ltrSuggestionMsg extends ltrMessage{
  function ltrSuggestionMsg($dataset){
   $this->ltrMessage($dataset);
  }

  function print_simple($val){
   global $sess, $glob_userdata, $auth, $ltrstr;

   $this->nodeinfo = get_node_info($this->data['Trail']);
   
   $tpl = new Template("/home/linktrai/templates/messages/short", "keep");
   $tpl->set_file(array("simple" => "recommendation.html"));
   $trailpath = $this->nodeinfo['path'];
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $tpl->set_var("MYID", $this->id);
   $tpl->set_var("USERURL", $sess->url(build_good_url("/Experts/".$this->sender)));
   $tpl->set_var("USERNAME", $this->sender);

   if(!defined("SUBSCRIPTIONS_INC"))
    include("dbapi/subscriptions.inc");

   $nm = get_notification_method($auth->auth['uname'], $this->data['Trail']);
   $subscribed = ($nm != -1);
   $verb = ($subscribed) ? $ltrstr['recommended'] : $ltrstr['recommends'];
   $tpl->set_var("VERB", $verb);
   
   $tpl->set_var("TRAILURL", $sess->url(build_good_url($trailpath)));
   $tpl->set_var("TRAILNAME", friendlyname($trailpath));
   $tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));

   $b['begin'] = (!$this->is_read) ? '<b>'  : "";
   $b['end']   = (!$this->is_read) ? '</b>' : "";
   
   $tpl->set_var("BOLD_BEGIN", $b['begin']);
   $tpl->set_var("BOLD_END",   $b['end']);
   
   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);
   $tpl->set_var("DATE", $d);
   $tpl->set_var("OPTIONLIST", $this->print_option_list($this->data['Trail'], false, $subscribed));
   
   $tpl->parse("main", "simple");
   return $tpl->get("main");
  }

  function print_extended($val){
   global $sess, $glob_userdata, $ltrstr, $glob_userdata, $PHP_SELF, $auth;
   
   $this->set_read();

   $tpl = new Template("/home/linktrai/templates/messages/long", "keep");
   $tpl->set_file(array("extended" => "recommendation.html"));
   $trailpath = id2path($this->data['Trail']);
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $tpl->set_var("MYID", $this->id);
   $tpl->set_var("USERURL", $sess->url(build_good_url("/Experts/".$this->sender)));
   $tpl->set_var("USERNAME", $this->sender);

   if(!defined("SUBSCRIPTIONS_INC"))
    include("dbapi/subscriptions.inc");   

   $nm = get_notification_method($auth->auth['uname'], $this->data['Trail']);
   $subscribed = ($nm != -1);
   $verb = ($subscribed) ? $ltrstr['recommended'] : $ltrstr['recommends'];
   $tpl->set_var("VERB", $verb);
   
   $tpl->set_var("TRAILURL", $sess->url(build_good_url($trailpath)));
   $tpl->set_var("TRAILNAME", friendlyname($trailpath));
   $tpl->set_var("TARGET", 'trail'.($glob_userdata['NewWindow'] ? rand(11111,99999) : 222));
   
   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);
   $tpl->set_var("DATE", $d);
   $tpl->set_var("OPTIONLIST", $this->print_option_list($this->data['Trail'], true, $subscribed));
   $tpl->set_var("USER_MESSAGE", nl2br(longwrap($this->data['message'])));
   $str = (!$subscribed) ? sprintf($ltrstr['acceptnote'], build_good_url($PHP_SELF)."?objectid=".$this->id."&action=subscribe&viewtype=0") : "";
   $tpl->set_var("ACCEPTNOTE",  $str);
   $tpl->parse("main", "extended");
   return $tpl->get("main");
  }
  
  
  function print_option_list($trail_id, $open, $subscribed){
   global $auth, $ltrstr, $PHP_SELF;

   if ($open)
    $str = sprintf($ltrstr['close_link'], build_good_url($PHP_SELF)).' | ';
   else
    $str = sprintf($ltrstr['read_link'], build_good_url($PHP_SELF)."?objectid=".$this->id."&viewtype=1").' | ';
    
   if ( $subscribed){
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
   
   if ($http_vars['action'] == "delete"){
    $this->self_destruct();
    if(!defined("SUBSCRIPTIONS_INC"))
     include("dbapi/subscriptions.inc");   
    if(!defined("MSG_SUGGESTIONS_INC"))
     include("messages/suggestions.inc");
    
    $nm = get_notification_method($auth->auth['uname'], $this->data['Trail']);
    $subscribed = ($nm != -1);
    if (!$subscribed){ //if the trail is not subscriebed and the user kills the message, this is a declination
     $user = get_user_from_name($this->sender);
     $user = $user['User_ID'];
     sub_action($auth->auth['uname'], $user, $this->nodeinfo, "declined");
    }
   }if ($http_vars['action'] == "subscribe"){
    if(!defined("SUBSCRIPTOINS.INC"))
     include("dbapi/subscriptions.inc");
    if(!defined("MSG_SUGGESTIONS_INC"))
     include("messages/suggestions.inc");
    subscribe($auth->auth['uname'], $this->data['Trail'], 999); //TODO: New notification-method-sheme
    $user = get_user_from_name($this->sender);
    $user = $user['User_ID'];
    sub_action($auth->auth['uname'], $user, $this->nodeinfo, "accepted");
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
   $tpl = new Template("/home/linktrai/templates/messages/short", "keep");
   $tpl->set_file(array("simple" => "user_message.html"));
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $tpl->set_var("USERURL", $sess->url(build_good_url("/Experts/".$this->sender)));
   $tpl->set_var("USERNAME", $this->sender);
   $str = (strlen($this->data['subject']) > 20) ? substr($this->data['subject'], 0, 20)."..." : $this->data['subject'];
   $tpl->set_var("SHORTDESC", $str); 
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
  
  function print_extended($val, $error=false){
   global $sess, $PHP_SELF, $ltrstr, $sess;

   $this->set_read();
   
   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);
   $tpl = new Template("/home/linktrai/templates/messages/long", "keep");
   $tpl->set_file(array("long" => "user_message.html"));
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $tpl->set_var("USERURL", $sess->url(build_good_url("/Experts/".$this->sender)));
   $tpl->set_var("USERNAME", $this->sender);
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
   $tpl->set_var("MESSAGE", nl2br(longwrap($this->data['message'])));
   $tpl->set_var("TRAILINFO", $str);
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
   $tpl->set_var("ACTION", build_good_url($PHP_SELF));
   $tpl->set_var("MYID", $this->id);
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
    talk($auth->auth['uname'], $user, $http_vars['field_subject'], $http_vars['field_message']);
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
   $tpl = new Template("/home/linktrai/templates/messages/short", "keep");
   $tpl->set_file(array("simple" => "friendship_request.html"));
   
   
   $color = ($val) ? BACK_COLOR1 : BACK_COLOR2;
   $tpl->set_var("COLOR", $color);
   $tpl->set_var("USERURL", $sess->url(build_good_url("/Experts/".$this->sender)));
   $tpl->set_var("USERNAME", $this->sender);
   $tpl->set_var("DATE", $d);
   $tpl->set_var("MYID", $this->id);
   $tpl->set_var("DELURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=delete&viewtype=0");
   $tpl->set_var("READURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&viewtype=1");
   $tpl->set_var("ACCEPTURL", build_good_url($PHP_SELF)."?objectid=".$this->id."&action=accept&viewtype=0");

   $b['begin'] = (!$this->is_read) ? '<b>'  : "";
   $b['end']   = (!$this->is_read) ? '</b>' : "";
   
   $tpl->set_var("BOLD_BEGIN", $b['begin']);
   $tpl->set_var("BOLD_END",   $b['end']);
   
   
   $tpl->parse("main", "simple");
   return $tpl->get("main");
  }

  function print_extended($val){
   global $sess, $PHP_SELF;

   $this->set_read();
   
   $dh = friendlydate($this->dat);
   $d  = sprintf('%s/%s/%s', $dh['month'], $dh['day'], $dh['year']);
   $tpl = new Template("/home/linktrai/templates/messages/long", "keep");
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
    if (!defined("MSG_FRIENDSHIPS_INC"))
     include("messages/friendships.inc");
    $user = get_user_from_name($this->sender);
    $user = $user['User_ID']; 
    friendship_notification($auth->auth['uname'], $user, "ignore");
    friendship_notification($auth->auth['uname'], $auth->auth['uid'], "ignore_self", $this->sender);
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
