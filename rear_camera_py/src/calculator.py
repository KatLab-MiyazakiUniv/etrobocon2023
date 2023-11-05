"""計算を実行するモジュール."""

import numpy as np


class Calculator:
    @staticmethod
    def calculate_line_coordi_2(x1, y1, x2, y2):
        """2点から直線の傾きと切片を求める関数."""
        print(f"x1, y1, x2, y2", x1, y1, x2, y2)
        # 傾きを計算
        a = (y2 - y1) / (x2 - x1)

        # y切片を計算
        b = y1 - a * x1

        return a, b

    @staticmethod
    def calculate_line_coordi_4(x1, y1,
                                x2, y2,
                                x3, y3,
                                x4, y4):
        """4点から直線の傾きと切片を求める関数."""
        # 傾きを計算
        coordi = np.array([[x1, y1], [x2, y2], [x3, y3], [x4, y4]])
        # y切片を計算
        a, b = np.linalg.lstsq(
            np.c_[coordi[:, 0], np.ones(4)], coordi[:, 1], rcond=None)[0]
        return a, b
