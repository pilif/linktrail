#!/usr/local/bin/php -q

<?php
 $postgres = pg_pconnect("user=postgres dbname=linktrail");
 $mysql    = mysql_pconnect("localhost", "root", "oelberg");
 mysql_select_db("linktrail", $mysql);
 
 function path2id($str){
   global $mysql;
   
   $query = "SELECT Node_ID FROM ltrDirectory WHERE Name = '$str'";
   $res   = mysql_query($res);
   if (!$res) return 'NULL';
   if (mysql_num_rows($res) == 0) return 'NULL';
   $row = mysql_fetch_array($res);
   return $row['Node_ID'];
 }

 printf("\nWorking on users...\n");
 $query = "DELETE FROM auth_user";
 pg_exec($postgres, $query) or die("\tCould not clean the users table\n");
 $query = "SELECT * FROM auth_user";
 $res = mysql_query($query) or die("\tCould not query the users\n");
 while ($row = mysql_fetch_array ($res)) {
    $query = sprintf("INSERT INTO auth_user VALUES ('%s', '%s', '', '%s', '%s')",
                     $row['user_id'], $row['username'], $row['password'], $row['perms']);
    pg_exec($postgres, $query);		     
    echo("\tInserted user ".$row['username']."\n");
 }
 mysql_free_result ($res);

 printf("\nWorking on users-data...\n");
 $query = "DELETE FROM ltrUserData";
 pg_exec($postgres, $query) or die("Could not clean the ltrUserData table\n");
 $query = "SELECT * FROM ltrUserData";
 $res = mysql_query($query) or die("Could not query the ltrUserData\n");
 while ($row = mysql_fetch_array ($res)) {

    if ($row['LastOnline'] == '0000-00-00 00:00:00')
     $row['LastOnline'] = '1970-01-01 00:00:00';
    if ($row['LastSent'] == '0000-00-00')
     $row['LastSent'] = '1970-01-01';
    if ($row['LastReadMessages'] == '0000-00-00 00:00:00')
     $row['LastReadMessages'] = '1970-01-01 00:00:00';
     
    foreach($row as $key => $value)
     $row[$key] = addslashes($value);
      
    $query = sprintf("
      INSERT INTO 
       ltrUserData
      (User_ID, FirstName, LastName, Email, Homepage, Description,
       Gender, Age, Country, PhotoUrl, ShowNobody, ShowFriend,
       ShowAnyone, PopupPos, NS6Sidebar, ResPerPage, HighlightSearch,
       IfaceLang, Langs, NotificationStyle, Vacation, MypageAfterLogon,
       LastOnline, NewWindow, LastReadMessages, BounceFlag, LastSent)
      VALUES(
       '%s', '%s', '%s', '%s', '%s', '%s',
       '%s', '%s', '%s', '%s', %d, %d,
       %d, '%s', '%s', %d, '%s',
       %d, %d, %d, 'n', '%s',
       '%s', '%s', '%s', %d, '%s'
      ) 
       ", 
        $row['User_ID'], $row['FirstName'], $row['LastName'], $row['Email'],
	$row['Homepage'], $row['Description'], $row['Gender'], $row['Age'],
	$row['Country'], $row['PhotoUrl'], $row['ShowNobody'], $row['ShowFriend'],
	$row['ShowAnyone'], $row['PopupPos'], $row['NS6Sidebar'], $row['ResPerPage'],
	$row['HighlightSearch'], $row['IfaceLang'], $row['Langs'], $row['NotificationStyle'],
	$row['MypageAfterLogon'], $row['LastOnline'], $row['NewWindow'], $row['LastReadMessages'],
	$row['BounceFlag'], $row['LastSent']
       );
   // die($query."\n\n");   
    echo("\tAdding UID: ".$row['User_ID']."\n");
    pg_exec($postgres, $query);		     
 }
 mysql_free_result ($res);

 printf("\nWorking on the directory...\n");
 $query = "DELETE FROM ltrDirectory";
 pg_exec($postgres, $query) or die("Could not clean the directory\n");
 $query = "SELECT ltrDirectory.*, ltrDirectoryData.*, FROM_UNIXTIME(UNIX_TIMESTAMP(ChangeDate)) as CompDate FROM ltrDirectory LEFT JOIN ltrDirectoryData ON ltrDirectory.Node_ID = ltrDirectoryData.Node_ID";
 $res = mysql_query($query) or die("Could not query the directory\n");
 $direntry = array();
 while ($row = mysql_fetch_array ($res)) {
   foreach($row as $key => $value)
    $row[$key] = addslashes($value);
   if ($row['AddDate'] == '') $row['AddDate'] = $row['CompDate'];
   $direntries[] = $row;
 }
 printf("\tRead data...\n");
 foreach($direntries as $direntry){
  $direntry['LinkTo'] = path2id($direntry['LinkTo']);
  if ($direntry['IntNode'] == "") $direntry['IntNode'] = "NULL";
  $direntry['Owner'] = ($direntry['Owner'] == "") ? 'NULL' : "'".$direntry['Owner']."'";
  $query = sprintf("
      INSERT INTO 
       ltrDirectory
       (Name, Node_ID, Parent, LinkTo, Level, Language, 
        IntNode, Description, UserAccess, FriendAccess, 
	ExtraLong, ChangeDate, Owner, AddDate)
      VALUES
       ('%s', %d, %d, %s, %d, %d,
        %s, '%s', %d, %d,
	%d, '%s', %s, '%s')
       ",
       $direntry['Name'], $direntry['Node_ID'], $direntry['Parent'], $direntry['LinkTo'],
       $direntry['Level'], $direntry['Language'],
       $direntry['IntNode'], $direntry['Description'], $direntry['UserAccess'], $direntry['FriendAccess'],
       $direntry['ExtraLong'], $direntry['CompDate'], $direntry['Owner'], $direntry['AddDate']);
  pg_exec($postgres, $query) or die("\nQuery-Error\n$query\n\n");	     
  printf("\tInserted ".$direntry['Name']."\n");
 }
 mysql_free_result ($res);

 printf("\nWorking on the links...\n");
 $query = "DELETE FROM ltrLinks";
 pg_exec($postgres, $query) or die("Could not clean the links\n");
 $query = "SELECT ltrLinks.*, FROM_UNIXTIME(UNIX_TIMESTAMP(ChangeDate)) as CompDate FROM ltrLinks";
 $res = mysql_query($query) or die("Could not query the links\n");
 $direntry = array();
 pg_exec($postgres, "BEGIN WORK") or die("\nCould not begin transaction\n\n");	     
 while ($row = mysql_fetch_array ($res)) {
  if ($row['Trail'] == 28)
   continue;
  if ($row['Trail'] == 41)
   continue;
  if ($row['Trail'] == 46)
   continue;
  if ($row['Trail'] == 59)
   continue;
  foreach($row as $key => $value)
   $row[$key] = addslashes($value);
  if ($row['AddDate'] == '') $row['AddDate'] = $row['CompDate'];
  if ($row['Next'] == "") $row['Next'] = 'NULL';
  
  $query = sprintf("
      INSERT INTO 
       ltrLinks
       (Link_ID, Trail, Name, Description, Url, Owner, ChangeDate, 
        AddDate, Next) 
      VALUES
       (%d, %d, '%s', '%s', '%s', '%s',
        '%s', '%s', %s)
       ",
       $row['Link_ID'], $row['Trail'], $row['Name'], $row['Description'],
       $row['Url'], $row['Owner'], $row['CompDate'], $row['AddDate'], $row['Next'] 
       );
  if (!pg_exec($postgres, $query)){
    pg_exec($postgres, "ROLLBACK WORK");
    die("\nQuery-Error\n$query\n\nTransaction rolled back\n\n");	     
  }
  printf("\tInserted ".$row['Name']."(".$row['Link_ID'].")\n");
 }
 mysql_free_result ($res);
 printf("\tCommiting work...\n");
 pg_exec($postgres, "COMMIT WORK") or die("\nCould not begin transaction\n\n");	     
 printf("\tWork commited...\n");

 printf("\nWorking on subscriptions...\n");
 $query = "DELETE FROM ltrSubscriptions";
 pg_exec($postgres, $query) or die("\tCould not clean the subscriptions table\n");
 $query = "SELECT auth_user.user_id, ltrSubscriptions.Trail FROM auth_user, ltrSubscriptions WHERE auth_user.username = ltrSubscriptions.Username";
 $res = mysql_query($query) or die("\tCould not query the subscriptions\n");
 while ($row = mysql_fetch_array ($res)) {
  if ($row['Trail'] == 32)
   continue;
  if ($row['Trail'] == 97)
   continue;
  if ($row['Trail'] == 106)
   continue;
  if ($row['Trail'] == 40)
   continue;
  if ($row['Trail'] == 526)
   continue;
  if ($row['Trail'] == 176)
   continue;
  if ($row['Trail'] == 927)
   continue;
  if ($row['Trail'] == 735)
   continue;
  $query = sprintf("INSERT INTO ltrSubscriptions VALUES ('%s', %d)",
                     $row['user_id'], $row['Trail']);
  pg_exec($postgres, $query) or die("\nQuery-Error\n$query\n\n");	     
  printf("\tInserted subscription (User= %s; Trail= %d)\n", $row['user_id'], $row['Trail']);
 }
 mysql_free_result ($res);

 printf("\nWorking on Friendships...\n");
 $query = "DELETE FROM ltrFriends";
 pg_exec($postgres, $query) or die("\tCould not clean the friendships table\n");
 $query = "SELECT Userid, IsFriendOf FROM ltrFriends";
 $res = mysql_query($query) or die("\tCould not query the friendships\n");
 while ($row = mysql_fetch_array ($res)) {
  if ( ($row['Userid'] == 'c1b984f523714d0bdf9b0bbf9bceea4e') or ($row['IsFriendOf'] == 'c1b984f523714d0bdf9b0bbf9bceea4e'))
   continue;
   
  $query = sprintf("INSERT INTO ltrFriends VALUES ('%s', '%s')",
                     $row['Userid'], $row['IsFriendOf']);
  pg_exec($postgres, $query) or die("\nQuery-Error\n$query\n\n");	     
  printf("\tInserted friendship (User1= %s; User2= %s)\n", $row['Userid'], $row['IsFriendOf']);
 }
 mysql_free_result ($res);

 printf("\nWorking on Messages...\n");
 $query = "DELETE FROM ltrMessages";
 pg_exec($postgres, $query) or die("\tCould not clean the messages table\n");
 $query = "SELECT * FROM ltrMessages";
 $res = mysql_query($query) or die("\tCould not query the messages\n");
 while ($row = mysql_fetch_array ($res)) {
  if ($row['Target'] == "") continue;
  if ($row['Target'] == "-1") $row['Target'] = "eiowjestcool15e437b61bfed3joi3os";

  foreach($row as $key => $value)
   $row[$key] = addslashes($value);
  
  $query = sprintf("
    INSERT INTO 
     ltrMessages 
    (Target, MessageType, Sender, Date, Done, Data, ReferenceCount)
    VALUES ('%s', %d, '%s', '%s', '%s', '%s', %d)
    ",
    $row['Target'], $row['MessageType'], $row['Sender'], $row['Date'], $row['Done'],
    $row['Data'], $row['ReferenceCount']);
  pg_exec($postgres, $query) or die("\nQuery-Error\n$query\n\n");	     
  printf("\tInserted message (From= %s; To= %s)\n", $row['Sender'], $row['Target']);
 }
 mysql_free_result ($res);

 printf("\nWorking on Slots...\n");
 $query = "DELETE FROM ltrSlots";
 pg_exec($postgres, $query) or die("\tCould not clean the slots table\n");
 $query = "SELECT * FROM ltrSlots";
 $res = mysql_query($query) or die("\tCould not query the messages\n");
 while ($row = mysql_fetch_array ($res)) {

  foreach($row as $key => $value)
   $row[$key] = addslashes($value);

  $row['Trail_2_ID']   = ($row['Trail_2_ID'] == "")   ? 'NULL' : $row['Trail_2_ID'];
  $row['Trail_2_Text'] = ($row['Trail_2_Text'] == "") ? 'NULL' : "'".$row['Trail_2_Text']."'";
  $row['Next']         = ($row['Next'] == "")         ? 'NULL' : "'".$row['Next']."'";
  
  $query = sprintf("
    INSERT INTO 
     ltrSlots 
    (Node_ID, Trail_1_ID, Trail_1_Text, Trail_2_ID, Trail_2_Text, Description, Next, IsLive)
    VALUES 
    (%d, %d, '%s', %s, %s, '%s', %s, '%s')
    ",
    $row['Node_ID'], $row['Trail_1_ID'], $row['Trail_1_Text'], $row['Trail_2_ID'],
    $row['Trail_2_Text'], $row['Description'], $row['Next'], $row['IsLive']);
  pg_exec($postgres, $query) or die("\nQuery-Error\n$query\n\n");	     
  printf("\tInserted Slots (Node %d)\n", $row['Node_ID']);
 }
 mysql_free_result ($res);


echo("\nconversion done!\n\n");
?>
