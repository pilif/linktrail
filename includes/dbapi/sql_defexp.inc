<?php
define("SQL_DEFEXP_INC", true);
/*
Contains SQL-Statements for defined featured experts
*/

/*
 These 3 (incomplete) SQL-Statements are used to cleanup the directory.
 After the IN will follow a list of parentless IDs in ()
*/


define("GET_EXPERT","SELECT * FROM ltrDefExperts WHERE Node_ID = #I");

define("EDIT_EXPERT","
UPDATE
 ltrDefExperts
SET
 Username = #S,
 Description = #S
WHERE
 Node_ID = #I
");

define("ADD_EXPERT","
INSERT INTO
 ltrDefExperts
VALUES
 (#I, #S, #S)
");

define("DELETE_EXPERT","DELETE FROM ltrDefExperts WHERE Node_ID = #I");

define("VALID_EXPERT", "SELECT * FROM auth_user WHERE username = #S");
?>