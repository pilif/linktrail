#!/bin/sh


echo "Removing cached files..."
rm -f /thomas_linktrail/html/img/slots/*
rm -f /thomas_linktrail/cache/directory/subnodes/*
cd /home/linktrai/
echo "Copying files..."
cp -R cache/ html/ includes/ phplib/ scripts/ static_pages/ templates/ /thomas_linktrail/
echo "Backup done: " `date +%D`
