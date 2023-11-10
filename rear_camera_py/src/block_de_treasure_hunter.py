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
        # ロボット侵入動作
        first_rotate = "clockwise,右回頭" if is_left_course else "anticlockwise,左回頭"
        second_rotate = "anticlockwise,左回頭" if is_left_course else "clockwise,右回頭"
        enter_edge = "right" if is_left_course else "left"
        enter_motions = f"PR,60,55,{first_rotate}\n" \
            + "DS,80,250,直進\n" \
            + "CS,BLACK,150,黒まで直進、ブロックエリア侵入\n" \
            + "DS,50,150,黒線から外れるよう直進\n" \
            + f"EC,{enter_edge},エッジ切替\n" \
            + f"PR,60,60,{second_rotate}\n" \
            + f"CL,BLUE,200,-10,0.4,0.22,0.1,青サークルまで移動({start_coord[0]} {start_coord[1]} {start_direction.name})"
        # ロボット初期化
        start_robot = Robot(*start_coord, start_direction,
                            [Motion({"enter_motions": enter_motions})])
        end_robot = Robot(*end_coord, end_direction)

        # 動作を計画する
        robot = navigator.navigate(start_robot, end_robot)
        # コマンドを出力する
        commands = ""
        for motion in robot.motions:
            commands += f"{motion.make_command()}\n"
        cource_string = "Left" if is_left_course else "Right"
        with open(f"../../datafiles/BlockDeTreasure{cource_string}.csv", "w") as f:
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
