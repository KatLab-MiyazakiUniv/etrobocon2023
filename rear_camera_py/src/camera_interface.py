"""カメラインターフェースモジュール.

CameraInterfaceインスタンスを利用することでカメラ画像を取得することができる.
参考:
https://datasheets.raspberrypi.com/camera/picamera2-manual.pdf
https://github.com/raspberrypi/picamera2

@author kawanoichi
"""
from typing import Tuple, Union
from picamera2 import Picamera2
import numpy as np
from PIL import Image

from datetime import datetime
import os
import argparse
import time


class CameraInterface:
    """カメラインターフェースクラス."""

    def __init__(
        self,
        camera_id: int = 0,
        data_format: str = "RGB888",
        size: Tuple[int, int] = (1640, 1232)
    ) -> None:
        """カメラインターフェースのコンストラクタ.

        WARNING: キャリブレーション時と同じ解像度に設定しないとバグる.

        WARNING:
            ここでカメラデバイスを初期化すると、
            デフォルト引数は定義時評価というPythonの仕様により、
            カメラデバイスを同時に複数回初期化しようとしてエラーが発生する場合がある.
            これは、複数個所でCameraInterfaceをデフォルト引数に設定している場合、
            もしくは、1箇所デフォルト引数に設定した上で、
            さらに処理中にCameraInterfaceをインスタンス化した際に発生する可能性がある.
        """
        self.__camera_id = camera_id
        self.__format = data_format
        self.__size = size
        self.__picam2 = None

    def start_camera(self) -> None:
        """Picamera2インスタンスの初期化を行う."""
        if self.__picam2 is None:
            picam2 = Picamera2(camera_num=self.__camera_id)
            conf = picam2.create_preview_configuration(
                main={"format": self.__format, "size": self.__size})
            picam2.configure(conf)
            picam2.start()
            self.__picam2 = picam2

    def capture_image(self) -> Union[np.ndarray, None]:
        """カメラで画像を取得する関数.

        Returns:
            Union[np.ndarray, None]: カメラ画像データ
        """
        return self.__picam2.capture_array()

    def capture_save_image(self, save_path) -> None:
        """カメラで画像を取得し、保存する関数.

        Args:
            save_path (int): 画像の保存先パス(拡張子込み)
        """
        img = self.capture_image()
        # カラーチャンネルの順序を変換（BGRからRGB）
        img_rgb = img[:, :, ::-1]

        # NumPyの配列をPILの画像オブジェクトに変換
        pil_img = Image.fromarray(img_rgb)
        pil_img.save(save_path)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="リアカメラに関するプログラム")
    parser.add_argument("--camera-num", type=int, default=0,
                        help="カメラIDを指定する")
    parser.add_argument("-s", "--images",  action="store_true",
                        help="1秒ごとに撮影を実行する")
    parser.add_argument("-spath", "--save_path", type=str, default=None,
                        help="保存する画像の名前を指定")
    args = parser.parse_args()

    # 保存用フォルダの作成
    current_path = os.path.dirname(os.path.abspath(__file__))
    parent_path = os.path.dirname(current_path)
    folder_path = os.path.join(parent_path, "image_data")
    if not os.path.exists(folder_path):
        os.mkdir(folder_path)

    # 画像の取得
    camera = CameraInterface(args.camera_num)
    camera.start_camera()

    # 1秒ごとに撮影を実行する
    if args.images:
        while True:
            data_name = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
            save_path = os.path.join(folder_path, data_name+".png")
            camera.capture_save_image(folder_path+"/"+data_name+".png")
            time.sleep(1)

    # 1枚の画像を取得する
    else:
        if args.save_path is not None:
            save_path = os.path.join(folder_path, args.save_path)
        else:
            data_name = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
            save_path = os.path.join(folder_path, data_name+".png")
        camera.capture_save_image(save_path)