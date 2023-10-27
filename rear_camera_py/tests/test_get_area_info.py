"""ブロックエリア情報を取得するクラスのテスト.

@author: kawanoichi
"""
from src.get_area_info import GetAreaInfo

import os
import unittest
import numpy as np

script_dir = os.path.dirname(os.path.abspath(__file__))
# PROJECT_DIR_PATH = os.path.dirname(script_dir)


def delete_img(path):
    """ファイルが存在していたら削除."""
    if os.path.exists(path):
        os.remove(path)


class TestRoboSnap(unittest.TestCase):
    def setUp(self):
        """前処理."""
        # テスト用画像のあるディレクトリパス
        self.test_image_dir = os.path.join(script_dir, "test_data", "block_area_img")
        self.test_images = os.listdir(self.test_image_dir)

    def test_start(self):
        """ロボコンスナップ攻略クラスのテスト."""
        for img_name in self.test_images:
            # "_", "_0.png"を削除
            img = img_name[:19].replace('_', '')
            expect_info = [int(img[i]) for i in range(len(img))]
            expect_info = np.array(expect_info).reshape(4, 4)

            expect_info[:3, :] = 0  # 完成したらいらないはずの処理
            # expect_info[2, 3] = int(img[4*3-1])  # 完成したらいらないはずの処理

            info = GetAreaInfo(img_name, self.test_image_dir, develop=False)
            course_info = info.get_area_info(isR=True)

            if np.array_equal(expect_info, course_info):
                assert True
            else:
                print(f"expect_info\n: {expect_info}")
                print(f"course_info\n: {course_info}")
                assert False
