#!/usr/bin/perl

# See Note 1 in the instructions-f-to-f-ex.txt file. Use your text editor's search function
# to quickly search for these notes

use CGI::Request;
$req = new CGI::Request;
$req->import_names('FORM');


# Note 2

if (!$FORM::QTY  || !$FORM::SUBTOTAL || !$FORM::SHIPPING_COST || !$FORM::TOTAL) {
  &cgi_header(); 
  print "<b>Please enter the correct quantity, subtotal, shipping cost, and total; click on your BACK button to enter this information.</b>" ;
  exit;
}


if (!$FORM::LAST_NAME  || !$FORM::ADDRESS_1 || !$FORM::CITY) {
  &cgi_header(); 
  print "<b>You need to fill in a Name and Address; click on your BACK button to enter this information.</b>" ;
  exit;
}


if (!$FORM::CARD_TYPE  || !$FORM::CARD_NUMBER || !$FORM::EXPIRY_DATE) {
  &cgi_header(); 
  print "<b>Please enter your credit card information: the type of card, the card number, and the expiration date. Click on your BACK button to enter this information.</b>" ;
  exit;
}


# Note 3

open(SENDMAIL,">>order_ex.txt") || &err_exit(400,"cannot run sendmail");


# Note 4

print SENDMAIL qq|"$FORM::QTY","$FORM::SUBTOTAL","$FORM::TAX","$FORM::SHIPPING","$FORM::SHIPPING_COST","$FORM::TOTAL","$FORM::TITLE","$FORM::FIRST_NAME","$FORM::LAST_NAME","$FORM::COMPANY_NAME","$FORM::ADDRESS_1","$FORM::ADDRESS_2","$FORM::CITY","$FORM::STATE","$FORM::COUNTRY","$FORM::ZIP","$FORM::EMAIL","$FORM::TELEPHONE","$FORM::CARD_TYPE","$FORM::CARD_NUMBER","$FORM::EXPIRY_DATE"|;

close(SENDMAIL);


# Note 5

&cgi_header();
print << "EndResponse";
<h3>Thank you very much for your order. You provided the following information:</h3>

<table border="1" width="50%" bordercolor="#000000">
  <tr>
    <td><strong>Quantity:&nbsp;&nbsp; </strong></td>
    <td>$FORM::QTY</td>
  </tr>
  <tr>
    <td><strong>Subtotal:&nbsp;&nbsp; </strong></td>
    <td>$FORM::SUBTOTAL</td>
  </tr>
  <tr>
    <td><strong>Tax:&nbsp;&nbsp; </strong></td>
    <td>$FORM::TAX</td>
  </tr>
  <tr>
    <td><strong>Shipping:&nbsp;&nbsp; </strong></td>
    <td>$FORM::SHIPPING</td>
  </tr>
  <tr>
    <td><strong>Shipping Cost:&nbsp;&nbsp; </strong></td>
    <td>$FORM::SHIPPING_COST</td>
  </tr>
  <tr>
    <td><strong>Total:&nbsp;&nbsp; </strong></td>
    <td>$FORM::TOTAL</td>
  </tr>
  <tr>
    <td><strong>Title:&nbsp;&nbsp; </strong></td>
    <td>$FORM::TITLE</td>
  </tr>
  <tr>
    <td><strong>First Name:&nbsp;&nbsp; </strong></td>
    <td>$FORM::FIRST_NAME</td>
  </tr>
  <tr>
    <td><strong>Last Name:&nbsp;&nbsp; </strong></td>
    <td>$FORM::LAST_NAME</td>
  </tr>
  <tr>
    <td><strong>Company:&nbsp;&nbsp; </strong></td>
    <td>$FORM::COMPANY_NAME</td>
  </tr>
  <tr>
    <td><strong>Address:&nbsp;&nbsp; </strong></td>
    <td>$FORM::ADDRESS_1</td>
  </tr>
  <tr>
    <td><strong>Address2:&nbsp;&nbsp; </strong></td>
    <td>$FORM::ADDRESS_2</td>
  </tr>
  <tr>
    <td><strong>City:&nbsp;&nbsp; </strong></td>
    <td>$FORM::CITY</td>
  </tr>
  <tr>
    <td><strong>State:&nbsp;&nbsp; </strong></td>
    <td>$FORM::STATE</td>
  </tr>
  <tr>
    <td><strong>Zip:&nbsp;&nbsp; </strong></td>
    <td>$FORM::ZIP</td>
  </tr>
  <tr>
    <td><strong>Country:&nbsp;&nbsp; </strong></td>
    <td>$FORM::COUNTRY</td>
  </tr>
  <tr>
    <td><strong>E-mail:&nbsp;&nbsp; </strong></td>
    <td>$FORM::EMAIL</td>
  </tr>
  <tr>
    <td><strong>Telephone:&nbsp;&nbsp; </strong></td>
    <td>$FORM::TELEPHONE</td>
  </tr>
  <tr>
    <td><strong>Card Type:&nbsp;&nbsp; </strong></td>
    <td>$FORM::CARD_TYPE</td>
  </tr>
  <tr>
    <td><strong>Card#:&nbsp;&nbsp; </strong></td>
    <td>$FORM::CARD_NUMBER</td>
  </tr>
  <tr>
    <td><strong>Expiration Date:&nbsp;&nbsp; </strong></td>
    <td>$FORM::EXPIRY_DATE</td>
  </tr>
  <tr>
    <td colspan="2"><strong></strong>If you have any corrections or questions about your
    order, please e-mail <a href="mailto:customerservice\@topfloor.com">customerservice\@topfloor.com</a>
    <strong></strong></td>
  </tr>
</table>




EndResponse
exit;


# Note 6


sub cgi_header {
    local($window) =@_;
    $window && (print "Window-target: $window\n");
    print "Content-type: text/html\n";
    print "\n";
}

sub err_exit {
    local($exitcode,$reason) = @_;
    print "Status: $exitcode\n\n";
    $reason && print "$reason\n";
    exit;
}

