<?php
 define("SPELLING_INC", true);
 
function check_language($text, $check_for="en"){
/* $words = explode(" ", $text);
 $pspell_link = pspell_new ($check_for, "", "", "", (PSPELL_FAST|PSPELL_RUN_TOGETHER));
 $ok  = 0;
 $bad = 0;
 foreach($words as $word){
  if (strlen($word) < 3) continue;
  if (pspell_check ($pspell_link, $word))
   $ok++;
  else
   $bad++;
 }
 return !($ok < $bad);*/
 return true;
}
?>
