<?php
define("FORMS_INC", true);

if (!defined("COMUTILS_INC"))
 include("dbapi/comutils.inc");

define("FIELD_NAME_LASTNAME",  'field_lastname');
define("FIELD_NAME_FIRSTNAME", 'field_firstname');
define("FIELD_NAME_EMAIL",     'field_email');
define("FIELD_NAME_COUNTRY",   'field_country');
define("FIELD_NAME_DATETIME",  'field_datetime');

define("HASH_ENTRY_LASTNAME",  'LastName');
define("HASH_ENTRY_FIRSTNAME", 'FirstName');
define("HASH_ENTRY_EMAIL",     'Email');
define("HASH_ENTRY_COUNTRY",   'Country');
define("HASH_ENTRY_DATETIME",  'DateTime');

function is_valid_field($contents, $validation_method){
 if ($validation_method == "email") return validate_email($contents);
return false;
}

function parse_form_mail_template($http_vars, $string){
 foreach($http_vars as $key => $value){
  $string = str_replace ("[[".strtoupper($key).']]', $value, $string);
 }
 return $string; 
}
?>