<?php
define("SQL_MSGLIST_OUT_INC", true);
/*
Contains SQL-Statements used for getting the Messages for a user,
grouped by trail where necessary
*/


define("GET_MESSAGES_EX", "
SELECT 
 *, 0 as inbox, auth_user.username as to_name 
FROM 
 ltrMessages
LEFT JOIN auth_user ON ltrMessages.Target = auth_user.user_id
WHERE
 (Sender = #S)
AND
 ( ((ReferenceCount & 2) != 0) )
ORDER BY
 #I DESC
LIMIT #I, ".MSG_MAX_MSGS
);

define("GET_MESSAGES_EX_UL", "
SELECT 
 *, 0 as inbox, auth_user.username as to_name 
FROM 
 ltrMessages
LEFT JOIN auth_user ON ltrMessages.Target = auth_user.user_id
FROM 
 ltrMessages
WHERE
 (Sender = #S)
AND
 ( ((ReferenceCount & 2) != 0) )
ORDER BY
 #I DESC
");

define("COUNT_RESULTS_MSG", "SELECT count(*) as cnt FROM ltrMessages WHERE Sender = #S");
?>