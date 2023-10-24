"""経路探索クラス.

@author: miyashita64 YKhm20020
"""

import numpy as np
### TODO: robot.pyが色々持ちすぎなので追々分解する ###
from robot import Robot, Direction
from robot import Color, Motion, Straight, Curve

class Navigator:
    """経路探索するクラス."""
    def __init__(self, map, robot):
        """コンストラクタ.

        Args:
            map (np.array): コース情報
            robot (Robot):  ロボット
        """
        self.map = map
        self.robot = robot

    def decide_block_order(self) -> [(int, int)]:
        """ブロックの運搬順序を決定する.

        Returns:
            block_order([(int, int)]): 運ぶ順にソートしたブロックの座標のリスト
        """

        ### TODO: 最適と思われるブロックの移動順を決定できるように実装する ###

        return [(0,0),(1,1),(1,3)]

    def navigate(self, target_coord) -> [Motion]:
        """ロボットが目標座標に移動するための経路を算出する.

        Args:
            target_coord (int, int): 目標座標
        Returns:
            motions ([Motion]): 目標座標に移動するための動作のリスト
        """
        open_list = [self.robot]
        close_list = []
        motions = []

        while len(open_list) > 0:
            # 最有力な探索対象を取り出し
            current_robot = open_list.pop(0)
            # 目標座標に行き着いた場合、探索終了
            if current_robot.get_coord() == target_coord:
                self.robot = current_robot
                return current_robot.motions
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
        return motions


if __name__ == "__main__":
    # マップ初期化
    length = 4
    map = np.zeros(length**2)
    map.shape = (length, length)
    # ブロック設置
    dummy_block_coords = [(0,0), (1,1)]
    treasure_block_coord = (1,3)
    for (y, x) in dummy_block_coords:
        map[y][x] = 1
    map[treasure_block_coord[0], treasure_block_coord[1]] = 2
    # マップ表示
    print(map)

    ### この時点でのmapがコース情報取得から得られるはず ###

    ### TODO: CC(交点→交点)は色指定ライントレースであるためマップ上のサークルの色情報も付加する必要がある ###
    ### TODO: サークルの色や開始位置・終了位置が変わるためLRどちらのコースなのかを認識する必要がある ###

    # ロボット初期化
    robot = Robot(2, 0, Direction.N)
    # ナビゲーター初期化
    navigator = Navigator(map, robot)

    ### TODO: 最適と思われるブロックの移動順を決定できるように実装する ###
    # ブロックを探索する順番を決定(とりあえずトレジャーブロックが最後)
    block_coords = dummy_block_coords + [treasure_block_coord]
    # block_coords = navigator.decide_blocks_order()

    # ブロック毎に経路探索
    motions = []
    for block_coord in block_coords:
        motions = navigator.navigate(block_coord)
    for motion in motions:
        print(motion.make_command())
