#!/bin/bash
<<DOCUMENT_COMMENT
@file   set_state.sh
@brief  Webサーバが持つ走行状況を設定する
@author miyashita64
@note
    bash set_state.sh {走行状況(必須)} {RasPIのIP(任意)}

    走行状況:
        - notReady: 通信未確立
        - wait:     開始合図待ち
        - start:    走行開始
        - lap:      LAPゲート通過
        - finish:   ゴールライン通過(処理停止)
DOCUMENT_COMMENT

# Bluetooth接続する場合のRasPiのIP(デフォルト値)
IP="172.20.1.1:8000"

# stateが渡されたことを確認
if [ -z ${state} ]; then
    echo "Error: state is NULL"
    exit 1
fi
state=$1

# IPが渡された場合、指定IPにリクエストを送る
if [ -n ${$2} ]; then
    IP=$2
fi

curl -X POST -d ${state} http://${IP}/robot_info/state
