"""経路探索クラス.

@author: miyashita64 YKhm20020
"""

import numpy as np
### TODO: robot.pyが色々持ちすぎなので追々分解する ###
from robot import Robot, Direction, Color
from block_area_map import BlockAreaMap
from motion import Motion
import itertools as it


class Navigator:
    """経路探索するクラス."""

    def __init__(self, map_data):
        """コンストラクタ.

        Args:
            map_data (np.array): コース情報
        """
        self.map_data = map_data

    def calculate_distance(self, start, target):
        """2点間のユークリッド距離を計算する.

        Args:
            start (int): 初期地点
            target (int): 対象ブロックの座標

        Returns:
            distance (double): 2点間のユークリッド距離
        """

        x1, y1 = start
        x2, y2 = target
        distance = ((x1 - x2) ** 2 + (y1 - y2) ** 2) ** 0.5
        return distance

    def decide_blocks_order(self) -> [(int, int)]:
        """ブロックの運搬順序を決定する.

        Returns:
            block_order([(int, int)]): 運ぶ順にソートしたブロックの座標のリスト
        """

        # ブロックの運搬順を格納する空のリスト
        block_order = []

        for dummy_block in dummy_block_coords:
            block_order.append(dummy_block)

        block_order.append(treasure_block_coord)

        # 赤いブロックが goal_corrdinate にない場合、移動順に追加する.
        if not end_coordinate == treasure_block_coord:
            block_order.append(end_coordinate)

        # 距離が近い順にブロックに向かう（色の区別なし）
        block_order.sort(key=lambda pos: self.calculate_distance(start_coordinate, pos))

        return block_order

    def navigate(self, initial_robot, target_coord) -> Robot:
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
            open_list += current_robot.get_transitionable_robots(self.map_data.shape)
            # 探索済みの状態を記録
            close_list += [current_robot]
            # 重複している or 探索終了済みのロボットの状態を除去
            open_list = list(set(open_list) - set(close_list))
            # 予測コスト + 実コストでソートする
            ### 現状実コストしか見ていない ###
            open_list = sorted(open_list)

        # 経路発見不可
        print("\nFailed navigate.\n")
        return None


if __name__ == "__main__":
    ### TODO: CC(交点→交点)は色指定ライントレースであるためマップ上のサークルの色情報も付加する必要がある ###
    
    is_left_course = False
        
    # ナビゲーター初期化
    block_area_map = BlockAreaMap(is_left_course)
    
    map_data = block_area_map.map_data
    navigator = Navigator(map_data)
    
    # TODO start_coordinate にブロックがある場合のコマンドと処理を連携する
    start_coordinate = block_area_map.start_coord  # 初期地点. 変更する可能性あり.
    end_coordinate = block_area_map.end_coord  # 運搬終了地点. 変更する可能性あり.
    circle_color_mapping = block_area_map.circle_color_mapping

    # TODO map.py から dummy_block_coords と treasure_block_coord を呼び出したい
    dummy_block_coords = block_area_map.dummy_block_coords
    treasure_block_coord = block_area_map.treasure_block_coord
    print(f'dummy_blocks are {dummy_block_coords}')
    print(f'treasure_block is {treasure_block_coord}')

    # ロボット初期化
    robot = Robot(*start_coordinate, Direction.N, [Motion({"comment": f"start = ({start_coordinate[0]} {start_coordinate[1]} {Direction.N.name})"})])
    robot2 = Robot(*start_coordinate, Direction.S, [Motion({"comment": f"start = ({start_coordinate[0]} {start_coordinate[1]} {Direction.S.name})"})])

    # ブロックを探索する順番を決定
    block_coords = navigator.decide_blocks_order()
    print(f'block_coords: {block_coords}')

    # ブロック毎の組み合わせの実験
    robots = []
    # 初期状態が北を向いている場合
    for _block_coords in [list(coords) for coords in it.permutations(block_coords)]:
        # ブロック毎に経路探索
        current_robot = robot
        _block_coords += [end_coordinate]
        for block_coord in _block_coords:
            current_robot = navigator.navigate(current_robot, block_coord)
            if current_robot is None:
                exit()
        robots += [current_robot]
    # 初期状態が南を向いている場合
    for _block_coords in [list(coords) for coords in it.permutations(block_coords)]:
        # ブロック毎に経路探索
        current_robot = robot2
        _block_coords += [end_coordinate]
        for block_coord in _block_coords:
            current_robot = navigator.navigate(current_robot, block_coord)
            if current_robot is None:
                exit()
        robots += [current_robot]
    for motion in sorted(robots)[0].motions:
        print(motion.make_command())
