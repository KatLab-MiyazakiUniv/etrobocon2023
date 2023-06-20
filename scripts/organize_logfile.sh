#!/bin/sh

mkdir -p logfiles
oldName="logfile.txt"
newName=`date +"%m%d-%H:%M.txt"`

mv -f $oldName etrobocon2023/logfiles/$newName