<?php
/**
* API for setting scores of elements
*
* This module contains some low-level database functions for working 
* with the scoring-system and the self-regulation.
*
* @module       scoring
* @modulegroup  dbapi
* 
*/
 define("SCORING_INC", true);

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

define("BT_SUBSCRIBE", 20);
define("BT_UNSUBSCRIBE", -20);
define("BT_SUGGEST", 10);
define("BT_UNSUGGEST", -10);
  
/**
* Getting the avarage points of all trails in a node
*
* Every new trail gets as its initial point-value the avarage points of
* all nodes (directely) in a trail or 10'000 if this avarage is 0
* (the node is empty)
*
* @param    integer Node_ID of parent node of trail-object
* @return   avaarage of points in given node
*/
function get_avg_points($node_id){
 global $myDB;
 
 $query = FormatSQL(SQL_AVG_POINTS, (array)($node_id));
 $myDB->query($query);
 $myDB->next_record();
 $avg = ($myDB->f("points") != 0) ? $myDB->f("points") : 1000;
 return $avg;
}

/**
* Adding a bonus to a trail
*
* This function is used for adding a point-bonus to a trail. The point-values
* can be found in sql_strs.inc
*
* @param    integer Node_ID of trail to change
* @param    integer type of change
* @return   true (dies on db-error)
*/
function add_bonus($node_id, $type){
 global $myDB;
 $count['subs'] = get_subscription_count($node_id);
 $count['sugs'] = get_suggestion_count($node_id);

 $query = FormatSQL(SQL_BONUS_POINTS, array($count['subs'], $count['sugs'], $type, $node_id));
 $myDB->query($query);
 
 return true;
}

function get_subscription_count($node_id){
 global $myDB;

 $query = FormatSQL(SQL_SUBSCRIPTION_COUNT, array($node_id));
 $myDB->query($query);
 $myDB->next_record();
 return ($myDB->f("count") > 0) ? ($myDB->f("count") -1 ) : 0; //dont' add bonus for the newly created subscriptoin
}

function get_suggestion_count($node_id){
 global $myDB;

 $query = FormatSQL(SQL_SUGGESTION_COUNT, array($node_id));
 $myDB->query($query);
 $myDB->next_record();
 return ($myDB->f("count") > 0) ? ($myDB->f("count") -1 ) : 0;
}

function set_cat_points($node_id, $points){
 global $myDB;
 $query = FormatSQL(SET_KAT_POINTS, array($points, $node_id));
 return  $myDB->query($query); 
}