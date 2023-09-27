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
import PIL
import cv2

from datetime import datetime
import os
import argparse
import time

from image_processing import ImageProcessing

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

    def capture_image(self, save_path=None, resize=False) -> PIL.Image.Image:
        """カメラで画像を取得する関数.

        Arg:
            save_path (str):保存パス
                            指定がない場合保存しない
            resize(int, int):リサイズする大きさ
            
        Returns:
            <class 'PIL.Image.Image'>: カメラ画像データ
        """
        # 画像取得
        img = self.__picam2.capture_array()
        opencv_image = cv2.cvtColor(np.array(img), cv2.COLOR_RGB2BGR)

        processed_img_path = ImageProcessing.sharpen_image(opencv_image)

        # リサイズ
        processed_img_path = ImageProcessing.resize_img(processed_img_path)

        # # リサイズ
        # if not resize == False:
        #     img.resize(resize)

        # 保存
        if save_path is not None:
            cv2.imwrite(save_path, processed_img_path)

        return img


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="リアカメラに関するプログラム")
    parser.add_argument("--camera-num", type=int, default=0,
                        help="カメラIDを指定する")
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
    camera = CameraInterface(camera_id=args.camera_num)
    camera.start_camera()

    # １枚だけリアカメラで画像を取得する
    if args.save_path is not None:
        save_path = os.path.join(folder_path, args.save_path)
    else:
        file_name = datetime.now().strftime("%Y-%m-%d_%H-%M-%S") + ".png"
        save_path = os.path.join(folder_path, file_name)

    camera.capture_image(save_path, resize=(640, 480))
