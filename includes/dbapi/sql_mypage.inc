<?php
define("SQL_MYPAGE_INC", true);
/*
Contains SQL-Statements to get the list of trails on the mypage
*/

/* create TT and fill possesed trails */
define("CREATE_AND_FILL_MYPAGE","
CREATE TEMPORARY TABLE #I
 SELECT 
 ltrDirectoryData.AddDate, ltrDirectoryData.ChangeDate, 
 ltrDirectory.Name, ltrDirectory.Node_ID, ltrDirectory.Language,
 0 as SubscribedToTrail,
 tbl2.Name as parent,
 RIGHT(ltrDirectory.Name, INSTR(MID(REVERSE(ltrDirectory.Name),2,LENGTH(ltrDirectory.Name)) ,'/')) as friendlyname_trail,
 RIGHT(tbl2.Name, INSTR(MID(REVERSE(tbl2.Name),2,LENGTH(tbl2.Name)) ,'/')) as friendlyname_parent
 FROM 
 ltrDirectoryData, ltrDirectory, ltrDirectory as tbl2
where
 ( ltrDirectoryData.owner = #S)
AND
 ( tbl2.Node_ID = ltrDirectory.Parent)
AND
 ltrDirectory.Node_ID = ltrDirectoryData.Node_ID
");


/* insert subscribed */
define("INSERT_SUBSCRIBED", "
INSERT INTO #I
SELECT 
 ltrDirectoryData.AddDate, ltrDirectoryData.ChangeDate, 
 ltrDirectory.Name, ltrDirectory.Node_ID, ltrDirectory.Language,
 ltrSubscriptions.Trail as SubscribedToTrail,
 tbl2.Name as parent,
 RIGHT(ltrDirectory.Name, INSTR(MID(REVERSE(ltrDirectory.Name),2,LENGTH(ltrDirectory.Name)) ,'/')) as friendlyname_trail,
 RIGHT(tbl2.Name, INSTR(MID(REVERSE(tbl2.Name),2,LENGTH(tbl2.Name)) ,'/')) as friendlyname_parent
FROM 
 ltrDirectoryData, ltrDirectory, ltrDirectory as tbl2, ltrSubscriptions
where
 ( ltrSubscriptions.Username = #S)
AND
 (ltrDirectoryData.Node_ID = ltrSubscriptions.Trail)
AND
 ( tbl2.Node_ID = ltrDirectory.Parent)
AND
 ltrDirectory.Node_ID = ltrDirectoryData.Node_ID
");

/* Read all trails. Ordered, limited*/
define("GET_TRAILS_MYPAGE", "
SELECT 
 * 
FROM 
 #I
ORDER BY
 #I #I
LIMIT #I, ".DIR_MAX_TRAILS
);

/* Read all trails. Ordered, unlimited*/
define("GET_TRAILS_MYPAGEU", "
SELECT 
 * 
FROM 
 #I
ORDER BY
 #I #I
");

define("COUNT_RESULTS_MYPAGE", "SELECT count(*) as cnt FROM #I");

/* Drop der Tabelle */
define("DROP_TEMP_MYPAGE", "DROP TABLE #I");

?>