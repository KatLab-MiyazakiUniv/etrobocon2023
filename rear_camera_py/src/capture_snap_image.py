"""ロボコンスナップのために画像を取得し、加工するモジュール.

@author: kawanoichi
"""
import argparse
import cv2
import numpy as np
import os

from camera_interface import CameraInterface


class CaptureSnapImage:
    """走行体側ロボコンスナップクラス."""

    resize = (640, 480)

    def __init__(self, camera_id=0, img_name="FigA_1.png"):
        """コンストラクタ."""
        self.camera_id = camera_id
        self.img_name = img_name

        # 保存用フォルダの作成
        current_path = os.path.dirname(os.path.abspath(__file__))
        parent_path = os.path.dirname(current_path)
        folder_path = os.path.join(parent_path, "image_data")
        if not os.path.exists(folder_path):
            os.mkdir(folder_path)

        self.save_path = os.path.join(folder_path, self.img_name)

    def sharpen_image(self, img) -> np.ndarray:
        """画像の鮮明化を行う関数.

        手法：カラー画像のアンシャープマスク

        Args:
            img (np.ndarray): 鮮明化する画像
        Return:
            result(np.ndarray): 鮮明化後画像
        Raises:
            FileNotFoundError: 画像が見つからない場合に発生
        """
        try:
            # アンシャープマスクを適用
            blurred = cv2.GaussianBlur(img, (0, 0), 2)  # ぼかし処理
            # 引数: 元画像, 元の画像に対する加重係数（強度）
            #       ブラー画像, ブラー画像に対する減重係数(強度), 画像の明るさ(0は無視)
            result = cv2.addWeighted(img, 2.5, blurred, -1.5, 0)  # 差分から鮮明化

        except FileNotFoundError as e:
            print("Error:", e)
            return None

        return result

    def resize_img(self, img) -> np.ndarray:
        """一枚の画像をリサイズ.

        Args:
            img (np.ndarray): リサイズする画像
        Return:
            result(numpy.ndarray): リサイズ後画像
        Raises:
            FileNotFoundError: 画像が見つからない場合に発生
        """
        try:
            # リサイズ
            resized_img = cv2.resize(img, (self.resize[0], self.resize[1]))

        except FileNotFoundError as e:
            print("Error:", e)
            return None

        return resized_img

    def capture(self) -> None:
        """画像取得し、加工する."""
        try:
            # 画像取得
            camera = CameraInterface(camera_id=self.camera_id)
            camera.start_camera()
            img = camera.capture_image()

            # 型変換
            img = img[:, :, ::-1]
            img = cv2.cvtColor(np.array(img), cv2.COLOR_RGB2BGR)

            # 鮮明化
            sharpen_img = self.sharpen_image(img)

            # リサイズ
            resize_img = self.resize_img(sharpen_img)

            # 保存
            cv2.imwrite(self.save_path, resize_img)

        except FileNotFoundError as e:
            print("Error:", e)
            print("Failed to CaptureSnapImage.capture()")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="リアカメラに関するプログラム")
    parser.add_argument("--camera_num", type=int, default=0,
                        help="カメラIDを指定する")
    parser.add_argument("-spath", "--save_path", type=str, default=None,
                        help="保存する画像の名前を指定")
    args = parser.parse_args()

    snap = CaptureSnapImage(camera_id=args.camera_num,
                            img_name=args.save_path)

    snap.capture()
