'''ロボットの走行情報を提供するWebサーバ.

 @author  desty505 aridome222 miyashita64
'''
import os
import csv
import re
import pathlib
from flask import Flask, request

# ロボットの走行情報
robot_info = {
    # state
    #   - notReady: 通信未確立
    #   - wait: 開始合図待ち
    #   - start: 走行開始
    #   - lap: LAPゲート通過
    #   - finish: 処理停止
    "state": "notReady"
}

# 撮影終了フラグ用ファイルのパス
SKIP_FLAG_FILE = "skip_camera_action.flag"

app = Flask(__name__)


# '/robot_info/init'へのGETリクエストに対する操作
@app.route('/robot_info/init', methods=["GET"])
def init() -> None:
    """ロボットの走行情報を初期化する."""
    robot_info["state"] = "notReady"
    try:
        # ファイルを削除
        os.remove(SKIP_FLAG_FILE)
        print(f"Remove the file '{SKIP_FLAG_FILE}'.")
    except OSError as e:
        print(f"OS error: {e}")
    return str(robot_info)

# '/robot_info/state'へのGETリクエストに対する操作
@app.route('/robot_info/state', methods=["GET"])
def getState() -> str:
    """ロボットの状況を返す.
    
    Returns:
        robot_info["state"] (str):  サーバが持つロボットの状況
    """
    return robot_info["state"]

# '/robot_info/state'へのPOSTリクエストに対する操作
@app.route('/robot_info/state', methods=["POST"])
def setState() -> str:
    """ロボットの状況を設定する.
    
    Returns:
        robot_info["state"] (str):  サーバが持つロボットの状況
    """
    # request.get_data()を使って送信されたデータを受け取る
    request_data = request.get_data()
    # byte型のデータ"request_data"をstr型の文字列"request_data"にデコードを行う
    charset = 'UTF-8'
    request_text = request_data.decode(charset, 'replace')
    # ロボットの状況を更新する
    robot_info["state"] = request_text
    return robot_info["state"]

# '/robot_info/skip_camera_action_true'へのPOSTリクエストに対する操作
@app.route('/robot_info/skip_camera_action_true', methods=["POST"])
def setTrueSkipCameraAction() -> None:
    """撮影動作終了フラグを立てる."""
    # C++側から確認できるようファイルを生成する(C++はファイルの有無で判断)
    skip_camera_action_flag_file = pathlib.Path(SKIP_FLAG_FILE)
    skip_camera_action_flag_file.touch()
    try:
        # 撮影終了フラグとして、空ファイルを作成する
        with open(SKIP_FLAG_FILE, 'w') as file:
            pass
        print(f"Touch the file '{SKIP_FLAG_FILE}'.")
    except IOError as e:
        print(f"I/O error: {e}")
    return "skip" # 何かしらの値を返す必要があるため任意の値を設定


# ポート番号の設定
if __name__ == "__main__":
    ip = "127.0.0.1"
    host = os.uname()[1]
    if host == "katlab2":
        ip = "172.20.1.1"
    app.run(host=ip, port=8000)
