<html>
<head>

<LINK REL=stylesheet HREF="/styles/trail_1.css" TYPE="text/css">

<body>
<?
// Heise Newsticker - Headlines auslesen
// Basiert auf HeiseNewsticker Auslese Script von Jan "_bErT_" Lehnardt,
// Modifiziert von Peter "[DiSAStA]" Petermann liest nur noch Headlines, 
// und bietet links zu den entsprechenden News
// Danke auch an Tobias "Yapa" Ratschiller.
//
// Ich habe das Script noch weiter angepa˜t. So werden die News zu in einer
// bestimmten Zeitspanne (Default = 3 Stunden) abgerufen und dann 
// gecached.
//
// Fehler (</A></FONT></B> zuviel) wurde beseitigt. Die Quelltextausgabe wurde
// versch÷nert (Pro Eintrag eine neue Zeile.
//
// Mark Kronsbein
// http://www.php-homepage.de 


$link_prefix    =     "&nbsp;&nbsp;o";
$link_postfix    =     "<BR>\n";
$cache_file    =     "/home/linktrai/tmp/heise.cache";
$cache_time    =    3600;


$items        =    0;
$time        =    split( " ", microtime());

srand((double)microtime()*1000000);
$cache_time_rnd    =    300 - rand(0, 600);

if ((!(file_exists($cache_file))) || ((filectime($cache_file) + $cache_time - $time[1]) + $cache_time_rnd < 0) || (!(filesize($cache_file))) ) {
$fp1=fopen("http://www.heise.de/default.shtml", "r");
$string=fread($fp1,20000);

ereg("<!-- MITTE \(NEWS\) -->(.*)<!-- MITTE \(NEWS-UEBERBLICK\) -->",
$string, $matches);
$match=str_replace("HREF=\"/newsticker/","href=\"http://www.heise.de/newsticker/",$matches[1]);

$exp="#newsticker/data/(.*)/\">(.*</FONT></B>)#i";
preg_match_all($exp, $matches[1], $matchin);

for($i=0;$i<count($matchin[1]);$i++)
   {
$body.= "$link_prefix <a href=\"http://www.heise.de/newsticker/data/".$matchin[1][$i]."/\">" . $matchin[2][$i]. "</a><br>\n";
   }
$body = eregi_replace( "</A></FONT></B>", "", $body);
fclose($fp1);

$fpwrite = fopen($cache_file,'w');
fputs($fpwrite,  "$body");
fclose($fpwrite);
}
include($cache_file);
?> 
</body>
</html>                            
