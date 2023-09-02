#!/bin/bash

# 引数は3つ
# 第1引数がユーザ名、第2引数が接続先IPアドレス、第3引数が送信する画像ファイル名

# リモートマシンのユーザ名と接続先IPアドレス
# 第1引数をREMOTE_USER、第2引数をREMOTE_HOSTに格納する
REMOTE_USER="$1"
REMOTE_HOST="$2"

# 送信する画像ファイル
# 第3引数をLOCAL_IMAGEに格納
LOCAL_IMAGE="$3"

# リモート上のコピー先のパス
REMOTE_DIRECTORY="コピー先のディレクトリのパス"

# SSH経由で画像を転送
scp "../rear_camera_py/image_data/$LOCAL_IMAGE" "$REMOTE_USER@$REMOTE_HOST:$REMOTE_DIRECTORY"

# スクリプトの終了
exit 0