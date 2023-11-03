"""ブロックエリアのマップを定義するクラス(経路探索用).

@author: miyashita64 YKhm20020
"""

import numpy as np


class BlockAreaMap:
    """ブロックエリアのマップを定義するクラス."""

    L_COURSE_CIRCLE_COLOR_MAP = np.array([
        ["RED", "RED", "YELLOW", "YELLOW"],
        ["RED", "RED", "YELLOW", "YELLOW"],
        ["BLUE", "BLUE", "GREEN", "GREEN"],
        ["BLUE", "BLUE", "GREEN", "GREEN"]
    ])

    def __init__(self, is_left_course, block_map):
        """コンストラクタ.

        Args:
            is_left_course (bool): Lコースであるかどうか
            block_map (np.array): コース情報
        """
        self.block_map = block_map

        # ブロックの座標のリスト
        block_coords = np.transpose(np.nonzero(self.block_map))
        # 型をタプルに統一する
        self.block_coords = [tuple(coord) for coord in block_coords]

        # L/Rに合わせてサークルの色を設定
        if is_left_course:
            self.circle_color_map = BlockAreaMap.L_COURSE_CIRCLE_COLOR_MAP
        else:
            # x軸(第2のインデックス)を反転
            self.circle_color_map = BlockAreaMap.L_COURSE_CIRCLE_COLOR_MAP[:, ::-1]
