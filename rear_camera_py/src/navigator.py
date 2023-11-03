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
        simulatable_robots = [initial_robot] # 探索候補のロボットの状態
        simulated_robots = []                # 探索済みのロボット状態

        while len(simulatable_robots) > 0:
            # コストが最小な探索対象
            robot = min(simulatable_robots)
            # 目標座標に行き着いた場合、探索終了
            if robot.get_coord() == target_coord:
                return robot
            # 遷移可能なロボットの状態を探索対象に追加
            simulatable_robots += robot.get_transitionable_robots(
                self.block_area_map.circle_color_map
            )
            # 探索済みの状態を記録
            simulated_robots += [robot]
            # 集合として、重複している or 探索済みのロボットの状態を除去
            simulatable_robots = list(set(simulatable_robots) - set(simulated_robots))

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
                # 回頭動作を追加
                robot.motions.append(Motion({
                    "command": "PR",
                    "pwm": 75,
                    "angle": Motion.calc_rotate_angle(abs(angle_diff)),
                    "direction": "clockwise" if angle_diff > 0 else "anticlockwise",
                    "comment": f"({robot.y} {robot.x} {robot.direction.name})",
                }))
            robots += [robot]

        # 動作コストが最も小さいロボットを返す
        best_robot = min(robots)
        return best_robot
