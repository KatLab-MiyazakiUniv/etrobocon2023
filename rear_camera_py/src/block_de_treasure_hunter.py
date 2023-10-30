"""ブロックdeトレジャーハンター攻略クラス.

@author: miyashita64 YKhm20020
"""

import numpy as np
from block_area_map import BlockAreaMap
from navigator import Navigator
from robot import Robot, Direction
from motion import Motion


class BlockDeTreasureHunter:
    """ブロックdeトレジャーを攻略するクラス."""

    def hunt(self, is_left_course):
        """攻略を実行する.

        Args:
            is_left_course (bool): Lコースであるかどうか
        """
        # コース情報取得
        # TODO: ここから、マージする際に差し替える
        length = 4
        block_map = np.zeros((length, length))
        dummy_block_coords = [(0, 3), (3, 3)]
        treasure_block_coord = (3, 0)
        for (y, x) in dummy_block_coords:
            block_map[y][x] = 1
        block_map[treasure_block_coord[0], treasure_block_coord[1]] = 2
        # TODO: ここまで、マージする際に差し替える
        print(block_map)

        # ブロックエリアのマップ初期化
        block_area_map = BlockAreaMap(is_left_course, block_map)
        # ナビゲーター初期化
        navigator = Navigator(block_area_map)

        # L/Rに合わせて開始・終了状態を設定(Lコースを基準)
        start_direction = Direction.N
        end_direction = Direction.E
        start_coord = (2, 0)
        end_coord = (1, 3)
        if not is_left_course:
            # Rコースの場合
            end_direction = Direction.W
            start_coord = (2, 3)
            end_coord = (1, 0)
        # ロボット初期化
        start_robot = Robot(*start_coord, start_direction,
                            [Motion(
                                {"comment": f"start = ({start_coord[0]} {start_coord[1]} \
                                     {start_direction.name})"})])
        end_robot = Robot(*end_coord, end_direction)

        # 動作を計画する
        navigator.navigate(start_robot, end_robot)


if __name__ == "__main__":
    is_left_course = True
    hunter = BlockDeTreasureHunter()
    hunter.hunt(is_left_course)
