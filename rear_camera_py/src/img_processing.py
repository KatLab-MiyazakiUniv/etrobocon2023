"""画像加工モジュール"""

import numpy as np
import cv2
import os


# from get_area_info2 import Color, GetAreaInfo


script_dir = os.path.dirname(os.path.abspath(__file__))  # /src
PROJECT_DIR_PATH = os.path.dirname(script_dir)  # /rear_camera_py


class ImageProcess:
    @staticmethod
    def change_color(hsv_img, write_img, search_color, color_value):
        """一定の範囲のHSV値を指定したHSV値に統一する関数.

        Args:
            hsv_img : HSV値に変換した画像
            write_img: 統一した値を書き込む画像
            search_color: 一定の範囲を示すHSV値 (self.RED1などを指定)
            color_value: 統一するHSV値 (Color.RED.valueなど)

        Return:
            色変換画像
        """
        write_img[np.where((search_color[0][0] <= hsv_img[:, :, 0]) &
                           (hsv_img[:, :, 0] <= search_color[0][1]) &
                           (search_color[1][0] <= hsv_img[:, :, 1]) &
                           (hsv_img[:, :, 1] <= search_color[1][1]) &
                           (search_color[2][0] <= hsv_img[:, :, 2]) &
                           (hsv_img[:, :, 2] <= search_color[2][1]))
                  ] = color_value
        return write_img


if __name__ == "__main__":
    import time
    import argparse
    print("Image Process Start")
    start = time.time()

    work_dir_path = os.path.join(PROJECT_DIR_PATH, "tests", "test_data", "block_area_img")
    save_dir_path = os.path.join(PROJECT_DIR_PATH, "work_image_data")

    # impro = ImgProcess()

    execute_time = time.time() - start
    print(f"実行時間: {str(execute_time)[:5]}s")
    print("Finish")
