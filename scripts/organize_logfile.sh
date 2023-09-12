#!/bin/sh

mkdir -p etrobocon2023/logfiles
oldName="logfile.txt"
newName=`date +"%m%d-%H:%M.txt"`

cp -f $oldName etrobocon2023/logfiles/ios_logfile.txt
mv -f $oldName etrobocon2023/logfiles/$newName