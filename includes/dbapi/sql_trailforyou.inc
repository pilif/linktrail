<?php
define("SQL_TRAILFORYOU_INC", true);
/*
Contains SQL-statements for handling the "a trail for you"
Feature
*/

define("SQL_QUERYTRAILS", "
SELECT 
 ltrDirectory.Name, ltrDirectoryData.Description
FROM
 ltrDirectory, ltrDirectoryData, ltrDirectory as parent
LEFT JOIN
 ltrSubscriptions ON (ltrSubscriptions.Trail = ltrDirectory.Node_ID AND ltrSubscriptions.Username = #S)
WHERE
 parent.Node_ID IN (#I)
AND
 ltrSubscriptions.Username IS NULL
AND
 ltrDirectory.Parent = parent.Node_ID
AND
 ltrDirectory.ObjectType = 'trail'
AND 
 ltrDirectoryData.Node_ID = ltrDirectory.Node_ID
AND
 ltrDirectoryData.Owner != #S
ORDER BY
 ltrDirectoryData.ExtraLong DESC
LIMIT 0, 100
");




?>