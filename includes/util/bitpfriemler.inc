<?php
/*
 * class bitpfriemler
 * Copyright (c) 1998-1999 by Ralf Geschke <ralf@kuerbis.org>
 *  
 * Version 0.0.2     * last update: 1999-07-17
 *
 * bitpfriemler is distributed under the GNU Library General Public License.
 *   See the lesser.txt file for details.
 * 
 * Latest bitpfriemler and further info can be found at 
 * http://kuerbis.org/bitpfriemler/
 *
 *   Bug reports can be mailed to ralf@kuerbis.org
 *
 * Changelog:
 * 1999-07-17 / 0.0.2: fixes for PHP4
 * 1999-06-17 / 0.0.1: initial public release
 *


int in MySQL:
Signed range -2147483648 - +2147483647

int unsigned in MySQL  - 4 Bytes:
4294967295

  2^
  76543210
15
23
31 = 2147483648
32 = 4294967296

== rund 30 Werte (pauschal ein guter Wert...)

  00000000
  00000001
  00000010
  00000011
  00000100
  00000101
  00000110
  00000111
  00001000


 */

define("MAX_BITPFRIEMLER",30);

class bitpfriemler
{
  var $arr;

  function bitpfriemler() // $arr = 0;
    {
      /* bei PHP4 nur noch skalare Variablen in Konstruktur moeglich,
	 daher darf hier keine Uebergabe eines Arrays mehr erfolgen */
      /*if (gettype($arr) == "array")
	return $this->import_array($arr);*/
      unset($this->arr);
      return 1;
    }
    
  
  function add_value($name,$value = 0)
    {
      if ($value) $value = 1;
      settype($value,"integer");
      settype($name,"string");
      if ((!isset($this->arr["$name"])) and count($this->arr) == MAX_BITPFRIEMLER)
	return 0;
      
      $this->arr["$name"] = $value;
      // print $this->arr["$name"] . "<br>";
      return 1;
    }
  
  
  function upd_value($name,$value = 0)
    {
      return ($this->add_value($name,$value));
    }
    
  function import_array($imparr)
    {
      if (count($imparr) > MAX_BITPFRIEMLER)
	return 0; 
      unset($this->arr);
      reset($imparr);
      while (list($key,$wert) = each($imparr))
	{
	  settype($key,"string");
	  if ($wert) $wert = 1;
	  else $wert = 0;
	  settype($wert,"integer");
	  $this->arr["$key"] = $wert;
	}
      return 1;
    }
  
  function put_int($gr)
    {
      settype($gr,"integer");
      reset($this->arr);
      while (list($key,$wert) = each($this->arr))
	{
	  $rest = $gr % 2;
	  $gr /= 2;
	  $this->arr["$key"] = $rest;
	}
    }
  
  function get_int()
    {
      $i = 0;
      $gr = 0;
      reset($this->arr);
      //for ($j = 0 ; $j <= 31 ; $j++)
      while (list($key,$wert) = each($this->arr))
	{
	  $gr += (int) pow(2.0,$i) * $wert;
	  // print "Wert: $wert / gr ist $gr<br>";
	  $i++;
	}
      return $gr;
    }
  
  function calc_int()
    {
      return $this->get_int();
    }
  
  function get_array()
    {
      return $this->arr;
    }
  
  function get_value($key)
    {
      return $this->arr["$key"];
    }
  
  function tst()
    {
      print "count in tst: " . count($this->arr) . "<br>";
      reset($this->arr);
      while (list($key,$wert) = each($this->arr))
	{
	  print "key: |$key|, value: |$wert|  <br>";
	  
	}
      print "tst says goodbye<br>";
    }
  
  

}

?>
