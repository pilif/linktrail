<?php
define("SQL_MSGGETFRIENDS_INC", true);
/*
SQL for getting to know, in what Relatoins UID1 and UID2 lay
*/


/*
 SQL_HARDFRIENDS returns something if UID1 is a friend of UID2.
*/
define("SQL_HARDFRIENDS",
"SELECT 
  count(*) as cnt
FROM 
  ltrFriends
WHERE
  (Userid = #S AND IsFriendOf = #S) 
OR 
  (IsFriendOf = #S AND Userid = #S)
");

/*
 SQL_IS_FRIENDSHIP_REQUEST can be used to determine whether UID1 requested
 the friendship of UID2 or the other way. Returns nothing if no Friendship-Requests
 about the two UIDs could be found in the System.
 
 Sender contains the Requestor in the uid-Format
 Target contains the Target user in the uid-Format
*/
define("SQL_IS_FRIENDSHIP_REQUEST", "
SELECT
 Message_ID, tblUIDS.user_id as Sender, Target
FROM
 ltrMessages, auth_user as tblUIDS
LEFT JOIN
 auth_user ON auth_user.username = ltrMessages.Sender
WHERE(  
 ( Target = #S AND auth_user.user_id = #S )
OR
 ( Target = #S AND auth_user.user_id = #S )
)AND
 MessageType = 1011 
AND 
 ltrMessages.Sender = tblUIDS.username
");

define("WAITING_FOR_BEING_ACCEPTED_BY_MYSELF","
SELECT
 auth_user.user_id, ltrMessages.Sender AS username
FROM
 ltrMessages
LEFT JOIN
 auth_user ON auth_user.username = ltrMessages.Sender
WHERE
 Target = #S
AND
 MessageType = 1011
");

define("WAITING_FOR_BEING_ACCEPTED_BY_MY_FRIENDS", "
SELECT
 ltrMessages.Target as user_id, tblNAMES.username
FROM
 ltrMessages, auth_user
LEFT JOIN
 auth_user AS tblNAMES ON ltrMessages.Target = tblNAMES.user_id 
WHERE
 (Sender = auth_user.username AND auth_user.user_id = #S)
AND
 MessageType = 1011
");
?>