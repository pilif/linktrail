<?php
 require("lib.inc.php3");
?>

<html>
<head>
<title>phpMyAdmin</title>

 <script LANGUAGE="JavaScript">
   <!--
  // These scripts were originally found on cooltype.com. 
  // Modified 01/01/1999 by Tobias Ratschiller for linuxapps.com

  document.onmouseover = doDocumentOnMouseOver ;
  document.onmouseout = doDocumentOnMouseOut ;

  function doDocumentOnMouseOver() {
    var eSrc = window.event.srcElement ;
    if (eSrc.className == "item") {
      window.event.srcElement.className = "highlight";
    }
  }

  function doDocumentOnMouseOut() {
    var eSrc = window.event.srcElement ;
    if (eSrc.className == "highlight") {
      window.event.srcElement.className = "item";
    }
  }


var bV=parseInt(navigator.appVersion);
NS4=(document.layers) ? true : false;
IE4=((document.all)&&(bV>=4))?true:false;
ver4 = (NS4 || IE4) ? true : false;

function expandIt(){return}
function expandAll(){return}
//-->
</script>

<script LANGUAGE="JavaScript1.2">
<!--
isExpanded = false;

function getIndex(el) {
	ind = null;
	for (i=0; i<document.layers.length; i++) {
		whichEl = document.layers[i];
		if (whichEl.id == el) {
			ind = i;
			break;
		}
	}
	return ind;
}

function arrange() {
	nextY = document.layers[firstInd].pageY + document.layers[firstInd].document.height;
	for (i=firstInd+1; i<document.layers.length; i++) {
		whichEl = document.layers[i];
		if (whichEl.visibility != "hide") {
			whichEl.pageY = nextY;
			nextY += whichEl.document.height;
		}
	}
}

function initIt(){
	if (NS4) {
		for (i=0; i<document.layers.length; i++) {
			whichEl = document.layers[i];
			if (whichEl.id.indexOf("Child") != -1) whichEl.visibility = "hide";
		}
		arrange();
	}
	else {
		tempColl = document.all.tags("DIV");
		for (i=0; i<tempColl.length; i++) {
			if (tempColl(i).className == "child") tempColl(i).style.display = "none";
		}
	}
}

function expandIt(el) {
	if (!ver4) return;
	if (IE4) {expandIE(el)} else {expandNS(el)}
}

function expandIE(el) { 
	whichEl = eval(el + "Child");

        // Modified Tobias Ratschiller 01-01-99:
        // event.srcElement obviously only works when clicking directly
        // on the image. Changed that to use the images's ID instead (so
        // you've to provide a valid ID!).

	//whichIm = event.srcElement;
        whichIm = eval(el+"Img");

	if (whichEl.style.display == "none") {
		whichEl.style.display = "block";
		whichIm.src = "images/minus.gif";		
	}
	else {
		whichEl.style.display = "none";
		whichIm.src = "images/plus.gif";
	}
    window.event.cancelBubble = true ;
}

function expandNS(el) {
	whichEl = eval("document." + el + "Child");
	whichIm = eval("document." + el + "Parent.document.images['imEx']");
	if (whichEl.visibility == "hide") {
		whichEl.visibility = "show";
		whichIm.src = "images/minus.gif";
	}
	else {
		whichEl.visibility = "hide";
		whichIm.src = "images/plus.gif";
	}
	arrange();
}

function showAll() {
	for (i=firstInd; i<document.layers.length; i++) {
		whichEl = document.layers[i];
		whichEl.visibility = "show";
	}
}

function expandAll(isBot) {
	newSrc = (isExpanded) ? "images/plus.gif" : "images/minus.gif";

	if (NS4) {
        // TR-02-01-99: Don't need that
        // document.images["imEx"].src = newSrc;
		for (i=firstInd; i<document.layers.length; i++) {
			whichEl = document.layers[i];
			if (whichEl.id.indexOf("Parent") != -1) {
				whichEl.document.images["imEx"].src = newSrc;
			}
			if (whichEl.id.indexOf("Child") != -1) {
				whichEl.visibility = (isExpanded) ? "hide" : "show";
			}
		}

		arrange();
		if (isBot && isExpanded) scrollTo(0,document.layers[firstInd].pageY);
	}
	else {
		divColl = document.all.tags("DIV");
		for (i=0; i<divColl.length; i++) {
			if (divColl(i).className == "child") {
				divColl(i).style.display = (isExpanded) ? "none" : "block";
			}
		}
		imColl = document.images.item("imEx");
		for (i=0; i<imColl.length; i++) {
			imColl(i).src = newSrc;
		}
	}

	isExpanded = !isExpanded;
}

