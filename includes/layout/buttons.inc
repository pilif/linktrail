<?php
define("BUTTONS_INC", true);
/*
 Here you define the Link-Urls or the text that is used for the
 UI-Buttons.
*/
define("BTN_DEL_LINK",  '<a href="[URL]"[target]><img src="/img/del.gif"[align]border="0"></a>');
define("BTN_EDIT_LINK", '<a href="[URL]"[target]><img src="/img/edit.gif"[align]border="0"></a>');

define("BTN_EDITT_LINK", '<a href="[URL]"[target]>'.$ltrstr['Edit this link'].'</a>');
define("BTN_REL_LINK",  '<a href="[URL]"[target]>'.$ltrstr['Find related'].'</a>');


define("BTN_ADD_LINK",  '<a href="[URL]"[target]><img src="/img/add.gif"[align]border="0"></a>');
define("BTN_YES_LINK",  '<a href="[URL]"[target]><img src="/img/yes.gif"[align]border="0"></a>');
define("BTN_NO_LINK",   '<a href="[URL]"[target]><img src="/img/no.gif"[align]border="0"></a>');
define("BTN_OK_LINK",   '<a href="[URL]"[target]><img src="/img/ok.gif"[align]border="0"></a>');

define("BTN_DEL_FORM",  '<input type="image" name="del"[align]src="/img/del.gif" border="0"[script]>');
define("BTN_EDIT_FORM", '<input type="image" name="edit"[align]src="/img/edit.gif" border="0">');
define("BTN_ADD_FORM",  '<input type="submit" name="add"[align]value="'.$ltrstr['Add a link button'].'">');
define("BTN_YES_FORM",  '<input type="image" name="yes"[align]src="/img/yes.gif" border="0">');
define("BTN_NO_FORM",   '<input type="image" name="no"[align]src="/img/no.gif" border="0">');
define("BTN_OK_FORM",   '<input type="image" name="ok"[align]src="/img/ok.gif" border="0">');


////////////////////////////////////////////////////////////////////////////////
//     Please do not edit past this line (unless you know what you do)        //
////////////////////////////////////////////////////////////////////////////////

//Returns a button. $type is a number between 0 and 4, defining the
//type of the button (del/edit/add/yes/no). $url is the url to jump to
//if it is empty, the <input>-style-button will be returned

function get_button($type, $url="", $align="", $target="", $script=""){
 global $sess;
 
 if ($url != "")
  $url = $sess->url($url);
  
 switch ($type) {
     case 0:
         if ("" == $url)
          $str = BTN_DEL_FORM;
         else
          $str = str_replace("[URL]", $url, BTN_DEL_LINK);
         break;
     case 1:
         if ("" == $url)
          $str = BTN_EDIT_FORM;
         else
          $str = str_replace("[URL]", $url, BTN_EDIT_LINK);         
         break;
     case 2:
         if ("" == $url)
          $str = BTN_ADD_FORM;
         else
          $str = str_replace("[URL]", $url, BTN_ADD_LINK);         
         break;
     case 3:
         if ("" == $url)
          $str = BTN_YES_FORM;
         else
          $str = str_replace("[URL]", $url, BTN_YES_LINK);
         break;
     case 4:
         if ("" == $url)
          $str = BTN_NO_FORM;
         else
          $str = str_replace("[URL]", $url, BTN_NO_LINK);         
         break;
     case 5:
         if ("" == $url)
          $str = BTN_OK_FORM;
         else
          $str = str_replace("[URL]", $url, BTN_OK_LINK);         
         break;
     case 6:
          $str = str_replace("[URL]", $url, BTN_REL_LINK);         
         break;
     case 7:
         if ("" == $url)
          $str = BTN_EDIT_FORM;
         else
          $str = str_replace("[URL]", $url, BTN_EDITT_LINK);         
         break;
 }
 if ($align != "")
  $str = str_replace("[align]", ' align="'.$align.'" ', $str);          
 else
  $str = str_replace("[align]", ' ', $str);          
 
 if ($target != "") 
  $str = str_replace("[target]", ' target="'.$target.'" ', $str);          
 else
  $str = str_replace("[target]", ' ', $str);          

 if ($script != "") 
  $str = str_replace("[script]", ' '.$script, $str);          
 else
  $str = str_replace("[script]", '', $str);          
 
 return $str;     
}
?>