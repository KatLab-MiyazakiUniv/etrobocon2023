"""経路探索クラス.

@author: miyashita64 YKhm20020
"""

import numpy as np
from itertools import permutations
from robot import Robot, Direction
from block_area_map import BlockAreaMap
from motion import Motion


class Navigator:
    """経路探索するクラス."""

    def __init__(self, block_area_map):
        """コンストラクタ.

        Args:
            block_area_map (BlockAreaMap): ブロックエリアのマップ
        """
        self.block_area_map = block_area_map

    def simulate_route(self, initial_robot, target_coord) -> Robot:
        """ロボットが目標座標に移動するための経路を算出する.

        Args:
            initial_robot (Robot): ロボットの初期状態
            target_coord (int, int): 目標座標
        Returns:
            (Robot): 探索終了時のロボット
        """
        open_list = [initial_robot]
        close_list = []

        while len(open_list) > 0:
            # 最有力な探索対象を取り出し
            current_robot = open_list.pop(0)
            # 目標座標に行き着いた場合、探索終了
            if current_robot.get_coord() == target_coord:
                return current_robot
            # 遷移可能なロボットの状態を探索対象に追加
            open_list += current_robot.get_transitionable_robots(
                self.block_area_map.circle_color_map
            )
            # 探索済みの状態を記録
            close_list += [current_robot]
            # 重複している or 探索終了済みのロボットの状態を除去
            open_list = list(set(open_list) - set(close_list))
            # 実コストでソートする
            open_list = sorted(open_list)

        # 経路未発見
        print("\nFailed simulate.\n")
        return None

    def navigate(self, start_robot, end_robot) -> Robot:
        """ロボットの動作を計画する.

        Args:
            start_robot (Robot): ロボットの開始状態
            end_robot (Robot): ロボットの終了状態
        Returns:
            best_robot (Robot): 最適な経路を走行したロボットの状態
        """
        robots = []
        # ブロックの運搬順の全ての組み合わせについて繰り返す
        for block_order in permutations(self.block_area_map.block_coords):
            # ロボットの初期化
            robot = start_robot
            # ブロック毎に経路探索
            for block_coord in block_order:
                robot = self.simulate_route(robot, block_coord)
                # 探索に失敗した場合
                if robot is None:
                    # 動作ファイルを書き換えずに処理を終了(デフォルトのファイルが使用されるはず)
                    exit()
            # 終了状態の座標への経路探索
            robot = self.simulate_route(robot, end_robot.get_coord())
            # 終了状態に方角を合わせる
            angle_diff = end_robot.direction - robot.direction
            if angle_diff:
                robot.direction = end_robot.direction
                robot.motions.append(Motion({
                    "command": "PR",
                    "pwm": 75,
                    "angle": abs(angle_diff),
                    "direction": "clockwise" if angle_diff > 0 else "anticlockwise",
                    "comment": f"({robot.y} {robot.x} {robot.direction.name})",
                }))
            robots += [robot]

        # 動作コストが最も小さいロボットを返す
        best_robot = sorted(robots)[0]
        return best_robot
