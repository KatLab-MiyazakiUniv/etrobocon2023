"""画像処理を行うモジュール.

@author: kawanoichi
"""
import cv2
import numpy as np
import os


class ImageProcessing:
    """画像処理を行うクラス.

    NOTE:
        以下の関数を保持.
        ・画像の鮮明化(sharpen_image)
        ・画像のリサイズ(resize_img)
    """

    @staticmethod
    def sharpen_image(img) -> np.ndarray:
        """画像の鮮明化を行う関数.

        手法：カラー画像のアンシャープマスク

        Args:
            image_path(str): 鮮明化する画像パス
            save_path(str): 鮮明化した画像の保存先パス
                            パス指定がない場合保存しない
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

    @staticmethod
    def resize_img(img,
                   resize_w=640,
                   resize_h=480) -> np.ndarray:
        """一枚の画像をリサイズ.

        Args:
            img_path (str): リサイズする画像のパス
            save_path (str): リサイズした画像を保存するパス
                             パス指定がない場合保存しない
            resize_w (int): リサイズする画像の幅
            resize_h (int): リサイズする画像の高さ

        Return:
            result(numpy.ndarray): リサイズした画像

        Raises:
            FileNotFoundError: 画像が見つからない場合に発生

        """
        try:
            # リサイズ
            resized_img = cv2.resize(img, (resize_w, resize_h))

        except FileNotFoundError as e:
            print("Error:", e)
            return None

        return resized_img


if __name__ == '__main__':
    """作業用.
    $ poetry run python ./src/image_processing.py --resize 640 480 --sharpen
    """
    import argparse
    from pathlib import Path

    script_dir = os.path.dirname(os.path.abspath(__file__))
    IMAGE_DIR_PATH = os.path.join(script_dir, "..", "fig_image")
    IMAGE_DIR_PATH = Path(IMAGE_DIR_PATH)

    parser = argparse.ArgumentParser(description="リアカメラに関するプログラム")

    parser.add_argument("--sharpen", action='store_true',
                        help='重みファイルパス')
    parser.add_argument("--resize", nargs='+', type=int,
                        help='リサイズする画像サイズ(高さ, 幅)')
    parser.add_argument("-img", "--img_path", type=str,
                        default=IMAGE_DIR_PATH/'test_image.png', help='入力画像')
    parser.add_argument("-spath", "--save_path", type=str,
                        default=IMAGE_DIR_PATH/'processed_test_image.png',
                        help='画像処理後の保存先パス')

    args = parser.parse_args()

    if args.sharpen:
        ImageProcessing.sharpen_image(str(args.img_path), str(args.save_path))
        args.img_path = args.save_path

    if args.resize:
        ImageProcessing.resize_img(str(args.img_path),
                                   str(args.save_path),
                                   args.resize[0],
                                   args.resize[1])
