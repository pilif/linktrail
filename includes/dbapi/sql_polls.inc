<?php
/* $Id: sql_polls.inc,v 1.1 2000-11-13 21:57:25 pilif Exp $ */
define("SQL_POLLS_INC", true);
/*
Theese SQL-Statments will be used to handle the polling-stuff
*/

define("SQL_READ_POLLS", "
 SELECT 
  *
 FROM
  ltrCurrentPolls
 WHERE 
  Node_ID = #I
 ORDER BY
  AddDate 
 DESC
");

define("SQL_GET_VOTES", "
SELECT
 Vote, count(Vote) as cnt
from 
 ltrVotes
WHERE
 Poll_ID = #I
GROUP BY 
 Vote");
 
define("DESTRUCT_POLL",  "DELETE FROM ltrCurrentPolls WHERE Poll_ID = #I");
define("DESTRUCT_VOTES", "DELETE FROM ltrVotes WHERE Poll_ID = #I"); //unnecessary in TRUE RDBMS

define("PLACE_VOTE","
 INSERT INTO
  ltrVotes
  (Expert, Poll_ID, Vote)
 VALUES
  (#S, #I, #I) ");

define("UPDATE_VOTE", "
 UPDATE
  ltrVotes
 SET 
  Vote = #I
 WHERE 
  (Poll_ID = #I AND Expert = #S)
");  

define("CREATE_POLL", "
 INSERT INTO
  ltrCurrentPolls
  (Node_ID, PollType, ExtraData, AddDate, Owner)
 VALUES
  (#I, #I, #S, NOW(), #S)
");

define("HAS_VOTED", "
 SELECT Vote FROM ltrVotes WHERE ( (Expert = #S) AND (Poll_ID = #I))
");
?>