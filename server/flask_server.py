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

def server():

    request_data = request.get_data()
    charset = request.mimetype_params.get('charset') or 'UTF-8'
    request_text = request_data.decode(charset, 'replace')


    if len(request_text) != 0:

        data_list = re.split('[ ,]', request_text)

        change_path = "scripts/data.csv"

        with open(change_path, 'w', encoding='utf-8', newline="") as file:
            writer = csv.writer(file)
            writer.writerow(data_list)
            file.close()

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