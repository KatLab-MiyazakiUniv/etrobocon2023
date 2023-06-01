[![Google Test](https://github.com/KatLab-MiyazakiUniv/etrobocon2023/actions/workflows/google-test.yaml/badge.svg)](https://github.com/KatLab-MiyazakiUniv/etrobocon2023/actions/workflows/google-test.yaml)
[![Build Check](https://github.com/KatLab-MiyazakiUniv/etrobocon2023/actions/workflows/build-check.yaml/badge.svg)](https://github.com/KatLab-MiyazakiUniv/etrobocon2023/actions/workflows/build-check.yaml)

# etrobocon2023
宮崎大学片山徹郎研究室チームKatLabが作成する[ETロボコン2023](https://www.etrobo.jp/)アドバンストクラスの走行システムプログラムです。


## 開発環境
- [ETロボコン公式シミュレータ](https://github.com/ETrobocon/etrobo)
- [Raspi用 SPIKE制御開発環境「RasPike（ラスパイク）」](https://github.com/ETrobocon/RasPike)
- C++
- [GoogleTest](https://github.com/google/googletest)
- Git/GitHub/GitHub Actions

環境構築の詳細は、[wiki](https://github.com/KatLab-MiyazakiUniv/etrobocon2023/wiki)を見てください。

## Authors
KatLabメンバー, 宮崎大学片山徹郎研究室

## ビルド方法

ビルドする(開発環境、実機共通)
```
$ cd ~/etrobo/workspace/etrobocon2023
$ make build
```

走行を開始する(実機限定)
```
$ cd ~/etrobo/workspace/etrobocon2023
$ make start
```