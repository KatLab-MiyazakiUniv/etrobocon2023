import requests
import csv
import split

# flaskのwebサーバに接続する
url = "http://127.0.0.1:8000/read"

# サーバに送られているデータを文字列(str型)で受け取る
resultText = requests.get(url)
result = resultText

# 受け取ったデータをリストに変換する
rData = result.split( )


path = "scripts/result.csv"

# 1行の内容をCSVファイルに書き込み
with open(path, 'w', encoding='utf-8', newline="") as f:
    writer = csv.writer(f)
    writer.writerow(rData)
f.close()


print("written")