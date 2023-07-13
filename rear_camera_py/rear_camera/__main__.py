"""mainモジュール.

プロジェクトルートで、以下のコマンドを実行すると最初に呼び出されるファイル
> python -m rear_camera

必要最低限のコードのみを記述するようにする
@author: kawanoichi
"""


import argparse
from camera_interface import CameraInterface


def main():
    """リアカメラシステム."""
    parser = argparse.ArgumentParser(description="リアカメラに関するプログラム")
    parser.add_argument("--camera-num", type=int, default=0,
                        help="カメラID")
    args = parser.parse_args()

    camera = CameraInterface(args.camera_num)
    camera.start_camera()


if __name__ == "__main__":
    main()
