<?php
define("SQL_TOPEX_INC", true);
/*
Contains SQL-Statements used for getting the Top-Experts of a category
*/

//this statement is incomplete. I will insert (id1, id2, id3, id4, id5, id6,...)
define("GET_SUBSCRIBERSU",
"SELECT 
  ltrSubscriptions.Username, count(*) as cnt  
 FROM
  ltrSubscriptions, auth_user, ltrUserData
 WHERE
  Trail IN #I
 AND
  auth_user.username = ltrSubscriptions.Username
 AND
  ltrUserData.User_ID = auth_user.user_id
GROUP BY ltrSubscriptions.Username
ORDER BY
  cnt DESC, ltrUserData.LastOnline DESC
");

define("GET_SUBSCRIBERSL",
"SELECT 
  ltrSubscriptions.Username, count(*) as cnt  
 FROM
  ltrSubscriptions, auth_user, ltrUserData
 WHERE
  Trail IN #I
 AND
  auth_user.username = ltrSubscriptions.Username
 AND
  ltrUserData.User_ID = auth_user.user_id
GROUP BY ltrSubscriptions.Username
ORDER BY
  cnt DESC, ltrUserData.LastOnline DESC
LIMIT #I
");

define("GET_DESCRIPTION","
SELECT
 Description
FROM
 ltrUserData, auth_user
WHERE
 ltrUserData.User_ID = auth_user.user_id
AND
 auth_user.username = #S
");
?>