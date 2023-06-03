'''
 RasPiにWebサーバを設置する 
 @auther  desty505 
'''
import os
import csv
import re
from flask import Flask, url_for, request

app = Flask(__name__)


# ルーティングを設定
@app.route('/', methods=["POST", "GET"])


def server():
    """関数の簡単な説明.
    関数の詳細
    server()は"http://"サーバIPアドレス":8000/"にアクセスがされたときに実行される
    
    Returns:
        str型:  サーバが持つデータ
                GETリクエストを受け取った際の返信として送られるデータ
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
