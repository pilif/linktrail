<?php
 define("SEARCH_INC", true);
 
 if (!defined("COMUTILS_INC"))
  include("dbapi/comutils.inc");
  
/*
 Somwhere in a later time, this function is not necessaryx anymore since the server
 is then strong enough to parse the string with all it's errors.
*/ 
function format_query($query, $whole_words = false){
 $query =str_replace ("%", "ú", $query);
 $query =str_replace ("*", "%", $query);
 $query =str_replace ("ú", "\%", $query);

 $query =str_replace ("_", "*", $query);
 $query =str_replace ("?", "_", $query);
 $query =str_replace ("*", "\_", $query);
 
 $query =str_replace (" + ", " AND ", $query);
 $query =str_replace (" | ", " OR ", $query);
 $query =str_replace ("+", " AND ", $query);
 $query =str_replace ("|", " OR ", $query);
 

 $tokens    = explode(" ", $query);
 $nopercent = false;
 $query = "";
 for($x=0; $x < count($tokens); $x++){
   if (str_is_in(strtoupper($tokens[$x]), array("AND", "OR", "NOT", '}', '{')) ){
    $query .= ' '. strtoupper($tokens[$x])." ";
   }else{ 
//    printf('<font color="red">x ist: %d; nopercent ist: %s<br></font>', $x, $nopercent); 
    $nospace=false;
    if (!$nopercent){
      $nopercent = (substr($tokens[$x], 0, 1) == '"');
      if ($nopercent){
       $query .= '"%';
       $tokens[$x] = substr($tokens[$x], 1);
       $nospace = true;
      } 
    }
//    printf('<font color="red">x ist: %d; nopercent ist: %s (bei tokens: %s)<br></font>', $x, $nopercent, substr($tokens[$x], 0, 1)); 
    if (!$nopercent){
     $firstchar = substr($tokens[$x], 0, 1);
     $lastchar  = substr($tokens[$x], strlen($tokens[$x])-1, 1);
     $tokens[$x] = str_replace(")", "", $tokens[$x]);
     $tokens[$x] = str_replace("(", "", $tokens[$x]);
     
     if ( ($firstchar == "(") and ($lastchar == ")")){
       $query .= ($whole_words) ? '(% '.$tokens[$x].' %)' : '(%'.$tokens[$x].'%)';
     }elseif( ($firstchar == "(") and ($lastchar != ")") ){
       $query .= ($whole_words) ? '(% '.$tokens[$x].' %' : '(%'.$tokens[$x].'%';
     }elseif( ($firstchar != "(") and ($lastchar == ")") ){
       $query .= ($whole_words) ? '% '.$tokens[$x].' %)' : '%'.$tokens[$x].'%)';
     }else{
       $query .= ($whole_words) ? '% '.$tokens[$x].' %' : '%'.$tokens[$x].'%';
     }
    }else{
//      echo("<br>mache pur: ".$tokens[$x]."<p>");
      if ($nospace) {
        $query .= $tokens[$x];
        $nospace = false;      
      }else{
        $query .= " ".$tokens[$x];
      }
    }  
   $words[] = $tokens[$x];
//   printf('<font color="blue">x ist: %d; nopercent ist: %s<br></font>', $x, $nopercent); 
   if ($nopercent) {
     $nopercent = !(substr($tokens[$x], strlen($tokens[$x])-1, 1) == '"');
     if (!$nopercent){
      $query = substr($query, 0, strlen($query)-1);
      $query .= '%"';
     }
   } 
//   printf('<font color="blue">x ist: %d; nopercent ist: %s (substr: %s)<br></font>', $x, $nopercent, substr($tokens[$x], strlen($tokens[$x])-1, 1)); 
   }
  } 
 $hsh['query'] = $query; 
 $hsh['words'] = $words; 
// die($hsh['query']);
 return $hsh;
} 

function make_query($sSearchExpr, $sessid, $table_type="c", $ci=0, $rc=5, $restriction="/") {
 return $sessid.'|'.'A'.'|'.$table_type.'|'.$ci.'|'.$rc.'|'.$restriction.'|'.$sSearchExpr . chr(0);
}

function parse_header($headers){
 if (!is_array($headers)) return "Invalid header received from Search-Server";
 $hsh = "";
 foreach($headers as $header){
  if (strstr($header, '-')){
   list($errcode, $errstr) = explode(' - ', $header);
   if ($errcode != '0000'){
    $hsh['errcode'] = $errcode;
    $hsh['errstr']  = $errstr;
    return $hsh;
   }
  }
  if (strstr($header, '@')){
   list($rescount, $reqmax) = explode(' @ ', $header);
   list($req, $max) = explode("/", $reqmax);
   $hsh['rescount'] = $rescount;
   $hsh['reqested'] = $req;
   $hsh['max_resl'] = $max;
  }
 } 
 if (is_array($hsh))
  return $hsh;
 else
  return "Search-Server returned invalid header";
}

function parse_results($content, $req_type, $restriction){
 if (!is_array($content)) return array();
 $fertig = Array();
 if ($restriction == "/Experts/"){
  foreach($content as $result){
   list($hsh['uid'], $hsh['username'], $hsh['about']) = explode(' | ', $result);
   $fertig[] = $hsh;
  }
 }elseif( ($restriction != "/Experts/") and ($req_type == "c") ){
  foreach($content as $result){
   list($hsh['id'], $hsh['rank'], $hsh['n'], $hsh['name']) = explode(' | ', $result);
   $fertig[] = $hsh;
  } 
 }elseif( ($restriction != "/Experts/") and ($req_type == "e") ){
  foreach($content as $result){
   list($hsh['uid'], $hsh['rank'], $hsh['n'], $hsh['name']) = explode(' | ', $result);
   $fertig[] = $hsh;
  } 
 }else{
  foreach($content as $result){
   list($hsh['id'], $hsh['rank'], $hsh['n'], $hsh['name'], 
        $hsh['description'], $hsh['username'], $hsh['date']) = explode(' | ', $result);
   $fertig[] = $hsh;
  } 
 }
 return $fertig;
}

function execute_query($formated_query, $session_id, $current_index, $type="c", $restriction="/", $count=""){
 global $sess;
 if ($count == "")
 $count = ($type == "c") ? DIR_MAX_SEARCH : TRAIL_MAX_SEARCH;
// die(SEARCH_HOST." ".SEARCH_PORT);
 $fp = fsockopen(SEARCH_HOST, SEARCH_PORT);
 if (!$fp)
  error("Connection Error", "Could not connect to search-server<br>\n", __LINE__-1, __FILE__);
 $sFormattedSearch = make_query($formated_query, $sess->id, $type, $current_index, $count, $restriction);
  echo("Sent: $sFormattedSearch<br>");
 $iResult = fputs ($fp, $sFormattedSearch);
 $header_done = false;
 while (!feof($fp)) {
  $erg = fgets($fp,1024);
  if ( !$header_done and ($erg != "\n") )
   $header[] = $erg;
  elseif ( !$header_done and ($erg == "\n") )
   !$header_done = true;
  else
   $content[] = $erg;
 }
 $res_hash = parse_header($header);
 if (isset($res_hash['errcode'])){
  $hsh['error'] = $res_hash;
 }else{
  $fertig   = parse_results($content, $c, $restriction);
  $hsh['resinfo'] = $res_hash;
  $hsh['results'] = $fertig;
 }
 return $hsh;
} 

?>