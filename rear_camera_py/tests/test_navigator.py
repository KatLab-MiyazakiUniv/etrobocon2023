"""Navigatorクラスのテストを記述するモジュール.

@author: miyashita64
"""

import unittest

import numpy as np

from navigator import Navigator
from block_area_map import BlockAreaMap
from navigator import Navigator
from robot import Robot, Direction


class TestNavigator(unittest.TestCase):
    def test_navigate_left_course(self):
        """Lコースについて、想定しているコマンドファイルを返すかを検証する."""
        is_left_course = True
        # コース情報生成
        length = 4
        block_map = np.zeros((length, length))
        dummy_block_coords = [(3, 3), (0, 3)]
        treasure_block_coord = (0, 0)
        for (y, x) in dummy_block_coords:
            block_map[y][x] = 1
        block_map[treasure_block_coord[0], treasure_block_coord[1]] = 2

        # ブロックエリアのマップ初期化
        block_area_map = BlockAreaMap(is_left_course, block_map)
        # ナビゲーター初期化
        navigator = Navigator(block_area_map)

        # ロボット初期化
        start_robot = Robot(*(2, 0), Direction.N, [])
        end_robot = Robot(*(1, 3), Direction.E)

        # 動作を計画する
        robot = navigator.navigate(start_robot, end_robot)
        # コマンドを出力する
        actual_commands = ""
        for motion in robot.motions:
            actual_commands += f"{motion.make_command()}\n"
        expected_commands = """IS,交点内直進,
CC,RED,(1 0 N),
IS,交点内直進,
CC,RED,(0 0 N),
BT,ブロック移動,
IR,(0 0 E),
XR,0,100,回頭補正,
IS,交点内直進,
CC,RED,(0 1 E),
IS,交点内直進,
CC,YELLOW,(0 2 E),
IS,交点内直進,
CC,YELLOW,(0 3 E),
BT,ブロック移動,
IR,(0 3 S),
XR,0,100,回頭補正,
IS,交点内直進,
CC,YELLOW,(1 3 S),
IS,交点内直進,
CC,GREEN,(2 3 S),
IS,交点内直進,
CC,GREEN,(3 3 S),
BR,(3 3 N),
XR,0,100,回頭補正,
IS,交点内直進,
CC,GREEN,(2 3 N),
IS,交点内直進,
CC,YELLOW,(1 3 N),
PR,75,60.0,clockwise,(1 3 E),
"""
        self.assertEqual(actual_commands, expected_commands)

    def test_navigate_right_course(self):
        """Rコースについて、想定しているコマンドファイルを返すかを検証する."""
        is_left_course = False
        # コース情報生成
        length = 4
        block_map = np.zeros((length, length))
        dummy_block_coords = [(3, 3), (0, 3)]
        treasure_block_coord = (0, 0)
        for (y, x) in dummy_block_coords:
            block_map[y][x] = 1
        block_map[treasure_block_coord[0], treasure_block_coord[1]] = 2

        # ブロックエリアのマップ初期化
        block_area_map = BlockAreaMap(is_left_course, block_map)
        # ナビゲーター初期化
        navigator = Navigator(block_area_map)

        # ロボット初期化
        start_robot = Robot(*(2, 3), Direction.N, [])
        end_robot = Robot(*(1, 0), Direction.W)

        # 動作を計画する
        robot = navigator.navigate(start_robot, end_robot)
        # コマンドを出力する
        actual_commands = ""
        for motion in robot.motions:
            actual_commands += f"{motion.make_command()}\n"
        expected_commands = """BR,(2 3 S),
XR,0,100,回頭補正,
IS,交点内直進,
CC,BLUE,(3 3 S),
BT,ブロック移動,
BR,(3 3 N),
XR,0,100,回頭補正,
IS,交点内直進,
CC,BLUE,(2 3 N),
IS,交点内直進,
CC,RED,(1 3 N),
IS,交点内直進,
CC,RED,(0 3 N),
BT,ブロック移動,
IL,(0 3 W),
XR,0,100,回頭補正,
IS,交点内直進,
CC,RED,(0 2 W),
IS,交点内直進,
CC,YELLOW,(0 1 W),
IS,交点内直進,
CC,YELLOW,(0 0 W),
IL,(0 0 S),
XR,0,100,回頭補正,
IS,交点内直進,
CC,YELLOW,(1 0 S),
PR,75,60.0,clockwise,(1 0 W),
"""
        self.assertEqual(actual_commands, expected_commands)
