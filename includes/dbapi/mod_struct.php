<!-- SQL_st_struct.php
  a collection of SQL-statements for structobjects
  for documentation see 
    LinkTrailSQL_struct.html
-->




<?php
include("mod_SQL_struct.php");
include("mod_SQL_util.php");

//---------------------------------------------------------------------------
// constants to use for SQL_OBJECT_GET_ATTRIBUTES
//
    define("STRUCT_FIELD_ID",   0);
    define("STRUCT_FIELD_TYPE", 1);
    define("STRUCT_FIELD_NAME", 2);
    define("STRUCT_FIELD_DESC", 3);
    define("STRUCT_FIELD_UACC", 4);
    define("STRUCT_FIELD_FACC", 5);
    define("STRUCT_FIELD_EXTL", 6);
    define("STRUCT_FIELD_EXTS", 7);
    define("STRUCT_FIELD_DATE", 8);
 


//---------------------------------------------------------------------------
// constants to use for SQL_OBJECT_NEW
//
    define("STRUCT_NEW_FIELD_TYPE",0);
    define("STRUCT_NEW_FIELD_NAME",1);
    define("STRUCT_NEW_FIELD_DESC",2);
    define("STRUCT_NEW_FIELD_UACC",3);
    define("STRUCT_NEW_FIELD_FACC",4);
    define("STRUCT_NEW_FIELD_EXTL",5);
    define("STRUCT_NEW_FIELD_EXTS",6);
 
//---------------------------------------------------------------------------
// constants to use for SQL_OBJECT_SET_ATTRIBUITES
//
    define("STRUCT_SETATTS_FIELD_NAME",  0);
    define("STRUCT_SETATTS_FIELD_DESC",  1);
    define("STRUCT_SETATTS_FIELD_UACC",  2);
    define("STRUCT_SETATTS_FIELD_FACC",  3);
    define("STRUCT_SETATTS_FIELD_EXTL",  4);
    define("STRUCT_SETATTS_FIELD_EXTS",  5);
    define("STRUCT_SETATTS_FIELD_ID",    6);


//---------------------------------------------------------------------------
// constants to use for SQL_OBJECT_SET_NUMERIC_ATTRIBUTE
//                  and SQL_OBJECT_SET_STRNIG_ATTRIBUTE 
//
    define("STRUCT_SETATT_FIELD_ATTNAME", 0);
    define("STRUCT_SETATT_FIELD_ATTVAL",  1);
    define("STRUCT_SETATT_FIELD_ID",      2);


//---------------------------------------------------------------------------
// constants to use for SQL_OBJECT_SET_USER_ACCESS
//                  and SQL_OBJECT_SET_FRIEND_ACCESS
//
    define("STRUCT_ACC_FIELD_ACCVAL", 0);
    define("STRUCT_ACC_FIELD_ID",  1);


//--------------------------------------------
// struct_new
//   create a struct object
// input
//   $aArguments  : Array of arguments
// output :
//   0    : success
//  -1    : failure
//
function struct_new($aArguments) {
// echo "in struct_new func " . implode("|", $aArguments) . "<br>";
  $sSQL = FormatSQL(SQL_OBJECT_NEW, $aArguments);
  $iResult = mysql_query($sSQL);
	if ($iResult) {
	  $iResult = 0;
	} else {
    $iResult = -1;
	}
	return $iResult;
}



//--------------------------------------------
// struct_delete
//   delete a struct object
// input
//   $lID  : ID of object to delete
// output :
//   0    : success
//  -1    : failure
//
function struct_delete($lID) {
echo "in struct_delete func<br>";
  $aArguments[0] = $lID;
  $sSQL = FormatSQL(SQL_OBJECT_DELETE, $aArguments);
  $iResult = mysql_query($sSQL);
	if ($iResult) {
	  $iResult = 0;
	} else {
    $iResult = -1;
	}
	return $iResult;
}


//--------------------------------------------
// struct_get_attributes
//   get an object's attributes
// input
//   $lID  : ID of object to delete
// output :
//   $aAttributes : array of attributes
//   or empty on failure
//
function struct_get_attributes($lID) {
  $aArguments[0] = $lID;
  $sSQL = FormatSQL(SQL_OBJECT_GET_ATTRIBUTES, $aArguments);
  $iResult = mysql_query($sSQL);
	if ($iResult) {
	  $i = 0;
    // ther should be at most 1 result
	  $aAttributes = mysql_fetch_array($iResult);
	}
	return $aAttributes;
}


//--------------------------------------------
// struct_set_attributes
//   set an object's attributes
// input
//   $lID  : ID of object to delete
//   $aAttributes : array of attributes
// output :
//   0    : success
//  -1    : failure
//
function struct_set_attributes($lID, $aArguments) {
  $sSQL = FormatSQL(SQL_OBJECT_SET_ATTRIBUTES, $aArguments);
  $iResult = mysql_query($sSQL);
	if ($iResult) {
	  $iResult = 0;
	} else {
    $iResult = -1;
    echo "Err <br>";
	}
	return $iResult;

}

//-----------------------------------------------------------------------
// struct_get_by_name
//   find a struct object using its name
//
//   input :
//     $sName : name of object
//   output
//     $lID  ID of person or
//           negative value if not found
//
function struct_get_by_name($sName) {
  $lID = -1;
  $aArguments[0] = $sName;
  $sSQL = FormatSQL(SQL_OBJECT_GET_BY_NAME, $aArguments);
  $iResult = mysql_query($sSQL);
	if ($iResult) {
	  $i = 0;
    // there should be at most 1 result
	  $aAttributes = mysql_fetch_array($iResult);
    if ($aAttributes) {
      $lID = $aAttributes[STRUCT_FIELD_ID];
    }
	}
	return $lID;
}


//-----------------------------------------------------------------------
// struct_get_list
//   get a list of struct objects of desired type(s)
//
//   input :
//     $iType : any combination of TYPE_XXXX constants
//   output
//     $aaRows : array of attribute-arrays 
//        or empty on failure
//
function struct_get_list($iTypes) {
  $aArguments[0] = $iTypes;
  $sSQL = FormatSQL(SQL_TYPE_OBJECTS, $aArguments);
  $iResult = mysql_query($sSQL);
	if ($iResult) {
	  $i = 0;
	  while ($aRow = mysql_fetch_array($iResult)) {
	    $aaRows[$i] = $aRow;
      $i = $i + 1;
	  }
	}
	return $aaRows;
}




?>

