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
  (Target, MessageType, Sender, Date, Done, Data, ReferenceCount)
 VALUES
  (#S, #I, #S, NOW(), 'n', #S, #I)");

define("SET_DONE", "
 UPDATE 
  ltrMessages
 SET 
  Done = #S
 WHERE
  Message_ID = #I");
  
define("GET_ONE_MESSAGE","
 SELECT
   *, #I as inbox, auth_user.username as to_name 
 FROM
  ltrMessages
 LEFT JOIN auth_user ON ltrMessages.Target = auth_user.user_id
 WHERE
  Message_ID = #I");  

define("NEW_MSG_COUNT", "
select 
 count(*) as cnt
from 
 ltrMessages
WHERE
 Done = 'n'
AND
 Target = #S
AND
 MessageType IN (1001, 1002, 1003, 1008, 1009, 1011, 1012, 1014, 1015, 1016, 1017, 1018, 1019, 1021) 
AND 
 ( ((ReferenceCount & 1) != 0) )
");


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

define("SQL_UPDATE_RC","UPDATE ltrMessages Set ReferenceCount = #I WHERE Message_ID = #I");

define("SQL_UPDATE_DATA", "UPDATE ltrMessages SET Data = #S WHERE Message_ID = #I");

define("SQL_SET_ACTIVE", "UPDATE ltrMessages SET Active = #S WHERE Message_ID = #I");
?>