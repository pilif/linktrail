<?php
define("SQL_MSGLIST_INC", true);
/*
Contains SQL-Statements used for getting the Messages for a user,
grouped by trail where necessary
*/

/* Create a temp. table. Get all the Messages that are not grouped */
define("CREATE_AND_FILL_MSG","
CREATE TEMPORARY TABLE #I
 SELECT 
  *
 FROM 
  ltrMessages 
 WHERE 
    (Trail IS NULL)
 AND
    (Target = #S)
");


/* 
 Fill in the grouped user notifications. GROUP BY reduces the
 messages to one; the newest.
*/
define("INSERT_GROUPED_MSG", "
INSERT INTO #I
 SELECT
  *
 FROM 
  ltrMessages
 WHERE
  Target = #S
 AND
  Trail is not NULL
 GROUP BY 
  Trail
 ORDER BY Date DESC 
");

/* Now we are reading all the messages, ordered by date and limited by count */
define("GET_MESSAGES_EX", "
SELECT 
 * 
FROM 
 #I
ORDER BY
 #I DESC
LIMIT #I, ".MSG_MAX_MSGS
);

define("GET_MESSAGES_EX_UL", "
SELECT 
 * 
FROM 
 #I
ORDER BY
 #I DESC
");

define("COUNT_RESULTS_MSG", "SELECT count(*) as cnt FROM #I");

/* Drop der Tabelle */
define("DROP_TEMP_MSG", "DROP TABLE #I");

?>