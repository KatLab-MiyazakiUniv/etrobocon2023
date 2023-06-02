# flaskサーバ
調整用IOSアプリと通信する用のflaskサーバプログラムです。

## データ送信
curl -X POST -H "Content-Type: text/plain; charset = utf-8" -d '@scripts/data2.csv' http://<サーバIPアドレス>:8000

## データ受信
curl -o scripts/result.csv http://<サーバIPアドレス>:8000/