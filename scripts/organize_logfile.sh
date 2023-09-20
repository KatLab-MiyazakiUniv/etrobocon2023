#!/bin/sh
<<DOCUMENT_COMMENT
@file   organize_logfile.sh
@brief  前回のログファイルを退避する
@author desty505 miyashita64
DOCUMENT_COMMENT

mkdir -p etrobocon2023/logfiles
oldName="logfile.txt"
newName=`date +"%m%d-%H:%M.txt"`

mv -f $oldName etrobocon2023/logfiles/$newName