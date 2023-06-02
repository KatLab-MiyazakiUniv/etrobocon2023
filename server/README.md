# flaskサーバ
調整用IOSアプリと通信する用のflaskサーバプログラムです。

## データ送信
'''
$ curl -X POST -H "Content-Type: text/plain; charset = utf-8" -d '@scripts/"送信するcsvファイル"' http://"サーバIPアドレス":8000
'''

## データ受信
'''
$ curl -o scripts/"受信データを書き出すcsvファイル" http://"サーバIPアドレス":8000/
'''