<?php
page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm"));

    // default search server
    define("DEF_SEARCH_SERVER", "localhost");
    // default sarch port
    define("DEF_SEARCH_PORT", 7777);
    // divisor to bring highest byte down
    define("HI_DIV",      16777216);
    // divisor to bring mid-high byte down
    define("HIMID_DIV",   16);
    // divisor to bring mid-high byte down
    define("LOMID_SHIFT",  8);
    // mask for mid-high byte
    define("LOMID_MASK",  0x0000ff00);
    // mask for  byte
    define("BYTE_MASK",   0x000000ff);
    
    
    
    function FormatSearchExpr($sSearchExpr, $table_type="c", $ci=0, $rc=5, $restriction="/", $date="", $language="") {
      global $sess;
      srand ((double)microtime()*1000000);
      $randval = rand(11111,99999);
      $str = md5(uniqid($randval)).'|'.'A'.'|'.$table_type.'|'.$language.'|'.$date.'|'.$ci.'|'.$rc.'|'.$restriction.'|'.$sSearchExpr;
      echo("<p>Query: <tt>$str</tt><p>");
      return  $str . chr(0);
    }
?>

<!-- ------------------------------------------------------------ -->
<html>
<body bgcolor="gray" text="black" link="lime" vlink="white" alink="yellow">
<?php if (isset($txtSearchExpr)) { 
    echo "SearchExpr   is $txtSearchExpr   <BR>";
    echo "SearchServer is $txtSearchServer <BR>";
    echo "SearchPort   is $txtSearchPort   <BR>";
    $sFormattedSearch = FormatSearchExpr($txtSearchExpr, $lstTableType, $txtCurrentIndex, $txtResultCount, $txtRestriction, $txtDate, $txtLanguage);
    $fp = fsockopen($txtSearchServer, $txtSearchPort);
    if (!$fp) {
        echo "Could not connect to search-server<br>\n";
    } else {
      echo "connected <BR>";
      flush();
      $iResult = fputs ($fp, $sFormattedSearch);
      echo "<br>written $iResult chars<BR>";
      echo("<hr>");
      $header = Array();
      $header_done = false;
      $count = 0;
      while (!feof($fp)) {
       $erg = fgets($fp,1024);
       if ( !$header_done and ($erg != "\n") ){
        $header[] = $erg;
       }elseif ( !$header_done and ($erg == "\n") ){
        !$header_done = true;
       }else{
        $content[] = $erg;
       }
       $count++; 
      }
    echo("<p>"); 
    }
  printf("Header-Count: %d; Content-Count: %d<p>", count($header), count($content));
  echo("<hr width=\"25%\"><center>Header:</center><hr width=\"25%\">");
  echo(implode("<br>", (array)$header));
  echo("<p");
  echo("<hr width=\"25%\"><center>Content:</center><hr width=\"25%\">");
  echo(implode("<br>", (array)$content));
  }else{ ?>
<form action="<?= $PHP_SELF ?>" method="GET">
<table>
  <tr>
    <td>
      Search Expression
    </td>
    <td>
      &nbsp;
    </td>
  </tr>  
  <tr>
    <td>
      <INPUT type=text name=txtSearchExpr>
    </td>
    <td>  
      <INPUT type=submit value="Search!" name=butSearch>
    </td>
  </tr>
  <tr>
    <td COLSPAN=2>
      &nbsp;
    </td>
  </tr>
  <tr>
    <td>
      <span style="background-color: #ffff66"><b>Search</b></span> Server
    </td>
    <td>
      Search Port
    </td>
  </tr>  
  <tr>
    <td>
      <INPUT name="txtSearchServer" value = "<?= DEF_SEARCH_SERVER?>" >
    </td>
    <td>
      <INPUT name="txtSearchPort" value = "<?= DEF_SEARCH_PORT?>" >
    </td>
  </tr>  

  <tr>
    <td>
      CurrentIndex
    </td>
    <td>
      ResultCount
    </td>
  </tr>  
  <tr>
    <td>
      <INPUT name="txtCurrentIndex" value = "0" >
    </td>
    <td>
      <INPUT name="txtResultCount" value = "5" >
    </td>
  </tr>  

   <tr>
    <td>
      TableType
    </td>
    <td>
     Category Restriction
    </td>
  </tr>  
  <tr>
    <td>
    <select name="lstTableType" size="1">
	 <option value="c" SELECTED>Category</option>
	 <option value="t">trail</option>
	 <option value="e">Experts</option>
    </select>
    </td>
    <td>
      <INPUT name="txtRestriction" size="20" value="/">
    </td>
  </tr>  

  <tr>
    <td>
      Date
    </td>
    <td>
     Language
    </td>
  </tr>  
  <tr>
    <td>
      <INPUT name="txtDate" value = "&lt;<?php echo(strftime("%Y-%m-%d")); ?>" >
    </td>
    <td>
      <INPUT name="txtLanguage" size="20" value="3">
    </td>
  </tr>  
 
</table>
</form>
<? } ?>
</body>
</html>
<?php page_close(); ?>