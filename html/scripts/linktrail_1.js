<? 
 header("Content-Type: application/x-javascript");
 if (empty($pp))
  $pp="l";
?>

function openIt(url){
 xcord = 200;
 ycord = 200;

 <?php if ($pp == "l"){ ?>
 while (xcord > 30)
  xcord = Math.ceil(Math.random()*100);
 <?php }else{ ?>
 xcord = (screen.width - 255);
 <?php } ?>

 while (ycord > 30)
  ycord = Math.ceil(Math.random()*100);

 prefs = "width=225,";
 prefs = prefs + "height=525,";
 prefs = prefs + "scrollbars=yes,";
 prefs = prefs + "resizable=yes,";
 prefs = prefs + "toolbar=no,";
 prefs = prefs + "location=no,";
 prefs = prefs + "directories=no,";
 prefs = prefs + "status=no,";
 prefs = prefs + "menubar=no,";
 prefs = prefs + "copyhistory=no,left="+xcord+",top="+ycord+",screenX="+ xcord +",screenY="+ ycord;
  

 //trailwnd = window.open(url,'trail_' + Math.ceil(Math.random()*10000), prefs); 
 trailwnd = window.open(url,'trail222', prefs); 
 trailwnd.focus();
 }
