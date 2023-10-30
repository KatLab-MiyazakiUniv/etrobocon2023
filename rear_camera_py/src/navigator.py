"""経路探索クラス.

@author: miyashita64 YKhm20020
"""

import numpy as np
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

    def calculate_distance(self, start_coord, target_coord):
        """2点間のユークリッド距離を計算する.

        Args:
            start_coord ((int, int)): 初期地点
            target_coord ((int, int)): 対象ブロックの座標

        Returns:
            distance (double): 2点間のユークリッド距離
        """
        x1, y1 = start_coord
        x2, y2 = target_coord
        distance = ((x1 - x2) ** 2 + (y1 - y2) ** 2) ** 0.5
        return distance

    def decide_blocks_order(self, start_coord) -> [(int, int)]:
        """ブロックの運搬順序を決定する.

        Args:
            start_coord ((int,int)): スタート座標

        Returns:
            block_order([(int, int)]): 運ぶ順にソートしたブロックの座標のリスト
        """
        # ブロックの座標のリスト
        block_coords = np.transpose(np.nonzero(self.block_area_map.block_map))
        # 型をタプルに統一する
        block_coords = [tuple(coord) for coord in block_coords]

        # 距離が近い順にブロックに向かう（色の区別なし）
        block_order = sorted(
            block_coords, key=lambda coord: self.calculate_distance(start_coord, coord))

        return block_order

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

    def navigate(self, start_robot, end_robot) -> None:
        """ロボットの動作を計画する.

        Args:
            start_robot (Robot): ロボットの開始状態
            end_robot (Robot): ロボットの終了状態
        """
        # ブロックの運搬順を決定
        block_order = self.decide_blocks_order(start_robot.get_coord())

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

        # コマンドを出力する
        for motion in robot.motions:
            print(motion.make_command())
