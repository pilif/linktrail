<?php
page_open(array("sess" => "Linktrail_Session", "auth" => "Linktrail_Auth", "perm" => "Linktrail_Perm"));
include("template.inc");
mail_error($HTTP_POST_VARS);
$tpl = new Template(APPLICATION_HOME."/templates/trail", "keep");
$tpl->set_file(array("simpleframe" => "simpleframe.html"));
$tpl->set_var("CONTENT", $ltrstr['Error_Report_Sent']);
$tpl->parse("simpleframe", "simpleframe");
print $tpl->get("simpleframe");
page_close();

function mail_error($form){
$str = "";
foreach($form as $key => $value)
 $str .= "$key: $value\n";
 
mail("pilif@linktrail.com", "Fehlermeldung", $str);
}

?>                            

