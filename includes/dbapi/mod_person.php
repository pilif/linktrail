<!-- person.php
  routines for manipulation of person objects
-->

<?php

include("mod_struct.php");

//-----------------------------------------------------------------------
//  constants to use with person_get_attributes
//
define("PERSON_FIELD_NAME", 0);
define("PERSON_FIELD_PASSWORD", 1);
define("PERSON_FIELD_EMAIL", 2);

//-----------------------------------------------------------------------
//  constants to use with person_get_name_list and
//                        person_get_name_list
//
define("PERSON_LIST_FIELD_ID",        0);
define("PERSON_LIST_FIELD_NAME",      1);
define("PERSON_LIST_FIELD_PASSWORD",  2);
define("PERSON_LIST_FIELD_EMAIL",     3);
define("PERSON_LIST_FIELD_DATE",      5);

//-----------------------------------------------------------------------
// person_new
//   create a new person object
//
//   input :
//     $sName
//     $sPassword
//     $sEMail
//   output
//     $lID  ID of new person or
//           negative value on error
//
function person_new($sName, $sPassword, $sEMail) {
  $aArguments[STRUCT_NEW_FIELD_TYPE] = 1;
  $aArguments[STRUCT_NEW_FIELD_NAME] = $sName;
  $aArguments[STRUCT_NEW_FIELD_DESC] = $sPassword;
  $aArguments[STRUCT_NEW_FIELD_UACC] = 0;
  $aArguments[STRUCT_NEW_FIELD_FACC] = 0;
  $aArguments[STRUCT_NEW_FIELD_EXTL] = 0;
  $aArguments[STRUCT_NEW_FIELD_EXTS] = $sEMail;

  return struct_new($aArguments);
}


//-----------------------------------------------------------------------
// person_delete
//   delete a person object
//
//   input :
//     $lID         : ID of person to delete
//   output
//     0   :  success
//    -1   :  failure
//
function person_delete($lID) {
  return struct_delete($lID);
}

//-----------------------------------------------------------------------
// person_get_attributes
//   gets a person's attributes
//
//   input :
//     $lID         : ID of person to delete
//   output
//     $aAttributes :  Array of attributes (Name, password, email)
//     or empty var on erry
//
function person_get_attributes($lID) {
  $aStructAttributes = struct_get_attributes($lID);
  if ($aStructAtttributes) {
    $aAttributes[PERSON_FIELD_NAME]     = $aStructAttributes[STRUCT_FIELD_NAME];
    $aAttributes[PERSON_FIELD_PASSWORD] = $aStructAttributes[STRUCT_FIELD_DESC];
    $aAttributes[PERSON_FIELD_EMAIL]    = $aStructAttributes[STRUCT_FIELD_EXTS];
  } else {
    $aAtributes = $aStructAttributes;
  }
  return $aAttributes;
}

//-----------------------------------------------------------------------
// person_set_attributes
//   sets a person's attributes
//
//   input :
//     $lID         : ID of person to delete
//     $sName
//     $sPassword
//     $sEMail
//   output
//     0  on success
//    -1  on failure
//
function person_set_attributes($lID, $sName, $sPassword, $sEMail) {
  $aArguments[STRUCT_SETATTS_FIELD_NAME] = $sName;
  $aArguments[STRUCT_SETATTS_FIELD_DESC] = $sPassword;
  $aArguments[STRUCT_SETATTS_FIELD_UACC] = 0;
  $aArguments[STRUCT_SETATTS_FIELD_FACC] = 0;
  $aArguments[STRUCT_SETATTS_FIELD_EXTL] = 0;
  $aArguments[STRUCT_SETATTS_FIELD_EXTS] = $sEMail;
  $aArguments[STRUCT_SETATTS_FIELD_ID]   = $lID;

  return struct_set_attributes($lID, $aArguments);
}


//-----------------------------------------------------------------------
// person_get_by_name
//   find a person object using its name
//
//   input :
//     $sName : user id (should be unique)
//   output
//     $lID  ID of person or
//           negative value if not found
//
function person_get_by_name($sName) {
  return struct_get_by_name($sName);
}

//-----------------------------------------------------------------------
// person_get_name_list
//   get a list of persons
//
//   input :
//      - none - 
//   output
//     $aaRows : array of arrays of (ID, Name)
//        or empty on failure
//
function person_get_name_list() {
  $aaStructRows = struct_get_list(TYPE_PERSON);
  if ($aaStructRows) {
   $i = 0;
   while ($i < sizeof($aaStructRows)) {
	   $aaRows[$i] = Array($aaStructRows[$i][STRUCT_FIELD_ID],
                         $aaStructRows[$i][STRUCT_FIELD_NAME]);
     $i = $i + 1;
   }

  } else {
    $aaRows = $aaStructRows;
  }
  return $aaRows;
}


//-----------------------------------------------------------------------
// person_get_full_list
//   get a list of persons
//
//   input :
//      - none - 
//   output
//     $aaRows : array of attribute-arrays (ID, Name, Password, EMail, Date)
//        or empty on failure
//
function person_get_full_list() {
  $aaStructRows = struct_get_list(TYPE_PERSON);
  if ($aaStructRows) {
   $i = 0;
   while ($i < sizeof($aaStructRows)) {
	   $aaRows[$i] = Array($aaStructRows[$i][STRUCT_FIELD_ID],
                         $aaStructRows[$i][STRUCT_FIELD_NAME],
                         $aaStructRows[$i][STRUCT_FIELD_DESC],
                         $aaStructRows[$i][STRUCT_FIELD_EXTS],
                         $aaStructRows[$i][STRUCT_FIELD_DATE]
                         );
     $i = $i + 1;
   }

  } else {
    $aaRows = $aaStructRows;
  }
  return $aaRows;
}




?>
