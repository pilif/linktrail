#!/usr/bin/perl

use MIME::Base64;

$decoded = <STDIN>;
print "=>" . decode_base64($decoded). "\n";

