import os
from flask import Flask, url_for

app = Flask(__name__)


# ルーティングを設定
@app.route("/")

def send():
# 送信するデータファイルのパスを設定 
    path = "scripts/data.csv"

# データを取得
    f = open(path)
    data = f.read()
    f.close()

# 値の送信
    return str(data)

# ポート番号の設定
if __name__ == "__main__":
    app.run(host='192.168.11.16', port=8000, debug=True)