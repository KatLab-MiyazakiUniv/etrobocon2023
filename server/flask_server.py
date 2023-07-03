'''
 RasPiにWebサーバを設置する 
 @auther  desty505 aridome222
'''
import os
import csv
import re
from flask import Flask, request


# サーバが扱うファイルの設定
FILE_PATH = "datafiles/data.csv"

# サーバが扱うロボットの情報
# state
#   - notReady: 通信未確立
#   - wait: 開始合図待ち
#   - start: 走行開始
#   - lap: LAPゲート通過
#   - finish: 処理停止
robot_info = {
    "state": "notReady"
}

app = Flask(__name__)


# '/'へのGETリクエストに対する操作
@app.route('/', methods=["GET"])
def read() -> str:
    """GETリクエストに対しcsvファイルの内容を返す.
    "http://サーバIPアドレス:8000/"にGETリクエストされたときに実行される
    
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
    "http://サーバIPアドレス:8000/"にPOSTリクエストされたときに実行される
    
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


# '/robot_info/state'へのGETリクエストに対する操作
@app.route('/robot_info/state', methods=["GET"])
def send() -> str:
    """GETリクエストに対しロボットの状況を返す.
    "http://サーバIPアドレス:8000/robot_info/state"にGETリクエストされたときに実行される
    
    Returns:
        robot_info["state"] (str):  サーバが持つロボットの状況
    """

    # 値の送信
    return robot_info["state"]


# '/robot_info/state'へのPOSTリクエストに対する操作
@app.route('/robot_info/state', methods=["POST"])
def update() -> str:
    """POSTリクエストに対しロボットの状況を返す.
    "http://サーバIPアドレス:8000/robot_info/state"にPOSTリクエストされたときに実行される
    
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

    # 値の送信
    return robot_info["state"]


# ポート番号の設定
if __name__ == "__main__":

    ip = "127.0.0.1"
    
    host = os.uname()[1]
    if host == "katlab":
        ip = "192.168.11.16"
    elif host == "katlab2":
        ip = "192.168.11.17"
    app.run(host=ip, port=8000)