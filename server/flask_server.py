'''
 RasPiにWebサーバを設置する 
 @auther  desty505 
'''
import os
import csv
import re
from flask import Flask, url_for, request

app = Flask(__name__)

# GETリクエストに対する操作
@app.route('/', methods=["GET"])
def read():
    """GETリクエストに対しcsvファイルの内容を返す.
    "http://サーバIPアドレス:8000/"にGETリクエストされたときに実行される
    
    Returns:
        str:  サーバが持つcsvファイルの内容
    """

    # サーバに持たせるデータファイルのパスを指定
    open_path = "datafiles/data.csv"

    # データを取得
    file = open(open_path)
    open_data = file.read()
    file.close()

    # 値の送信
    return open_data


# POSTリクエストに対する操作
@app.route('/', methods=["POST"])
def write():
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

        # 受け取ったデータで上書きするファイルのパスを指定
        change_path = "datafiles/data.csv"

        # 受け取ったデータをファイルに上書きで保存
        with open(change_path, 'w', encoding='utf-8', newline="") as file:
            writer = csv.writer(file)
            writer.writerow(data_list)
            file.close()

    # サーバに持たせるデータファイルのパスを指定
    open_path = "datafiles/data.csv"

    # データを取得
    file = open(open_path)
    open_data = file.read()
    file.close()

    # 値の送信
    return open_data


# ポート番号の設定
if __name__ == "__main__":

    ip = "127.0.0.1"
    
    host = os.uname()[1]
    if host == "katlab":
        ip = "192.168.11.16"
    elif host == "katlab2":
        ip = "192.168.11.17"
    app.run(host=ip, port=8000)