<!-- dbopen.php
     opens a mysql-connection with the predefined
     server, user and password
-->
     
<?php    
  $myDB = new DB_Linktrail;
  
  $sServer = "linktrail.com";
  $sUser   = "root";
  $sPass   = "oelberg";                    
  
  $conn = mysql_connect($server, $user, $pass);
  if ($conn != 0) {
    $iSelect = mysql_select_db("linktrail", $conn);
//    echo "<B> Connection succeded </B>handle : $conn";
  } else {
    echo "<B> Connection failed </B>"; 
  }
?>