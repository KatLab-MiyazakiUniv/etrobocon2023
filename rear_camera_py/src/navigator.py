"""経路探索クラス.

@author: miyashita64 YKhm20020
"""

import numpy as np
### TODO: robot.pyが色々持ちすぎなので追々分解する ###
from robot import Robot, Direction
from robot import Color, Motion, Straight, Curve


class Navigator:
    """経路探索するクラス."""

    def __init__(self, map):
        """コンストラクタ.

        Args:
            map (np.array): コース情報
        """
        self.map = map

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

        # 青 -> 赤の順で処理.
        dummy_block_coords.sort(key=lambda pos: self.calculate_distance(start_coordinate, pos))

        for dummy_block in dummy_block_coords:
            block_order.append(dummy_block)

        block_order.append(treasure_block_coord)

        # 赤いブロックが goal_corrdinate にない場合、移動順に追加する.
        if not goal_coordinate == treasure_block_coord:
            block_order.append(goal_coordinate)

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
            open_list += current_robot.get_transitionable_robots(self.map.shape)
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
    # マップ初期化
    length = 4
    map = np.zeros(length**2)
    map.shape = (length, length)
    # ブロック設置
    # dummy_block_coords = [(0, 0), (1, 1)]
    # treasure_block_coord = (1, 3)
    dummy_block_coords = [(0, 3), (3, 3)]
    treasure_block_coord = (3, 0)

    for (y, x) in dummy_block_coords:
        map[y][x] = 1
    map[treasure_block_coord[0], treasure_block_coord[1]] = 2
    # マップ表示
    print(map)

    ### !!! この時点でのmapがコース情報取得から得られるはず                  !!! ###
    ### !!!  => これより上に値について、map以外は使用できないことを前提とする !!! ###

    # TODO start_coordinate にブロックがある場合のコマンドと処理を連携する
    start_coordinate = (2, 0)  # 初期地点. 変更する可能性あり.
    goal_coordinate = (1, 3)  # 運搬終了地点. 変更する可能性あり.

    ### TODO: CC(交点→交点)は色指定ライントレースであるためマップ上のサークルの色情報も付加する必要がある ###
    ### TODO: サークルの色や開始位置・終了位置が変わるためLRどちらのコースなのかを認識する必要がある ###

    # ロボット初期化
    robot = Robot(*start_coordinate, Direction.N)
    print(f"start = ({robot.y} {robot.x} {robot.direction.name})")
    # ナビゲーター初期化
    navigator = Navigator(map)

    # ブロックを探索する順番を決定
    block_coords = navigator.decide_blocks_order()
    print(f'block_coords: {block_coords}')

    # ブロック毎に経路探索
    current_robot = robot
    for block_coord in block_coords:
        current_robot = navigator.navigate(current_robot, block_coord)
        if current_robot is None:
            exit()
    for motion in current_robot.motions:
        print(motion.make_command())
