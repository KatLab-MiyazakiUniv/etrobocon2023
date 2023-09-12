'''
 RasPiにWebサーバを設置する 
 @auther  desty505 aridome222
'''
import os
import csv
import re
from flask import Flask, request
import multiprocessing.shared_memory as sm
import ctypes


# サーバが扱うファイルの設定
FILE_PATH = "datafiles/data.csv"

class SystemInfo(ctypes.Structure):
    """ C言語と共有する走行体情報のデータ型
        state: char[16]
        - notReady: 通信未確立
        - wait:     開始合図待ち
        - start:    走行開始
        - lap:      LAPゲート通過
        - finish:   処理停止

        skip_camera_action: bool
        - True:     撮影動作をスキップする
        - False:    撮影動作を行う
    """
    _fields_ = [("state", ctypes.c_char * 16),
                ("skip_camera_action", ctypes.c_bool)]

# 走行体情報用の共有メモリを設定
shm = sm.SharedMemory(name="system_info_memory", create=True, size=ctypes.sizeof(SystemInfo))
# 共有メモリにアクセスするためのビューを生成
system_info_buf = SystemInfo.from_buffer(shm.buf)
# 走行体情報を初期化
system_info_buf.state = "noReady".encode("utf-8")
system_info_buf.skip_camera_action = False

# Webサーバを定義
app = Flask(__name__)


# '/'へのGETリクエストに対する操作
@app.route('/', methods=["GET"])
def read() -> str:
    """GETリクエストに対しcsvファイルの内容を返す.
    
    Returns:
        str:  サーバが持つcsvファイルの内容
    """
    # データを取得
    file = open(FILE_PATH)
    open_data = file.read()
    file.close()

    # 値の送信
    return open_data

# '/'へのPOSTリクエストに対する操作
@app.route('/', methods=["POST"])
def write() -> str:
    """POSTリクエストに対しcsvファイルの内容を更新して、返す.
    
    Returns:
        str:  サーバが持つcsvファイルの内容
    """
    # request.get_data()を使って送信されたデータを受け取る
    request_data = request.get_data()

    # byte型のデータ"request_data"をstr型の文字列"request_data"にデコードを行う
    charset = 'UTF-8'
    request_text = request_data.decode(charset, 'replace')

    # データを受け取ったかどうかを判定する
    if len(request_text) != 0:
        
        # 文字列を" "および","で区分けする
        data_list = re.split('[ ,]', request_text)

        # 受け取ったデータをファイルに上書きで保存
        with open(FILE_PATH, 'w', encoding='utf-8', newline="") as file:
            writer = csv.writer(file)
            writer.writerow(data_list)
            file.close()

    # データを取得
    file = open(FILE_PATH)
    open_data = file.read()
    file.close()

    # 値の送信
    return open_data

# '/system_info/state'へのGETリクエストに対する操作
@app.route('/system_info/state', methods=["GET"])
def send() -> str:
    """ロボットの走行状況を返す.

    Returns:
        system_info_buf.state (str):  サーバが持つロボットの状況
    """
    # 値の送信
    return system_info_buf.state

# '/system_info/skip_camera_action'へのPOSTリクエストに対する操作
@app.route('/system_info/skip_camera_action', methods=["POST"])
def set_skip_camera_action() -> str:
    """撮影動作スキップフラグをTrueにする.
    
    Returns:
        system_info_buf.skip_camera_action (bool):  撮影動作スキップフラグ
    """
    # 共有メモリへ書き込み
    system_info_buf.skip_camera_action = True
    return system_info_buf.skip_camera_action


if __name__ == "__main__":
    # サーバ情報の設定
    ip = "127.0.0.1"
    host = os.uname()[1]
    if host == "katlab":
        ip = "192.168.11.16"
    elif host == "katlab2":
        ip = "192.168.11.17"
    # 走行体情報を提供するWebサーバを起動する
    try:
        # Webサーバを起動
        app.run(host=ip, port=8000)
    finally:
        # 共有メモリ用のビューを削除(共有メモリを閉じる前に削除する必要がある)
        del system_info_buf
        # 共有メモリを閉じて削除
        shm.close()
        shm.unlink()
