<?php

 if (!is_object($myDB)) {
  $testerDB = new DB_Tester;
 }

 if ($REQUEST_METHOD != "GET"){
  do_action();
 }else{
  print_form();
 }

 //got from px.sklar.com
function EmailEncodeQuotedPrintable($text,$header_charset="")
{
 $length=strlen($text);
 for($whitespace= "",$line=0,$encode= "",$index=0;$index<$length;$index++)
 {
  $character=substr($text,$index,1);
  $order=Ord($character);
  $encode=0;
  switch($order)
  {
   case 9:
   case 32:
    if($header_charset== "")
    {
     $previous_whitespace=$whitespace;
     $whitespace=$character;
     $character= "";
    }
    else
    {
     if($order==32)
      $character= "_";
     else
      $encode=1;
    }
    break;
   case 10:
   case 13:
    if($whitespace!= "")
    {
     if($header_charset== ""
     && $line+3>75)
     {
      $encoded.= "=\n";
      $line=0;
     }
     $encoded.=sprintf( "=%02X",Ord($whitespace));
     $line+=3;
     $whitespace= "";
    }
    $encoded.=$character;
    $line=0;
    continue 2;
   default:
    if($order>127
    || $order<32
    || $character== "="
    || ($header_charset!= ""
    && ($character== "?"
    || $character== "_"
    || $character== "("
    || $character== ")")))
     $encode=1;
    break;
  }
  if($whitespace!= "")
  {
   if($header_charset== ""
   && $line+1>75)
   {
    $encoded.= "=\n";
    $line=0;
   }
   $encoded.=$whitespace;
   $line++;
   $whitespace= "";
  }
  if($character!= "")
  {
   if($encode)
   {
    $character=sprintf( "=%02X",$order);
    $encoded_length=3;
   }
   else
    $encoded_length=1;
   if($header_charset== ""
   && $line+$encoded_length>75)
   {
    $encoded.= "=\n";
    $line=0;
   }
   $encoded.=$character;
   $line+=$encoded_length;
  }
 }
 if($whitespace!= "")
 {
  if($header_charset== ""
  && $line+3>75)
   $encoded.= "=\n";
  $encoded.=sprintf( "=%02X",Ord($whitespace));
 }
 if($header_charset!= ""
 && $text!=$encoded)
  return( "=?$header_charset?q?$encoded?=");
 else
  return($encoded);
}

 
function do_action(){
 global $HTTP_POST_VARS, $testerDB;
 
 add_account();
 
 if (isset($HTTP_POST_VARS['do_send_mail'])){
  send_welcome_mail($HTTP_POST_VARS['nachname'], 
                    $HTTP_POST_VARS['vorname'], 
                    $HTTP_POST_VARS['email'], 
                    $HTTP_POST_VARS['username'],
                    $HTTP_POST_VARS['passwort']);
 }	    
}

function add_account(){
 global $HTTP_POST_VARS, $testerDB;
 
 $nachname   = ($HTTP_POST_VARS['nachname']   != "") ? sprintf("'%s'", $HTTP_POST_VARS['nachname']) : "NULL";
 $vorname    = ($HTTP_POST_VARS['vorname']    != "") ? sprintf("'%s'", $HTTP_POST_VARS['vorname']) : "NULL";
 $nickname   = ($HTTP_POST_VARS['nickname']   != "") ? sprintf("'%s'", $HTTP_POST_VARS['nickname']) : "NULL";
 $email      = ($HTTP_POST_VARS['email']      != "") ? sprintf("'%s'", $HTTP_POST_VARS['email']) : "NULL";
 $homepage   = ($HTTP_POST_VARS['homepage']   != "") ? sprintf("'%s'", $HTTP_POST_VARS['homepage']) : "NULL";
 $firma      = ($HTTP_POST_VARS['firma']      != "") ? sprintf("'%s'", $HTTP_POST_VARS['firma']) : "NULL";
 $sprache    = ($HTTP_POST_VARS['sprache']    != "") ? sprintf("'%s'", $HTTP_POST_VARS['sprache']) : "NULL";
 $land       = ($HTTP_POST_VARS['land']       != "") ? sprintf("'%s'", $HTTP_POST_VARS['land']) : "NULL";
 $kontaktvon = ($HTTP_POST_VARS['kontaktvon'] != "") ? sprintf("'%s'", $HTTP_POST_VARS['kontaktvon']) : "NULL";
 $username   = ($HTTP_POST_VARS['username']   != "") ? sprintf("'%s'", $HTTP_POST_VARS['username']) : "NULL";
 $passwort   = ($HTTP_POST_VARS['passwort']   != "") ? sprintf("'%s'", $HTTP_POST_VARS['passwort']) : "NULL";

 
 $query = sprintf(" 
 INSERT INTO 
  tblUser
 (Nachname, Vorname, Nickname, Email, Homepage, Firma, Sprache, Land, KontaktVon, KontaktiertAm, Username, Passwort)
  VALUES
 (%s, %s, %s, %s, %s, %s, %s, %s, %s, NOW(), %s, %s)", $nachname, $vorname, $nickname, $email, $homepage, $firma,
                                                       $sprache, $land, $kontaktvon, $username, $passwort);
 $testerDB->query($query);
 $res = Array();
 exec("/usr/bin/htpasswd -b /home/linktrai/.htpasswd ".$HTTP_POST_VARS['username']." ".$HTTP_POST_VARS['passwort'], $res);
 echo("Htpasswd returned: <pre>".implode("\n", $res)."</pre>");
}

function get_passwords(){
 $res = Array();
 exec("/usr/local/bin/apg -L -a0 -n10", $res);
 return implode("\n", $res);
}

