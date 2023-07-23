from picamera2 import Picamera2

# カメラオブジェクトを作成
camera = Picamera2()

# カメラ情報を出力
print("カメラ解像度:", camera.resolution)
print("カメラフレームレート:", camera.framerate)

# カメラオブジェクトを終了
camera.close()