with (document) {
	write("<STYLE TYPE='text/css'>");
	if (NS4)
        {
        write(".parent {font-family: Verdana, Arial, Helvetica, sans-serif; color: #000000; text-decoration:none; position:absolute; visibility:hidden; color: black;}");
        write(".child {font-family: Verdana, Arial, Helvetica, sans-serif; font-size: 8pt;color: #000000; position:absolute; visibility:hidden}");
        write(".item { color: darkblue; text-decoration:none;}");
        write(".regular {font-family: Arial,Helvetica,sans-serif; position:absolute; visibility:hidden}");
        write("DIV { color:black; }")
        }
	else
        {
        write(".child {font-family: Verdana, Arial, Helvetica, sans-serif; color: #000000; text-decoration:none; display:none}");
        write(".parent {font-family: Verdana, Arial, Helvetica, sans-serif; color: #000000; text-decoration:none;}");
        write(".item { color: darkblue; text-decoration:none; font-size: 8pt;}");
        write(".highlight { color: red; font-size: 8pt;}");
        write(".heada { font: 12px/13px; Times}");
        write("DIV { color:black; }")
	    }
	write("</STYLE>");

}

onload = initIt;

//-->
</script>
<base target="phpmain">
<style type="text/css">
//<!--
body {  font-family: Arial, Helvetica, sans-serif; font-size: 10pt}
//-->
</style>

</head>

<body bgcolor="#D0DCE0">
 <DIV ID="el1Parent" CLASS="parent">
      <A class="item" HREF="main.php3?server=<?php echo $server; ?>">
      <FONT color="black" class="heada">
      <?php echo $strHome;?>   </FONT></A>
      </DIV>
<?php
// Don't display database info if $server==0 (no server selected)
// This is the case when there are multiple servers and 
// '$cfgServerDefault = 0' is set.  In that case, we want the welcome
// to appear with no database info displayed.
if ($server > 0) {

   if (empty($dblist))
      {
      $dbs = mysql_list_dbs();
      $num_dbs = mysql_numrows($dbs);
      }
   else 
      $num_dbs = count($dblist);

   for ($i=0; $i<$num_dbs; $i++)
       {
       if (empty($dblist))
          $db = mysql_dbname($dbs, $i);
       else
          $db = $dblist[$i];
       $j = $i + 2;
    ?>
      <div ID="el<?php echo $j;?>Parent" CLASS="parent">
      <a class="item" HREF="db_details.php3?server=<?php echo $server;?>&db=<?php echo $db;?>" onClick="expandIt('el<?php echo $j;?>'); return false;">
      <img NAME="imEx" SRC="images/plus.gif" BORDER="0" ALT="+" width="9" height="9" ID="el<?php echo $j;?>Img"></a>
      <a class="item" HREF="db_details.php3?server=<?php echo $server;?>&db=<?php echo $db;?>" onClick="expandIt('el<?php echo $j;?>');">
      <font color="black" class="heada">
    <?php echo $db;?>
      </font></a>
      </div>
      <div ID="el<?php echo $j;?>Child" CLASS="child">
    <?php
       $tables = mysql_list_tables($db);
       $num_tables = @mysql_numrows($tables);
       echo '            <script language="JavaScript1.2">';
       for ($j=0; $j<$num_tables; $j++)
           {
           $table = mysql_tablename($tables, $j);
        ?>
            document.write('&nbsp;&nbsp;&nbsp;&nbsp;<a class="item" target="phpmain" HREF="tbl_properties.php3?server=<?php echo $server;?>&db=<?php echo $db;?>&table=<?php echo urlencode($table);?>"><?php echo $table;?></a><br>');
        <?php
           }
       echo "            </script>";
       echo "</div>\n";
       }
?>
<script LANGUAGE="JavaScript1.2">
<!--
if (NS4) {
	firstEl = "el1Parent";
	firstInd = getIndex(firstEl);
	showAll();
	arrange();
}
//-->
</script>
<?php
}
?>
</body>
</html>
