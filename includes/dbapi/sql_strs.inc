<?php
define("SQLSTRS_INC", true);
/*
  sql_strs.inc; 1.01
  
  Contains SQL-Strings, used by the DBAPI
  
  Written by Pilif and Jody
  
  Created:  Feb, 20th 2000
  Modified: Mar, 22th/23th 2000
 
  © 2000 by Linktrail. All rights reserved  
 */

/*
 These 3 (incomplete) SQL-Statements are used to cleanup the directory.
 After the IN will follow a list of parentless IDs in ()
*/
define("SQL_CLEANUP_DATA",  "DELETE FROM ltrDirectoryData WHERE Node_ID IN "); 
define("SQL_CLEANUP_LINKS", "DELETE FROM ltrLinks WHERE Trail IN "); 
define("SQL_CLEANUP_DIR",   "DELETE FROM ltrDirectory WHERE Node_ID IN "); 

define("SQL_UID2NAME", "SELECT username FROM auth_user WHERE user_id = #S");

/* Update zum Downgraden aller Trails pro nicht ver„nderte Woche */
define("SQL_TIMEOUT", "
UPDATE 
 ltrDirectoryData 
SET 
 ExtraLong = ExtraLong - 
  FLOOR(
   (
    POW(1.5,
     FLOOR(( (UNIX_TIMESTAMP() - UNIX_TIMESTAMP(ChangeDate)) / 604800))+1)
   )*10
  ),
 ChangeDate = ChangeDate 
");

/* Durchschnitt aller "heimischen" Trails einer Kategorie */
define("SQL_AVG_POINTS", "
select
 Floor(
  avg(ltrDirectoryData.ExtraLong)
  ) as points 
from 
 ltrDirectory, ltrDirectoryData
WHERE 
 ltrDirectory.Parent = #I
AND
 ltrDirectory.Node_ID = ltrDirectoryData.Node_ID
AND
 ltrDirectory.ObjectType = 'trail' 
");

define("SET_KAT_POINTS", "UPDATE ltrDirectoryData SET ExtraLong = #I, ChangeDate = ChangeDate WHERE Node_ID = #I");
 
 /* Anzahl Subscriptions auf ein Objekt */
define("SQL_SUBSCRIPTION_COUNT", "
 SELECT 
  count(*) as count
 FROM
  ltrSubscriptions 
 WHERE 
  Trail = #I
");

 /* Anzahl anstehender Suggestions */
define("SQL_SUGGESTION_COUNT", "
 SELECT
  count(*) as count
 FROM
  ltrSubsSuggestions
 WHERE
  Node_ID = #I
");
 
 /* Update eines Trails. Jepp. Ich will Subselections */ 
define("SQL_BONUS_POINTS", "
 UPDATE 
  ltrDirectoryData  
 SET
  ExtraLong = ExtraLong 
   +( FLOOR(20*SQRT(#I))) /* Anzahl subscriptions */
   +( FLOOR(10*SQRT(#I))) /* Anzahl suggestions */
   + #I,                  /* 10 bei Suggestion, 20 bei Subscription */
   ChangeDate = ChangeDate
 WHERE
  Node_ID = #I
");


define("SQL_INSLINK_EX", "
INSERT INTO 
    ltrLinks 
    (Link_ID, Trail, Name, Description, Url, Next, Owner, ChangeDate, AddDate)
VALUES
    ( NULL, #I, #S, #S, #S, NULL, #S, NULL, NOW())
");

define("SQL_EDIT_LINK_EX",   
"UPDATE 
  ltrLinks 
 SET
  Name=#S,
  Description=#S,
  Url=#S,        
  Owner=#S
 WHERE Link_ID=#I");

define("SQL_EDIT_SLOT_EX",   
"UPDATE 
  ltrSlots 
 SET
  Trail_1_ID=#I,
  Trail_1_Text=#S,
  Trail_2_ID=#I,
  Trail_2_Text=#I,
  Description=#S
 WHERE Slot_ID=#I");

define("SQL_INSSLOT_EX", "
INSERT INTO 
    ltrSlots 
    (Slot_ID, Node_ID, Trail_1_ID, Trail_1_Text, Trail_2_ID, Trail_2_Text, Description)
VALUES
    ( NULL, #I, #I, #S, #I, #I, #S)
"); 
 
define("SQL_SLOT_DEL", "DELETE FROM ltrSlots WHERE Slot_ID=#I"); 

define("SQL_GET_SLOTS", "
SELECT 
 ltrSlots.*, tbl1.Name as Path1, tbl2.Name as Path2
FROM 
 ltrSlots 
LEFT JOIN ltrDirectory as tbl1 ON tbl1.Node_ID = ltrSlots.Trail_1_ID
LEFT JOIN ltrDirectory as tbl2 ON tbl2.Node_ID = ltrSlots.Trail_2_ID
WHERE ltrSlots.Node_ID = #I");

define("SQL_GET_SLOT_INFO", "
SELECT 
 ltrSlots.*, tbl1.Name as Path1, tbl2.Name as Path2
FROM 
 ltrSlots 
LEFT JOIN ltrDirectory as tbl1 ON tbl1.Node_ID = ltrSlots.Trail_1_ID
LEFT JOIN ltrDirectory as tbl2 ON tbl2.Node_ID = ltrSlots.Trail_2_ID
WHERE ltrSlots.Slot_ID = #I");


define("SQL_CREATE_SYMLINK", "INSERT INTO ltrDirectory SET Name = #S, LinkTo=#S, Parent=#I, Level=#I, ObjectType='node', Language=#I");
define("SQL_RENAME_SYMLINK", "UPDATE ltrDirectory SET Name=#S WHERE Node_ID=#I");

define("SQL_GET_SYMLINKS_TO_DESTINATION","SELECT * FROM ltrDirectory WHERE LinkTo=#S");
define("SQL_UPDATE_DO_CHANGE", "UPDATE ltrDirectoryData SET ChangeDate = NOW() WHERE Node_ID=#I");
define("SQL_UPDATE_USER_LOGIN","UPDATE ltrUserData SET LastOnline = NOW() WHERE User_ID=#S");


define("SQL_SHARE_DEL","DELETE FROM ltrSubsSuggestions WHERE (TargetUser = #S AND Node_ID = #I)");
define("SQL_SHARE_ADD", "INSERT INTO ltrSubsSuggestions VALUES (NULL, #I, #S, #S, NULL)");

define("SQL_UPDATE_TARGETS", "UPDATE ltrDirectory SET LinkTo = #S WHERE LinkTo = #S");

define("SQL_SHARE_PERM_OWNER_SUBS", "
SELECT 
ltrSubscriptions.Username as subscribed, 
auth_user.username as owner, ltrDirectoryData.Node_ID 
FROM 
ltrDirectoryData 
left join ltrSubscriptions on ltrDirectoryData.Node_ID = ltrSubscriptions.Trail 
left join auth_user on auth_user.user_id = ltrDirectoryData.Owner 
WHERE
 (
   ltrSubscriptions.Username IN (#I) 
  AND
   ltrSubscriptions.Trail = #I
  )
OR
 ( 
   auth_user.username IN (#I)
  AND
   ltrDirectoryData.Node_ID = #I
 )
");

define("SQL_SHARE_SUGGESTED_MSGS", "
SELECT
 auth_user.username, ltrMessages.Data
FROM
 ltrMessages, auth_user as lookup_name
LEFT JOIN auth_user on ltrMessages.Target = auth_user.user_id
WHERE
 ltrMessages.Target = lookup_name.user_id
AND
 lookup_name.username IN (#I)
AND
 ltrMessages.MessageType = 1008
AND
 ((ltrMessages.ReferenceCount & 1) != 0)
");

define("SQL_LOCK", "LOCK TABLES #I WRITE"); //#S would add '
define("SQL_UNLOCK", "UNLOCK TABLES"); //#S would add '

define("SQL_USER_EXISTS", "SELECT user_id FROM auth_user WHERE username=#S");

define("SQL_ADD_USER1", "INSERT INTO auth_user VALUES (#S, #S, #S, #S)");
define("SQL_ADD_USER2", "INSERT INTO ltrUserData SET Email = #S, User_ID=#S");

define("SQL_USER_SUGGESTIONS", "SELECT username FROM auth_user WHERE username LIKE '#I%' ORDER BY username");

define("SQL_CHANGE_PASSWORD", "UPDATE auth_user SET password = #S where username = #S");

define("SQL_GET_PW_FROM_UNAME", "
SELECT 
 auth_user.password, ltrUserData.Email, auth_user.username 
FROM 
 auth_user, ltrUserData 
WHERE 
 username=#S
AND
 auth_user.user_id = ltrUserData.User_ID
");

define("SQL_GET_PW_FROM_EMAIL", "
SELECT
 auth_user.password, ltrUserData.Email, auth_user.username
FROM 
 auth_user, ltrUserData
WHERE
 auth_user.user_id = ltrUserData.User_ID
AND
 ltrUserData.Email = #S
");

define("SQL_UPDATE_USER", 
"UPDATE 
  ltrUserData
 SET
  FirstName = #S,
  LastName = #S,
  Email = #S,
  Homepage = #S,
  Description = #S,
  MyURLs = #S,
  Gender = #S,
  Age = '#I',
  Interests = #S,
  Country = #S,
  PhotoUrl = #S,
  AIM = #S,
  Yahoo = #S,
  ICQ = #I,
  MSN_Name = #S,
  MSN_Pro = #S,
  ShowNobody = #I,
  ShowFriend = #I, 
  ShowAnyone = #I,
  PopupPos  = #S,
  NS6Sidebar = #S,
  ResPerPage = #I,
  HighlightSearch = #S,
  NotificationStyle = #I,
  Vacation = #S,
  MypageAfterLogon = #S,
  NewWindow = #S
 WHERE
  User_ID = #S 
  ");

define("SQL_RENAME_KAT_CHILDREN",
"
UPDATE
 ltrDirectory
SET
 Name = REPLACE (Name, #S, #S)
WHERE
 Name LIKE '#I%'
");

define("SQL_GET_CHILDREN_RENAME",
"
SELECT 
 Node_ID, Name
FROM
 ltrDirectory
WHERE
 Name LIKE '#I%'
AND 
 Language = #I 
");

define("SQL_UPDATE_CHILD",
"
UPDATE
 ltrDirectory
SET
 Name = #S,
 Level = #I
WHERE
 Node_ID = #I
");

define("SQL_UNSUBSCRIBE", "DELETE FROM ltrSubscriptions WHERE Username = #S AND Trail = #I");
define("SQL_SUBSCRIBE", "INSERT INTO ltrSubscriptions (Username, Trail) VALUES (#S, #I)");
//define("SQL_CHSUBSCRIPTION", "UPDATE ltrSubscriptions SET NotificationMethod = #I WHERE (Username = #S AND Trail = #I)");
//define("SQL_CHNOTIFICATION", "UPDATE ltrDirectoryData SET OwnerNotification = #I WHERE Node_ID = #I");


define("SQL_UPDATE_PERMS", "UPDATE ltrDirectoryData SET FriendAccess=#I, UserAccess=#I WHERE Node_ID=#I");

define("SQL_REMOVE_FRIENDSHIP", "DELETE FROM ltrFriends WHERE (Userid = #S AND IsFriendOf=#S) OR (Userid=#S AND IsFriendOf=#S)");
define("SQL_INSERT_FRIENDS", "INSERT INTO ltrFriends VALUES(NULL, #S, #S, 'stab')");
define("SQL_IS_FRIENDSHIP", "SELECT * FROM ltrFriends WHERE (Userid=#S AND IsFriendOf=#S) OR (Userid=#S AND IsFriendOf=#S)");
define("SQL_FRIEND_UPDATE_REC", "UPDATE ltrFriends SET State = #S WHERE Friendship_ID = #I"); 

define("SQL_GET_FRIENDS", 
"
SELECT 
 auth_user.username AS username,  auth_user.user_id AS user_id
FROM 
 ltrFriends, auth_user 
WHERE 
 ( (Userid = #S) OR (IsFriendOf=#S) ) 
AND 
 (State = 'stab') 
AND 
 (
   (auth_user.user_id != #S AND auth_user.user_id = IsFriendOf) OR (auth_user.user_id != #S AND auth_user.user_id = Userid)
  )
ORDER BY auth_user.username");
 
define("SQL_AWAITED",     "SELECT auth_user.username AS retval, IsFriendOf AS retval2 FROM ltrFriends, auth_user WHERE (Userid = #S) AND (State = 'req') AND (IsFriendOf = auth_user.user_id)");
define("SQL_WAITING",     "SELECT auth_user.username AS retval, Userid AS retval2 FROM ltrFriends, auth_user WHERE (IsFriendOf = #S) AND (State = 'req') AND (auth_user.user_id = Userid)");



define("SQL_IS_SUBSCRIBED", "SELECT NotificationMethod FROM ltrSubscriptions WHERE Username=#S AND Trail=#I");

define("SQL_GET_PERMISSION", 
"select 
 UserAccess, FriendAccess
from
 ltrDirectory, ltrDirectoryData
where 
 ltrDirectoryData.Node_ID = ltrDirectory.Node_ID 
and
 ltrDirectory.Name = #S
and 
 ltrDirectory.Langauge = #I 
");
 
define("SQL_GET_PARENTLESS", "
select 
 tbl1.Name, tbl1.Node_ID
from 
 ltrDirectory as tbl1
left join
 ltrDirectory as tbl2
on tbl1.Parent = tbl2.Node_ID
where 
 tbl2.Node_ID is NULL
AND 
 tbl1.Name <> '/'
");
 
define("SQL_MAXLEVEL", "SELECT Level FROM ltrDirectory ORDER BY Level DESC LIMIT 1");
 
//delete an user-change from the table (afer approving/disapproving it)
define("SQL_RUSERC", "DELETE FROM ltrUserChanges WHERE ID=#I");
 
//get a list of all user-changes avaiting approval
define("SQL_DUSERC", "SELECT * FROM ltrUserChanges ORDER BY When");

define("SQL_LINKDEL", "DELETE FROM ltrLinks WHERE Link_ID=#I");

define("SQL_INSLINK", "
INSERT INTO 
    ltrLinks 
    (Link_ID, Trail, Name, Description, Url, Position, Owner, ChangeDate, AddDate)
VALUES
    ( NULL, #I, #S, #S, #S, NULL, #S, NULL, #I)
");

define("SQL_INSLINKWP", "
INSERT INTO 
    ltrLinks 
    (Link_ID, Trail, Name, Description, Url, Position, Owner, ChangeDate, AddDate)
VALUES
    ( NULL, #I, #S, #S, #S, #I, #S, NULL, #I)
");

//Edit a directory-Object:
define("SQL_OEDT_1", "UPDATE ltrDirectory SET Name=#S, Level=#I, Language=#I, IntNode=#I WHERE Node_ID=#I");
define("SQL_OEDT_2", "UPDATE ltrDirectoryData SET Description=#S, UserAccess=#I, FriendAccess=#I, Owner=#S WHERE Node_ID=#I");

define("SQL_UPDATE_DIR_CHANGE_DATE", "UPDATE ltrDirectoryData SET ChangeDate = NOW() WHERE Node_ID=#I");
 
//add a trail. Because of a small design-bug in PHPLIB, I have to manually 
//ask for the generated Node_ID. So we have tree queries for adding a
//directory-objects.
define("SQL_OADD_1", "INSERT INTO ltrDirectory SET Name=#S, Parent=#I, Level=#I, ObjectType=#S, Language=#I, IntNode=#I");
define("SQL_OADD_a", "SELECT Node_ID FROM ltrDirectory WHERE Name=#S");
define("SQL_OADD_2", "INSERT INTO ltrDirectoryData SET Node_ID=#I, Description=#S, UserAccess=#I, FriendAccess=#I, Owner=#S, AddDate=NOW(), ExtraLong=#I");

//Direct deletion of a directory-object 
define("SQL_RM_D1",   "DELETE FROM ltrDirectory WHERE ltrDirectory.Node_ID=#I");
define("SQL_RM_D2",   "DELETE FROM ltrDirectoryData WHERE ltrDirectoryData.Node_ID = #I");
define("SQL_RM_D3",   "DELETE FROM ltrLinks WHERE Trail = #I");
//define("SQL_RM_D4",   "DELETE FROM ltrDirectory WHERE Trail = #I");
define("SQL_RM_D5",   "DELETE FROM ltrSubscriptions WHERE Trail = #I");
define("SQL_RM_D6",   "DELETE FROM ltrSubsSuggestions WHERE Node_ID = #I");
 
define("SQL_EDIT_LINK_WP",   
"UPDATE 
  ltrLinks 
 SET
  Name=#S,
  Description=#S,
  Url=#S,        
  Position=#I,
  Owner=#S
 WHERE Link_ID=#I");

define("SQL_EDIT_LINK_OP",   
"UPDATE 
  ltrLinks 
 SET
  Position=#I
 WHERE Link_ID=#I");
 
define("SQL_EDIT_LINK",   
"UPDATE 
  ltrLinks 
 SET
  Name=#S,
  Description=#S,
  Url=#S,        
  Owner=#S
 WHERE Link_ID=#I");

 
//Direct movement of a directory-object:
//(1): Update DirectoryStructure 
define("SQL_MV_D",   "UPDATE ltrDirectory SET ".
                     "Name=#S, Level=#I, Parent=#I WHERE Name=#S");

//add a link to the user-approvement-table 
define("SQL_MD_L_I", "INSERT INTO ltrUserChanges (Object_Name, Type, ChangeString, Owner, ObjectType)".
                     "VALUES (#S, #S, #S, #S, 'link')");

//add an item to the user-approvement-table 
define("SQL_MD_O_I", "INSERT INTO ltrUserChanges (Object_Name, Type, ChangeString)".
                     "VALUES (#S, #S, #S)");

define("SQL_GETUIDFNAME", "SELECT user_id FROM auth_user WHERE auth_user.username = #S"); 

 
define("SQL_GETUSERFUID", "SELECT auth_user.username, auth_user.user_id, ltrUserData.* FROM auth_user LEFT JOIN 
                            ltrUserData 
						   ON 
						    ltrUserData.User_ID = auth_user.user_id 
						   WHERE
                            auth_user.username = #S"); 

define("SQL_GETUSERFNAM", "SELECT username, ltrUserData.* FROM auth_user, ltrUserData WHERE
                            auth_user.username = #S 
                            AND auth_user.user_id = ltrUserData.User_ID"); 


define("SQL_LINKINFO", "SELECT ltrLinks.Link_ID, ltrLinks.Name as title, Description, Url, ChangeDate, AddDate, Position, username, user_id, Next
                     FROM   ltrLinks, auth_user
                     WHERE
                      ltrLinks.Link_ID = #I  
                     AND
                      ltrLinks.Owner = auth_user.user_id
                     ");
                            
 
define("SQL_LINKS", "SELECT ltrLinks.Link_ID, ltrLinks.Name as title, Description, Url, ChangeDate, Position, AddDate, username, user_id, Next
                     FROM   ltrLinks, auth_user, ltrDirectory as lookup
                     WHERE
                            (lookup.Name = #S)
                     AND
                            (lookup.Language = #I)
                     AND
                            ltrLinks.Trail = lookup.Node_ID  
                     AND
                            ltrLinks.Owner = auth_user.user_id
					 ORDER BY
					 	    ltrLinks.AddDate
					 ");
 
define("SQL_CATUP", "SELECT Name FROM ltrDirectory 
                      WHERE 
                       (Level > #I AND Level < #I+".CAT_LEVEL_UP_COUNT.") 
                      AND 
                       ExtraLong  >= ".CAT_MARK."
                      AND
                       ObjectType = 'node'
                      LIMIT ".CAT_UP_COUNT);


//find trails that can climb up
define("SQL_UPTRAILS",  
"SELECT 
ltrDirectoryData.*, 
auth_user.username, auth_user.user_id,
ltrDirectory.* 
FROM 
 ltrDirectoryData,
 auth_user, 
 ltrDirectory, 
 ltrDirectory as tbl2  
WHERE 
   ( (tbl2.Name=#S) AND (ltrDirectory.Parent = tbl2.Node_ID) ) 
AND 
   (ltrDirectory.ObjectType = 'trail') 
AND 
   ((ltrDirectory.Level > #I) AND (ltrDirectory.Level<= #I+".TRAIL_LEVEL_UP_COUNT."))
AND 
   (ltrDirectoryData.Owner = auth_user.user_id) 
AND 
   (ltrDirectory.Node_ID = ltrDirectoryData.Node_ID) 
ORDER BY ltrDirectoryData.ExtraLong DESC
LIMIT ".TRAIL_UP_COUNT);


define("SQL_TRAILCOUNT",
"SELECT 
COUNT(*) as c
FROM 
 ltrDirectoryData, ltrDirectory, ltrDirectory as tbl2, auth_user 
WHERE 
   ( (tbl2.Name=#S) AND (ltrDirectory.Parent = tbl2.Node_ID) ) 
AND
   (ltrDirectory.ObjectType = 'trail')
AND
   ( ltrDirectory.Node_ID = ltrDirectoryData.Node_ID)
AND
   ( auth_user.user_id = ltrDirectoryData.Owner)
");

//find all trails of a node. Limited.
define("SQL_DIRSTRAILL", 
"SELECT 
 ltrDirectoryData.*,  
 ltrDirectory.*, auth_user.username, auth_user.user_id 
FROM 
 ltrDirectoryData, ltrDirectory, ltrDirectory as tbl2, auth_user 
WHERE 
   ( (tbl2.Name=#S) AND (ltrDirectory.Parent = tbl2.Node_ID) ) 
AND
   ( tbl2.Language = #I )   
AND
   (ltrDirectory.ObjectType = 'trail')
AND
   ( ltrDirectory.Node_ID = ltrDirectoryData.Node_ID)
AND
   ( auth_user.user_id = ltrDirectoryData.Owner)
ORDER BY
    ltrDirectoryData.ExtraLong DESC 
LIMIT #I,".DIR_MAX_TRAILS);


define("SQL_NEWTRAILSL", 
"SELECT 
 ltrDirectoryData.AddDate, ltrDirectoryData.ChangeDate,  
 ltrDirectory.* 
FROM 
 ltrDirectoryData, ltrDirectory 
WHERE 
   ( ltrDirectory.Parent = #I ) 
AND
   (ltrDirectory.ObjectType = 'trail')
AND
   ( ltrDirectory.Node_ID = ltrDirectoryData.Node_ID)
AND
   (Language = #I)
ORDER BY
    ltrDirectoryData.AddDate DESC
LIMIT ".DIR_MAX_NEWTRAILS);

define("SQL_NEWTRAILSG", 
"SELECT 
 ltrDirectoryData.AddDate, ltrDirectoryData.ChangeDate,  
 ltrDirectory.* 
FROM 
 ltrDirectoryData, ltrDirectory 
WHERE 
   (ltrDirectory.ObjectType = 'trail')
AND
   ( ltrDirectory.Node_ID = ltrDirectoryData.Node_ID)
AND 
   (Language = #I)   
ORDER BY
    ltrDirectoryData.AddDate DESC
LIMIT ".DIR_MAX_NEWTRAILS);


define("SQL_NEWTRAILSU", 
"SELECT 
 ltrDirectoryData.*,  
 ltrDirectory.*, auth_user.username, auth_user.user_id 
FROM 
 ltrDirectoryData, ltrDirectory, ltrDirectory as tbl2, auth_user 
WHERE 
   ( (tbl2.Name=#S) AND (ltrDirectory.Parent = tbl2.Node_ID) ) 
AND
   (ltrDirectory.ObjectType = 'trail')
AND
   ( ltrDirectory.Node_ID = ltrDirectoryData.Node_ID)
AND
   ( auth_user.user_id = ltrDirectoryData.Owner)
AND
   ( ltrDirectoryData.ChangeDate > '#I' )
ORDER BY
    ltrDirectoryData.ExtraLong DESC 
");

//find all trails of a node. unLimited.
define("SQL_DIRSTRAILU", 
"SELECT 
 ltrDirectoryData.*,  
 ltrDirectory.*, auth_user.username, auth_user.user_id 
FROM 
 ltrDirectoryData, ltrDirectory, ltrDirectory as tbl2, auth_user 
WHERE 
   ( (tbl2.Name=#S) AND (ltrDirectory.Parent = tbl2.Node_ID) ) 
AND
   (ltrDirectory.ObjectType = 'trail')
AND
   ( ltrDirectory.Node_ID = ltrDirectoryData.Node_ID)
AND
   ( auth_user.user_id = ltrDirectoryData.Owner)
AND
   (Language = #I)   
ORDER BY
    ltrDirectoryData.ExtraLong DESC   
 ");
 

//Find all the Trails to display on the given User's Mypage
define("SQL_MYPAGE", 
"
SELECT 
ltrDirectoryData.*, 
ltrDirectory.*, 
ltrSubscriptions.NotificationMethod, ltrSubscriptions.Trail as SubscribedToTrail,
auth_user.username, tbl2.Name as parent,
ltrSubsSuggestions.Suggestor as suggested_from,
RIGHT(ltrDirectory.Name, INSTR(MID(REVERSE(ltrDirectory.Name),2,LENGTH(ltrDirectory.Name)) ,'/')) as friendlyname_trail,
RIGHT(tbl2.Name, INSTR(MID(REVERSE(tbl2.Name),2,LENGTH(tbl2.Name)) ,'/')) as friendlyname_parent
FROM 
 ltrDirectoryData, ltrDirectory, auth_user, ltrDirectory as tbl2
LEFT JOIN ltrSubscriptions ON ltrSubscriptions.Trail = ltrDirectoryData.Node_ID AND ltrSubscriptions.Username = auth_user.username
LEFT JOIN ltrSubsSuggestions ON ltrSubsSuggestions.Node_ID = ltrDirectoryData.Node_ID AND ltrSubsSuggestions.TargetUser = auth_user.username
where
 (auth_user.username = #S)
AND
 ( ltrDirectoryData.owner = auth_user.user_id OR ltrSubscriptions.Username = auth_user.username OR ltrSubsSuggestions.TargetUser = auth_user.username)
AND
 ( tbl2.Node_ID = ltrDirectory.Parent)
AND
 ltrDirectory.Node_ID = ltrDirectoryData.Node_ID
ORDER BY
    suggested_from DESC, #I #I   
");

//Find all the Trails to display on the given User's Mypage. With LIMIT
define("SQL_MYPAGEL", 
"
SELECT 
ltrDirectoryData.*, 
ltrDirectory.*, 
ltrSubscriptions.NotificationMethod, ltrSubscriptions.Trail as SubscribedToTrail,
auth_user.username, tbl2.Name as parent,
ltrSubsSuggestions.Suggestor as suggested_from,
RIGHT(ltrDirectory.Name, INSTR(MID(REVERSE(ltrDirectory.Name),2,LENGTH(ltrDirectory.Name)) ,'/')) as friendlyname_trail,
RIGHT(tbl2.Name, INSTR(MID(REVERSE(tbl2.Name),2,LENGTH(tbl2.Name)) ,'/')) as friendlyname_parent
FROM 
 ltrDirectoryData, ltrDirectory, auth_user, ltrDirectory as tbl2
LEFT JOIN ltrSubscriptions ON ltrSubscriptions.Trail = ltrDirectoryData.Node_ID AND ltrSubscriptions.Username = auth_user.username
LEFT JOIN ltrSubsSuggestions ON ltrSubsSuggestions.Node_ID = ltrDirectoryData.Node_ID AND ltrSubsSuggestions.TargetUser = auth_user.username
where
 (auth_user.username = #S)
AND
 ( ltrDirectoryData.owner = auth_user.user_id OR ltrSubscriptions.Username = auth_user.username OR ltrSubsSuggestions.TargetUser = auth_user.username)
AND
 ( tbl2.Node_ID = ltrDirectory.Parent)
AND
 ltrDirectory.Node_ID = ltrDirectoryData.Node_ID
ORDER BY
    suggested_from DESC, #I #I   
LIMIT #I,".DIR_MAX_TRAILS);

//Count all the Trails to display on the given User's Mypage
define("SQL_MYPAGE_COUNT", 
"
SELECT 
count(*) as c
FROM 
 ltrDirectoryData, ltrDirectory, auth_user, ltrDirectory as tbl2
LEFT JOIN ltrSubscriptions ON ltrSubscriptions.Trail = ltrDirectoryData.Node_ID AND ltrSubscriptions.Username = auth_user.username
LEFT JOIN ltrSubsSuggestions ON ltrSubsSuggestions.Node_ID = ltrDirectoryData.Node_ID AND ltrSubsSuggestions.TargetUser = auth_user.username
where
 (auth_user.username = #S)
AND
 ( ltrDirectoryData.owner = auth_user.user_id OR ltrSubscriptions.Username = auth_user.username OR ltrSubsSuggestions.TargetUser = auth_user.username)
AND
 ( tbl2.Node_ID = ltrDirectory.Parent)
AND
 ltrDirectory.Node_ID = ltrDirectoryData.Node_ID
");






//Find all the Trails to display on the given User's Mypage
define("SQL_FOREIGN_MYPAGE", 
"
SELECT 
ltrDirectoryData.*, 
ltrDirectory.*, 
ltrSubscriptions.NotificationMethod, ltrSubscriptions.Trail as SubscribedToTrail,
auth_user.username, tbl2.Name as parent,
RIGHT(ltrDirectory.Name, INSTR(MID(REVERSE(ltrDirectory.Name),2,LENGTH(ltrDirectory.Name)) ,'/')) as friendlyname_trail,
RIGHT(tbl2.Name, INSTR(MID(REVERSE(tbl2.Name),2,LENGTH(tbl2.Name)) ,'/')) as friendlyname_parent
FROM 
 ltrDirectoryData, ltrDirectory, auth_user, ltrDirectory as tbl2
LEFT JOIN ltrSubscriptions ON ltrSubscriptions.Trail = ltrDirectoryData.Node_ID AND ltrSubscriptions.Username = auth_user.username
where
 (auth_user.username = #S)
AND
 ( ltrDirectoryData.owner = auth_user.user_id OR ltrSubscriptions.Username = auth_user.username)
AND
 ( tbl2.Node_ID = ltrDirectory.Parent)
AND
 ltrDirectory.Node_ID = ltrDirectoryData.Node_ID
ORDER BY
    #I #I   
");

//Find all the Trails to display on the given User's Mypage. With LIMIT
define("SQL_FOREIGN_MYPAGEL", 
"
SELECT 
ltrDirectoryData.AddDate, ltrDirectoryData.ChangeDate, 
ltrDirectory.*, 
ltrSubscriptions.NotificationMethod, ltrSubscriptions.Trail as SubscribedToTrail,
auth_user.username, tbl2.Name as parent,
RIGHT(ltrDirectory.Name, INSTR(MID(REVERSE(ltrDirectory.Name),2,LENGTH(ltrDirectory.Name)) ,'/')) as friendlyname_trail,
RIGHT(tbl2.Name, INSTR(MID(REVERSE(tbl2.Name),2,LENGTH(tbl2.Name)) ,'/')) as friendlyname_parent
FROM 
 ltrDirectoryData, ltrDirectory, auth_user, ltrDirectory as tbl2
LEFT JOIN ltrSubscriptions ON ltrSubscriptions.Trail = ltrDirectoryData.Node_ID AND ltrSubscriptions.Username = auth_user.username
where
 (auth_user.username = #S)
AND
 ( ltrDirectoryData.owner = auth_user.user_id OR ltrSubscriptions.Username = auth_user.username)
AND
 ( tbl2.Node_ID = ltrDirectory.Parent)
AND
 ltrDirectory.Node_ID = ltrDirectoryData.Node_ID
ORDER BY
    #I #I   
LIMIT #I,".DIR_MAX_TRAILS);

//Count all the Trails to display on the given User's Mypage
define("SQL_FOREIGN_MYPAGE_COUNT", 
"
SELECT 
count(*) as c
FROM 
 ltrDirectoryData, ltrDirectory, auth_user, ltrDirectory as tbl2
LEFT JOIN ltrSubscriptions ON ltrSubscriptions.Trail = ltrDirectoryData.Node_ID AND ltrSubscriptions.Username = auth_user.username
where
 (auth_user.username = #S)
AND
 ( ltrDirectoryData.owner = auth_user.user_id OR ltrSubscriptions.Username = auth_user.username)
AND
 ( tbl2.Node_ID = ltrDirectory.Parent)
AND
 ltrDirectory.Node_ID = ltrDirectoryData.Node_ID
");





//find all trails that are subscribed by $uid and find the notificatoin-type
//Limited.
define("SQL_SUBSCRIBEDL", 
"SELECT 
 ltrDirectoryData.*,  
 ltrDirectory.*
 auth_user.username, ltrSubscriptions.NotificationType
FROM 
 ltrDirectoryData, ltrDirectory, ltrSubscriptions, auth_user
WHERE 
   ( ltrSubscriptions.User_ID = #S ) 
AND
   (ltrDirectory.Node_ID = ltrSubscriptions.Trail)
AND
   ( ltrDirectoryData.Node_ID = ltrSubscriptions.Trail)
AND
   ( auth_user.user_id = ltrDirectoryData.Owner)
ORDER BY
    ltrDirectoryData.ChangeDate DESC   
 LIMIT ".DIR_MAX_TRAILS);

//find all trails that are subscribed by $uid and find the notificatoin-type
define("SQL_SUBSCRIBEDU", 
"SELECT 
 ltrDirectoryData.*,  
 ltrDirectory.*
 auth_user.username, ltrSubscriptions.NotificationType
FROM 
 ltrDirectoryData, ltrDirectory, ltrSubscriptions, auth_user
WHERE 
   ( ltrSubscriptions.User_ID = #S ) 
AND
   (ltrDirectory.Node_ID = ltrSubscriptions.Trail)
AND
   ( ltrDirectoryData.Node_ID = ltrSubscriptions.Trail)
AND
   ( auth_user.user_id = ltrDirectoryData.Owner)
ORDER BY
    ltrDirectoryData.ChangeDate DESC   
 ");

define("SQL_GET_SUBSCRIBERS","
 SELECT 
  auth_user.user_id, ltrSubscriptions.NotificationMethod
 FROM 
  ltrSubscriptions, auth_user
 WHERE 
  ltrSubscriptions.Trail = #I 
 AND
  ltrSubscriptions.Username = auth_user.username");

//find object-ID from path
define("SQL_ID2PATH", "SELECT Name FROM ltrDirectory WHERE Node_ID=#I");
                        
//find object-ID from path
define("SQL_PATH2ID", "SELECT Node_ID FROM ltrDirectory WHERE Name=#S AND Language = #I");

//get the parent-id of an object
define("SQL_GETPARENT", "SELECT Parent FROM ltrDirectory WHERE Node_ID=#I");

//get everything about a node.
/*define("SQL_OBJINFO_NODE_old", 
"SELECT 
  ltrDirectory.Name, ltrDirectory.Level, ltrDirectoryData.*
FROM 
  ltrDirectory, ltrDirectoryData, auth_user
WHERE
  ltrDirectory.Name = #S
AND
  ltrDirectory.Node_ID = ltrDirectoryData.Node_ID");
*/

//get everything about a node.
define("SQL_OBJINFO_NODE", 
"SELECT 
  ltrDirectory.Name, ltrDirectory.Node_ID, ltrDirectory.IntNode, ltrDirectory.Language, ltrDirectory.LinkTo, ltrDirectory.Level,
  ltrDirectoryData.Description, ltrDirectoryData.UserAccess, ltrDirectoryData.FriendAccess,
  ltrDirectoryData.ExtraLong, ltrDirectoryData.ChangeDate, ltrDirectoryData.Owner,
  ltrDirectoryData.SponsoredBy, ltrDirectoryData.HasImage, ltrDirectoryData.OwnerNotification,
  ltrDirectoryData.AddDate
FROM 
  ltrDirectory
LEFT JOIN ltrDirectoryData ON ltrDirectory.Node_ID = ltrDirectoryData.Node_ID
WHERE
  ltrDirectory.Name = #S
AND 
  Language = #I
");
  
//get everything about a trail.
define("SQL_OBJINFO_old", 
"SELECT 
  ltrDirectory.Name, ltrDirectoryData.*, auth_user.username 
FROM 
  ltrDirectory, ltrDirectoryData, auth_user
WHERE
  ltrDirectory.Name = #S
AND
  ltrDirectory.Node_ID = ltrDirectoryData.Node_ID
AND  
  auth_user.user_id   = ltrDirectoryData.Owner");

define("SQL_OBJINFO", 
"SELECT 
  ltrDirectory.Name, ltrDirectory.Node_ID, ltrDirectory.IntNode, ltrDirectory.Language, ltrDirectory.LinkTo, auth_user.username, 
  ltrDirectoryData.Description, ltrDirectoryData.UserAccess, ltrDirectoryData.FriendAccess,
  ltrDirectoryData.ExtraLong, ltrDirectoryData.ChangeDate, ltrDirectoryData.Owner,
  ltrDirectoryData.SponsoredBy, ltrDirectoryData.HasImage, ltrDirectoryData.OwnerNotification,
  ltrDirectoryData.AddDate
FROM 
  ltrDirectory
LEFT JOIN ltrDirectoryData ON ltrDirectory.Node_ID = ltrDirectoryData.Node_ID
LEFT JOIN auth_user ON ltrDirectoryData.Owner = auth_user.user_id
WHERE
  ltrDirectory.Name = #S
AND
  Language = #I
");
  

// find all subnodes (Path's) of a container object
define("SQL_SUBNODE", "SELECT tbl1.Name, tbl1.LinkTo, tbl1.Parent, tbl2.Parent FROM ltrDirectory as tbl1, ltrDirectory as tbl2 WHERE tbl2.Name=#S AND tbl1.ObjectType='node' AND tbl1.Parent = tbl2.Node_ID AND tbl1.Language=#I ORDER BY tbl2.Name");  

define("SQL_SUBNODES_EX",
"
SELECT 
 tbl1.name, tbl2.name as subname, tbl1.LinkTo, tbl2.LinkTo as sub_link_to, ltrDirectoryData.ExtraLong as subpts
FROM 
 ltrDirectory as tbl1
LEFT JOIN 
 ltrDirectory as tbl2 on tbl2.parent = tbl1.Node_ID AND tbl2.ObjectType='node' 
LEFT JOIN
 ltrDirectoryData on tbl2.Node_ID = ltrDirectoryData.Node_ID
WHERE 
 tbl1.Parent = #I
AND NOT ((ltrDirectoryData.ExtraLong IS NULL) AND (tbl2.name IS NOT NULL))
AND
 tbl1.Language = #I
AND
 tbl1.ObjectType = 'node' 
ORDER BY
 tbl1.Name ASC, subpts DESC
");

?>
