<?
function clean_html($str)
{
 $str = htmlentities($str) ;
 $str = preg_replace("%&quot;%","\"",$str) ;
 $h= 'br|p|hr|h3|h4|h5|h6|b|i' .        
     '|code|em|font|cite|menu|q' .
     '|dl|dt|dd|ul|ol|dir|li|u' .
     '|pre|tt|kbd|listing|s|sup|sub' .
     '|a|small|big|strong|blink' .
     '|blockquote|center|dfn|strike';
$str = preg_replace("%&lt;(/?($h)(\b[^&]*)?)&gt;%i","<\\1>",$str) ; 
return $str ;
}
?>