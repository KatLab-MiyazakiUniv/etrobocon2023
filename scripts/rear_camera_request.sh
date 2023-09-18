#!/bin/bash
<<DOCUMENT_COMMENT
@file   rear_camera_request.sh
@brief  リアカメラキャリブレーション/角度補正を行う
@author 
@note
    キャリブレーション: bash ./rear_camera_request.sh --calibrate
    角度補正:          bash ./rear_camera_request.sh {ポート番号}
DOCUMENT_COMMENT

# デフォルト値
port=10338

# キャリブレーションの実行
if [[ "${1}" == "--calibrate" ]]; then
    bash -c "echo calibrate | nc 127.0.0.1 ${port}"
    echo ''  # 改行をする
    exit 0
fi

# 角度補正の場合
if [[ "${1}" != "" ]]; then
    port=${1}
fi

# サーバにangleコマンドを送信
bash -c "echo angle | nc 127.0.0.1 ${port}"