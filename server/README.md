# flaskサーバ
調整用iOSアプリと通信する用のflaskサーバプログラムです。
server()は、"http://"サーバIPアドレス":8000/"にアクセスがされたときに実行されます。

## サーバの立て方
```
$ cd ~/etrobo/workspace/etrobocon2023/server
$ python3 flask_server.py
```

## データ送信
csvファイルの内容を送信する場合
```
$ curl -X POST -H "Content-Type: text/plain; charset = utf-8" -d '@scripts/"送信したいcsvファイル"' http://"サーバIPアドレス":8000
```

任意の文字列を送信する場合
```
$ curl -X POST -H "Content-Type: text/plain; charset = utf-8" -d '"送信したい文字列"' http://"サーバIPアドレス":8000
```


## データ受信
```
$ curl -o scripts/"受信データを書き出すcsvファイル" http://"サーバIPアドレス":8000/
```