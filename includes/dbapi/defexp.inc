<?php
 define("DEFEXP_INC", true);

 /*
   The following is common to all DBAPI-Units. The DB-Connection 
   will only be opened once
 */

 global $myDB;


 if (!is_object($myDB)) {
  $myDB = new DB_Linktrail;
  include("dbapi/sql_strs.inc");
  include("dbapi/sql_util.inc");
 }

 if (!defined("COMUTILS_INC"))
  include("dbapi/comutils.inc");

 if (!defined("SQL_DEFEXP_INC"))
  include("dbapi/sql_defexp.inc");

function get_def_expert($node_id){
 global $myDB;

 $query = FormatSQL(GET_EXPERT, array($node_id));
 $myDB->query($query);
 if ($myDB->num_rows() == 0)
  return "";
 $myDB->next_record();
 $hsh['Name'] = $myDB->f("Username");
 $hsh['About'] = $myDB->f("Description");
 return $hsh;
}

function is_valid_expert($username){
 global $myDB;

 $query = FormatSQL(VALID_EXPERT, array($username));
 $myDB->query($query);
 return ($myDB->num_rows() != 0);
}

//This is a low-level function. To make error-handling easier,
//the function assumes a valid username
function add_expert($node_id, $username, $about){
 global $myDB;

 $query = FormatSQL(ADD_EXPERT, array($node_id, $username, $about));
 $myDB->query($query);
 return true;
}

//This is a low-level function. To make error-handling easier,
//the function assumes a valid username
function edit_expert($node_id, $username, $about){
 global $myDB;

 $query = FormatSQL(EDIT_EXPERT, array($username, $about, $node_id));
 $myDB->query($query);
 return true;
}

function delete_expert($node_id){
 global $myDB;

 $query = FormatSQL(DELETE_EXPERT, array($node_id));
 $myDB->query($query);
 return true;
}
