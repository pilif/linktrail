<?php
include("languages/en.inc");

define("DB_SERVER",   "troll.linktrail.work");
define("DB_DATABASE", "linktrail");
define("DB_USER",     "root");
define("DB_PASS",     "oelberg");

define("SEARCH_HOST", "troll.linktrail.work");
define("SEARCH_PORT", 7777);

define("CACHE_DIR", APPLICATION_HOME.'/cache');

define("DFLAG_FORMAT", ' <font face="Verdana, Arial, Helvetica, sans-serif" size="1" color="red">[FLAG]</font>');

define("CURR_COLOR", "#000000");
define("ERR_COLOR" , "#ff0000");

define("BACK_COLOR2", "#ffffff");
define("BACK_COLOR1", "#E4F4C6");
define("BACK_COLOR3", "#dcdcdc");
define("SEARCH_HILIGHT", "#C1E054");

//How many columns will be used for displaying subnodes?
define("SUBCOLCOUNT", 2);

//How many Trails will be returned by get_trails
//if $all is false?
define("DIR_MAX_TRAILS", 15);

define("MSG_MAX_MSGS", 18);


define("DIR_MAX_NEWTRAILS", 10);

define("DID_YOU_KNOW_COUNT", 10);

//Pagesize in the Search-Results
define("DIR_MAX_SEARCH", 10);
define("TRAIL_MAX_SEARCH", 10);



//How many levels deep should get_trails look for
//good trails to climb up?
define("TRAIL_LEVEL_UP_COUNT", 2);

//How many percent of the trailcount in a node may be accepted as climbing?
define("TRAIL_UP_COUNT", 10);

//How many Trails can climb up?
define("REM_POINTS", 0.75);

//How many percent of all Subscribers of a trail in a category
//may be top-experts? (ordered by last_online-Date)
define("EXP_MAX_SUBSCRIBERS", 50);

//How many topexperts re displayed in the sidebar?
define("EXP_VIEW_MAX_TOPEX", 10);

//If a category has less then MIN_TRAILS trails, the percent-rule does not
//apply and the application tries to get MIN_TRAILS from subnodes
define("MIN_TRAILS", 10);

//How many levels can a category climb up?
define("CAT_LEVEL_UP_COUNT", 3);
define("CAT_UP_COUNT_LO", 2);
define("CAT_UP_COUNT_HI", 3);
define("CAT_UP_MAX_CHARS", 25);


//$iwantto_order = array("register", "logon", "logoff", "mypage", "readmsg",  "dotour", "trailadd", "admin");
$iwantto_order = array("logon", "register", "mypage", "dotour",  "trailadd", "catdrop", "admin", "bugreport", "readmsg");
$iwantto_mypage_order = array("logon", "contactauthor", "mypage", "register", "dotour", "changeprefs", "friendship", "bugreport", "readmsg");
$invalid_user_names = array('New_User', 'nobody', 'Forgotten_Password', 'Forgotten Password', '-1');
//-1 is used when sending a message as admin/system
//I don't really think that someone wants to call himself -1, but I put it here anyway...

$static_about['main'] = array(
                        $ltrstr['Overview']        => array('/About/', "Overview"),
                       // $ltrstr['Company History'] => '/About_linktrail/Company_history',
                        $ltrstr['Our Team']        => array('/About/Team', "The founding team"),
                        $ltrstr['Our Philosophy']  => array('/About/Philosophy', "Our philosophy"),
                        $ltrstr['Careers']         => array('/About/Jobs', "Jobs"),
                        $ltrstr['Contact Us']      => array('/About/Contact/', "Contact")
                      );

$static_about['press'] = array(
                        $ltrstr['Overview']          => array('/About/Press_Center/', "Overview")
                   //     $ltrstr['Press Releases']  => '/About/Press_Center/Releases',
                   //     $ltrstr['In the News']     => '/About/Press_Center/In_the_news',
                   //     $ltrstr['FAQ']             => '/About/Press_Center/FAQ',
                   //     $ltrstr['Official Logos']  => '/About/Press_Center/Official_logos',
                   //     $ltrstr['Contact Us']      => '/About/Contact/Press'
                      );

$static_about['adv']  = array(
                        $ltrstr['Overview']        => array('/About/Advertise/', "Overview"),
                   //     $ltrstr['Why Advertise']   => '/About/Advertise_with_us/Why_advertise',
                        $ltrstr['Audience']        => array('/About/Advertise/Audience', "Audience"),
                        $ltrstr['Ad Oportunities'] => array('/About/Advertise/Ad_opportunities', "Ad opportunities")
//                        $ltrstr['Contact Us']      => '/About/Contact/Advertise'
                      );

$static_help['preview'] = array(
//                        $ltrstr['Overview']        => array('/Help/', "Overview"),
                        $ltrstr['Privacy']          => array('/Help/Privacy', "Privacy statement"),
                        $ltrstr['Terms of service'] => array('/Help/Terms_of_service', "Terms of service"),
                        $ltrstr['Report Bug']       => array('/Help/Report_a_bug', $ltrstr['Report Bug']),
                        $ltrstr['Tour']             => array('/Tour/', "Tour"),
                      );

$glob_allowed_tags = Array('br', 'p', 'hr', 'h5', 'h6', 'b', 'i', 'code', 'em', 'cite', 
                           'menu', 'q', 'ul', 'ol', 'li', 'u', 'pre', 'tt', 'kbd', 'listing',
                           'sup', 'sub', 'a', 'small', 'strong', 'blockquote', 'center');

$tour_steps[0] = Array('What',             $ltrstr['What is linktrail']);
$tour_steps[1] = Array('Trail',            $ltrstr['What is a trail']);
$tour_steps[2] = Array('Add_links',        $ltrstr['Add your best links']);
$tour_steps[3] = Array('Find_a_person',    $ltrstr['Find a person']);
$tour_steps[4] = Array('Contact_a_member', $ltrstr['Contact any member']);
$tour_steps[5] = Array('Make_friendship',  $ltrstr['Make friendship']);
$tour_steps[6] = Array('Subscribe',        $ltrstr['Subscribe a trail']);
$tour_steps[7] = Array('Create',           $ltrstr['Create a trail']);
$tour_steps[8] = Array('Set_permissions',  $ltrstr['Set permissions']);
//$tour_steps[8] = Array('Register_now',     $ltrstr['Register now']);

?>
