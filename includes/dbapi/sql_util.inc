<?php
 define("SQLUTIL_INC", true);
  //---------------------------------------------------------------------------
  // FormatSQL
  //   formats a string by substituting markers by values in array
  //    #L denotes a  long      - substitute by next array element
  //    #I denotes an int       - substitute by next array element
  //    #S denotes a  string    - substitute by next array element enclosed in quotes
  //    #D denotes a  date      - substitute by current date (YYYYMMDDhhmmss, we use a string...)
  //    ## denotes a '#'        - substitute by '-'
  //    other combinations      - substitute by '???'
  //   if array too small substitutions become '!!!'                
  //                                                      
  //   note :
  //     strpos delivers an empty string if character doesn't appear in string
  //     substr delivers rest of string if second argument omitted
  //
  function FormatSQL($sSQLMask, $aValues) {
    $sMask = $sSQLMask;
    $sSQL = "";
    $iStartPos = 0;          
    $iPos      = 1;
    $iIndex    = 0;
    $iNum      = sizeof($aValues);
    $sSubst    = "";

//  echo "sMask : $sMask <br>";
         
    while (($iPos > 0) && (strlen($sMask) > 0)) {
      $iLen      = strlen($sMask);
      $iPos = strpos($sMask, "#");
      if (strlen($iPos) > 0) {    
        if ($iPos < $iLen-1) { 
          $cCur = substr($sMask, $iPos+1, 1);      
          $sSubst    = "!!!";
//  echo "cCur : $cCur, iIndex : $iIndex, val[0] : " . $aValues[0] . ", val[1] : " . $aValues[1] . ", val[2] : " . $aValues[2] . " <br>";

          switch ($cCur) {
            case "#" :
              $sSubst = "#";
              $iIncrement = 0;
              break;
            case "L" :
            case "I" :    
              if ($iIndex < $iNum) {
                $sSubst = $aValues[$iIndex];
              }
              $iIncrement = 1;
              break;
            case "S" :    
              if ($iIndex < $iNum) {
                $sSubst = "'" . $aValues[$iIndex] . "'";
              }
              $iIncrement = 1;
              break;
            case "D" :
              $sSubst =  date("YmdHis");
              $iIncrement = 0;
              break;
            default :
              $sSubst = "???";
              $iIncrement = 0;    
              break;
          }
//    echo "sSubst : $sSubst <br>";
          $sSQL = $sSQL . substr($sMask, 0, $iPos) . $sSubst;
        }  
      } else {
        $sSQL = $sSQL . substr($sMask, $iPos);     
        $iPos = 0;
      }                                             
      $sMask = substr($sMask, $iPos+2);
      $iIndex = $iIndex + $iIncrement;
//    echo "iPos  : $iPos <br>";
//    echo "sSQL  : $sSQL <br>";
//    echo "sMask : $sMask <br>";
    }
    return $sSQL;
  }
  
   
?>