<?php
define("SQL_TRAILUP_INC", true);
/*
Contains many SQL-Statements for climbing trails
*/

/*
 These 3 (incomplete) SQL-Statements are used to cleanup the directory.
 After the IN will follow a list of parentless IDs in ()
*/

/* Anzahl An Trails eines Nodes*/
define("TRAILCOUNT",
"
SELECT 
 count(*) as nr
FROM
 ltrDirectory
WHERE 
 Parent = #I
AND
 ObjectType = 'trail'
");
 

/* Tempor„re Tabelle erzeugen und Heimische Trails einfgen */
define("CREATE_AND_FILL","
CREATE TEMPORARY TABLE #I
 SELECT 
  ltrDirectoryData.*,  
  ltrDirectory.Parent, ltrDirectory.Name, ltrDirectory.Level,
  auth_user.username, 
  auth_user.user_id,
  ExtraLong as new_points
 FROM 
  ltrDirectoryData, ltrDirectory, auth_user 
 WHERE 
    (ltrDirectory.Parent = #I)
 AND
    (ltrDirectory.ObjectType = 'trail')
 AND
    ( ltrDirectory.Node_ID = ltrDirectoryData.Node_ID)
 AND
    ( auth_user.user_id = ltrDirectoryData.Owner)
");


/* Aufsteigende Trails (only)... */
define("INSERT_CLIMBING", "
INSERT INTO #I
 SELECT 
  ltrDirectoryData.*,  
  ltrDirectory.Parent, ltrDirectory.Name, ltrDirectory.Level,
  auth_user.username, 
  auth_user.user_id,
  FLOOR(ExtraLong * POW(0.97,(Level - #I + 1))  ) as new_points
 FROM 
  ltrDirectoryData, ltrDirectory, auth_user
 WHERE 
    ( (ltrDirectory.Name LIKE '#I%') ) 
 AND
    (ltrDirectory.ObjectType = 'trail')
 AND
    ( ltrDirectory.Node_ID = ltrDirectoryData.Node_ID)
 AND
    ( auth_user.user_id = ltrDirectoryData.Owner)
 AND
    ( (Level < #I) AND (Level > #I) )
 ORDER BY new_points DESC
 LIMIT #I
");

/* Lesen der Trails. Geordnet */
define("GET_TRAILS", "
SELECT 
 * 
FROM 
 #I
ORDER BY
 new_points DESC, ChangeDate ASC
LIMIT #I, ".DIR_MAX_TRAILS
);

/* Lesen der Trails. Geordnet */
define("GET_TRAILSU", "
SELECT 
 * 
FROM 
 #I
ORDER BY
 new_points DESC, ChangeDate ASC
");

define("COUNT_RESULTS", "SELECT count(*) as cnt FROM #I");

/* Drop der Tabelle */
define("DROP_TEMP", "DROP TABLE #I");

?>