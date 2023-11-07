"""計算を実行するモジュール."""

import numpy as np


class Calculator:
    @staticmethod
    def calculate_line_coordi_2(x1, y1, x2, y2):
        """2点から直線の傾きと切片を求める関数."""
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
        coordi = np.array([[x1, y1], [x2, y2], [x3, y3], [x4, y4]])
        coordi = coordi[np.any(coordi != [0, 0], axis=1)]  # [0, 0]は削除

        # 傾きと切片を算出
        a, b = np.linalg.lstsq(
            np.c_[coordi[:, 0], np.ones(coordi.shape[0])], coordi[:, 1], rcond=None)[0]

        return a, b

    @staticmethod
    def calc_dis_line_to_coordi(coordi: np.ndarray, line: np.ndarray):
        """直線と点のx,y座標それぞれで距離を求める関数.

        Args:
            line: 直線の傾きと切片を格納した配列[a, b]
            coordi: 座標[x, y]
        Return:
            diff_x: 直線と点のx座標の差
            diff_y: 直線と点のy座標の差
        """
        a, b = line  # 傾き, 切片
        x, y = coordi

        # x座標の差
        line_x = (y - b) / a
        diff_x = np.abs(line_x - x)

        # y座標の差
        line_y = a * x + b
        diff_y = np.abs(line_y - y)

        return diff_x, diff_y
