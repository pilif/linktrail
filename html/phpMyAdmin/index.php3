<?php
  // Process config file to determine default server (if any)
  require('lib.inc.php3');
?>

<html>
<head>
<title>phpMyAdmin</title>
</head>

<frameset cols="150,*" rows="*" border="0" frameborder="0"> 
  <frame src="left.php3?server=<?php echo $server;?>" name="nav">
  <frame src="main.php3?server=<?php echo $server;?>" name="phpmain">
</frameset>
<noframes>
<body bgcolor="#FFFFFF">

</body>
</noframes>
</html>
