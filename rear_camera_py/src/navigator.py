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

        ### TODO: A*か何かでMotionのリストを生成する ###

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
    robot = Robot(2,0, Direction.N)
    # ナビゲーター初期化
    navigator = Navigator(map, robot)

    ### TODO: 最適と思われるブロックの移動順を決定できるように実装する ###
    # ブロックを探索する順番を決定(とりあえずトレジャーブロックが最後)
    block_coords = dummy_block_coords + [treasure_block_coord]
    # block_coords = navigator.decide_blocks_order()

    # ブロック毎に経路探索
    for block_coord in block_coords:
        motions = navigator.navigate(block_coord)
        for motion in motions:
            print(motion.make_command())
