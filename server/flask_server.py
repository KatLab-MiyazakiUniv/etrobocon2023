'''
 RasPiにWebサーバを設置する 
 @auther  desty505 
'''

import csv
from flask import Flask, url_for
from flask import Flask, request
import re

app = Flask(__name__)


# ルーティングを設定
@app.route('/', methods=["POST", "GET"])

# server()は"http://"サーバIPアドレス":8000/"にアクセスがされたときに実行される
def server():

    # 受け取ったbyte型のデータ"request_data"をstr型の文字列"request_data"にデコードを行う
    request_data = request.get_data()
    charset = 'UTF-8'
    request_text = request_data.decode(charset, 'replace')


    if len(request_text) != 0:
        
        # 文字列を" "および","で区分けする
        data_list = re.split('[ ,]', request_text)

        # 受け取ったデータで上書きするファイルのパスを指定
        change_path = "scripts/data.csv"

        # 受け取ったデータをファイルに上書きで保存
        with open(change_path, 'w', encoding='utf-8', newline="") as file:
            writer = csv.writer(file)
            writer.writerow(data_list)
            file.close()

    # サーバに持たせるデータファイルのパスを指定
    open_path = "scripts/data.csv"

    # データを取得
    file = open(open_path)
    open_data = file.read()
    file.close()

    # 値の送信
    return open_data

# ポート番号の設定
if __name__ == "__main__":
    app.run(host='172.26.104.32', port=8000) 