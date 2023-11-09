"""ブロックdeトレジャーハンター攻略クラス.

@author: miyashita64 YKhm20020 kawanoichi
"""

import sys
import numpy as np
import os

from block_area_map import BlockAreaMap
from navigator import Navigator
from robot import Robot, Direction
from motion import Motion
from get_area_info import GetAreaInfo

script_dir = os.path.dirname(os.path.abspath(__file__))  # /src
PROJECT_DIR_PATH = os.path.dirname(script_dir)  # /rear_camera_py


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
        dummy_block_coords = [(3, 3), (0, 3)]
        treasure_block_coord = (0, 0)
        for (y, x) in dummy_block_coords:
            block_map[y][x] = 1
        block_map[treasure_block_coord[0], treasure_block_coord[1]] = 2
        # TODO: ここまで、マージする際に差し替える

        # ブロックエリア情報の取得
        image_name = "BlockDeTreasure.png"
        image_dir_path = os.path.join(PROJECT_DIR_PATH, "image_data")
        area_info = GetAreaInfo(image_name=image_name, image_dir_path=image_dir_path)
        block_map = area_info.get_area_info(isL=is_left_course)
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
        comment = f"start = ({start_coord[0]} {start_coord[1]} {start_direction.name})"
        start_robot = Robot(*start_coord, start_direction,
                            [Motion({"comment": comment})])
        end_robot = Robot(*end_coord, end_direction)

        # 動作を計画する
        robot = navigator.navigate(start_robot, end_robot)
        # コマンドを出力する
        commands = ""
        for motion in robot.motions:
            commands += f"{motion.make_command()}\n"
        # TODO: ブロックdeトレジャーハンター攻略の詳細が決まり次第パスを設定しなおす
        # 既存のコマンドファイルを書き換えたくないので、Left/Rightはあえて入れていません。
        with open("../../datafiles/BlockDeTreasure.csv", "w") as f:
            f.write(commands)


if __name__ == "__main__":
    args = sys.argv
    if 2 <= len(args):
        if args[1] == "left" or args[1] == "right":
            is_left_course = args[1] == "left"
            hunter = BlockDeTreasureHunter()
            hunter.hunt(is_left_course)
        else:
            print("An invalid argument was given.")
    else:
        print("The argument is_left_course was not given.")
