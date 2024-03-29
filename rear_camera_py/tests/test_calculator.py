"""ブロックエリア情報取得時に使用する計算クラスのテスト.

@author: kawanoichi
"""
import unittest
import numpy as np

from src.line_calculator import LineCalculator


class TestCalculator(unittest.TestCase):

    def test_calc_from_coordi_2(self):
        result = LineCalculator.calc_from_coordi_2(2, 3, 5, 6)
        self.assertEqual(result, (1.0, 1.0))

        result = LineCalculator.calc_from_coordi_2(2, 3, 2, 3)
        self.assertEqual(result, (None, None))

    def test_calc_from_coordi_4(self):
        result = LineCalculator.calc_from_coordi_4(0, 0, 0, 0, 0, 0, 0, 0)
        self.assertEqual(result, (None, None))

        result_a, result_b = LineCalculator.calc_from_coordi_4(2, 3, 5, 6, 0, 0, 0, 0)
        self.assertEqual((round(result_a), round(result_b)), (1, 1))

        result_a, result_b = LineCalculator.calc_from_coordi_4(1, 1, 2, 2, 3, 3, 4, 4)
        self.assertEqual((round(result_a), round(result_b)), (1, 0))

    def test_calc_distance_line_to_coordi(self):
        line = (1, 1)
        coordi = np.array([0, 0])
        result = LineCalculator.calc_distance_line_to_coordi(coordi, line)
        self.assertEqual(result, (1, 1))

        line = (-1, 0)
        coordi = np.array([0, 0])
        result = LineCalculator.calc_distance_line_to_coordi(coordi, line)
        self.assertEqual(result, (0, 0))

        line = (0, 0)
        coordi = np.array([0, 0])
        result = LineCalculator.calc_distance_line_to_coordi(coordi, line)
        self.assertEqual(result, (None, 0))
