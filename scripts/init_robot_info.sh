#!/bin/bash
<<DOCUMENT_COMMENT
@file   init_robot_info.sh
@brief  Webサーバが持つ走行情報を初期化する
@author miyashita64
@note
    $ bash init_robot_info.sh {RasPIのIP(任意)}
DOCUMENT_COMMENT

# Bluetooth接続する場合のRasPiのIP(デフォルト値)
IP="172.20.1.1:8000"

# IPが渡された場合、指定IPにリクエストを送る
if [ -n ${$1} ]; then
    IP=$1
fi

curl http://${IP}/robot_info/init