function print_form(){ 
 global $PHP_SELF;
?> 
<html>
<head>

<LINK REL=stylesheet HREF="/styles/trail_1.css" TYPE="text/css">

<body>
 <h1>Alpha-Tester-Form</h1>
<p>
Willkommen zum Alphatest-Add-Formular. Bitte Geben Sie unten die Daten
des Testers ein.
<p>
Vorgeschlagene Passwörter:
<pre>
<?= get_passwords(); ?>
</pre>

<form action="<?= $PHP_SELF ?>" method="POST">
 <table border="0">
  <tr>
   <td><b>Nachname</b></td>
   <td><input type="text" name="nachname" size="25" maxlength="80"></td>
  </tr>
  <tr>
   <td><b>Vorname</b></td>
   <td><input type="text" name="vorname" size="25" maxlength="80"></td>
  </tr>
  <tr>
   <td><b>Nickname</b></td>
   <td><input type="text" name="nickname" size="25" maxlength="80"></td>
  </tr>
  <tr>
   <td><b>Email</b></td>
   <td><input type="text" name="email" size="25" maxlength="80"></td>
  </tr>
  <tr>
   <td><b>Homepage</b></td>
   <td><input type="text" name="homepage" size="25" maxlength="80"></td>
  </tr>
  <tr>
   <td><b>Firma</b></td>
   <td><input type="text" name="firma" size="25" maxlength="80"></td>
  </tr>
  <tr>
   <td><b>Sprache</b></td>
   <td>
    <select name="sprache" size="1">
		<option value="de" SELECTED>de</option>
		<option value="en">en</option>
    </select>
   </td>
  </tr>
  <tr>
   <td><b>Land</b></td>
   <td><input type="text" name="land" size="25" maxlength="80"></td>
  </tr>
  <tr>
   <td><b>KontaktVon</b></td>
   <td><input type="text" name="kontaktvon" size="25" maxlength="80"></td>
  </tr>
  <tr>
   <td><b>Username</b></td>
   <td><input type="text" name="username" size="25" maxlength="80"></td>
  </tr>
  <tr>
   <td><b>Passwort</b></td>
   <td><input type="text" name="passwort" size="25" maxlength="80"></td>
  </tr>
  <tr>
   <td colspan="2"><input type="checkbox" name="do_send_mail" value="on" id="1"><label for="1">Send email</label></td>
  </tr>
  <tr>
   <td colspan="2"><p>&nbsp;<center><input type="submit" name="submit"></center></td>
  </tr>
 </table> 
</form>
</body>
</html> 
<? } 

function send_welcome_mail($name, $vorname, $email, $username, $passwort){

$msg = "Liebe/r $vorname $name

Wir freuen uns, dass wir Ihnen die Zugangsdaten für linktrail zusenden
dürfen:

http://preview.linktrail.com
Username: $username
Passwort: $passwort

Um was es sich bei linktrail handelt und was Sie alles mit unseren 
Services anstellen können, erfahren Sie am einfachsten mit unserer Tour.
Sie können diese bequem über die Navigation \"I want to\" oder über den 
direkten Link (http://preview.linktrail.com/Tour/) erreichen.

Durch das Aktivieren Ihres obenerwähnten Accounts erklären Sie, dass
Sie die Vertraulichkeitsvereinbarung (weiter unten in diesem Mail oder
auf http://www.linktrail.com/confidential_de.html) gelesen, verstanden
und akzeptiert haben.

Bugs und/oder Kommentare können Sie einfach über Mail an 
preview@linktrail.com oder bugs@linktrail.com senden. Wir versprechen 
eine speditive und vertrauliche Behandlung.

Und nun möchten wir Sie nicht länger aufhalten und wünschen Ihnen viel
Spass mit linktrail.

Ihr linktrail Team

--->

Vertraulichkeitsvereinbarung:

Sie erhalten hiermit die Erlaubnis, sich am Preview des Dienstes 
linktrail der Firma linktrail Inc. zu beteiligen.

Der Dienst linktrail, der im Speziellen die linktrail-Webseite, die 
Geschäftsidee, das Design und die darauf enthaltenen Inhalte umfasst, 
gilt für die Dauer des Vorab-Tests als vertraulich. Der Dienst 
linktrail, umfassend die oben aufgeführten Komponenten, wird 
ausschliesslich und nur für den Zweck des Vorab-Tests für Sie zugänglich
gemacht.

Sie verpflichten sich hiermit, die von linktrail Inc. unter dieser 
Vertraulichkeitserklärung bezogene Information unter keinen Umständen 
ohne die schriftliche Genehmigung von linktrail Inc. an andere Parteien
weiterzugeben.

Im Speziellen verpflichten Sie sich, die von linktrail Inc. bezogenen 
Zugangsdaten (Benutzername und Passwort) unter keinen Umständen an 
Parteien weiterzugeben, die nicht am Vorab-Test beteiligt sind.

Sie verpflichten sich hiermit, die von linktrail Inc. während der Dauer
des Vorab-Tests bezogene Information in gleichem Masse vertraulich zu 
behandeln, wie Sie dies mit Information tut, die Sie für sich als 
vertraulich bezeichnen.

Durch das Aktivieren des in diesem Mail erwähnten Accounts erklären Sie,
diese Vertraulichkeitserklärung gelesen, verstanden und akzeptiert 
zu haben. 

<---
"; 

$headers = "";
$headers .= "From: linktrail team <preview@linktrail.com>\n";
$headers .= "Cc: pilif@linktrail.com\n";
//$headers .= "To: \n";
$headers .= "MIME-Version: 1.0\n";
$headers .= "Content-Type: text/plain;\n";
$headers .= "              charset=\"ISO-8859-1\"\n";

mail ("$vorname $name <$email>", "preview.linktrail.com", $msg, $headers);
}
?>
