<?php
define("SQL_MESSAGES_INC", true);
/*
 SQL-Statments for sending and listing messages
*/

define("GET_MESSAGES", "
SELECT
 *
FROM
 ltrMessages
WHERE
 Target = #S
ORDER BY Date DESC
LIMIT #I,".MSG_MAX_MSGS);

define("GET_REPORT", "
SELECT 
 *
FROM
 ltrMessages
WHERE 
 Urgency = 'r'
AND
 Target = #S
AND
 Date <= #S
ORDER BY Date DESC");

define("DELETE_MESSAGE", "
 DELETE FROM 
  ltrMessages
 WHERE 
  Message_ID = #I");

define("DELETE_BY_DATE", "
 DELETE FROM
  ltrMessages
 WHERE
  Date <= #S
 AND
  Target = #I");
  
define("SEND_MESSAGE", "
 INSERT INTO
  ltrMessages
  (Target, MessageType, Sender, Date, Urgency, Done, Data, Trail)
 VALUES
  (#S, #I, #S, NOW(), #S, 'n', #S, #I)");

define("SET_DONE", "
 UPDATE 
  ltrMessages
 SET 
  Done = #S
 WHERE
  Message_ID = #I");
  
define("GET_ONE_MESSAGE","
 SELECT
  *
 FROM
  ltrMessages
 WHERE
  Message_ID = #I");  

define("NEW_MSG_COUNT", "
select 
 count(*) as cnt
from 
 ltrMessages
WHERE
 UNIX_TIMESTAMP(Date) > UNIX_TIMESTAMP(#S)
AND
 Target = #S
AND
 MessageType IN (1001, 1002, 1003, 1008, 1009, 1011, 1012, 1014, 1015, 1016, 1017, 1018, 1019, 1021) 
");

define("MSG_READ_CHILD","
SELECT
 *
FROM
 ltrMessages
WHERE
 Target = #S
AND
 Trail = #I
AND
 Message_ID != #I 
ORDER BY
 Date DESC");

define("MSG_KILL_CHILDREN","
DELETE FROM
 ltrMessages
WHERE
 Target = #S
AND
 Trail = #I ");
 
define("SQL_UPDATE_READ", "
UPDATE
 ltrUserData
SET
 LastReadMessages = #S
WHERE
 User_ID = #S
"); 

define("SQL_READ_REQUESTS", "
SELECT
 ltrMessages.Message_ID
FROM
 ltrMessages, auth_user
WHERE
( ltrMessages.MessageType = 1011) 
AND ((
  ltrMessages.Target = #S /*myself*/
 AND
  ltrMessages.Sender = auth_user.username
 AND
  auth_user.user_id = #S /*the one that requested*/
) OR (
  ltrMessages.Target = #S /*the one I am asking*/
 AND
  ltrMessages.Sender = auth_user.username
 AND
  auth_user.user_id = #S /*myself*/
))
");

define("SQL_DEL_REQUESTS", "DELETE FROM ltrMessages WHERE Message_ID IN (#I)");
?>