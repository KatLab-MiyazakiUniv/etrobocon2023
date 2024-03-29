"""mainモジュール.

プロジェクトルートで、以下のコマンドを実行すると最初に呼び出されるファイル
> python -m src

必要最低限のコードのみを記述するようにする
@author: aridome222
"""
import argparse

from src.angle_server import AngleServer
from src.calibrator import Calibrator
from src.camera_interface import CameraInterface
from src.line_angle_calculator import LineAngleCalculator


parser = argparse.ArgumentParser(description="回頭補正用角度算出プログラム")
parser.add_argument("--camera-num", type=int, default=0,
                    help="カメラID")
parser.add_argument("--calibrate", action="store_true",
                    help="キャリブレーションを行う場合指定するフラグ")
parser.add_argument("--debug", action="store_true",
                    help="デバッグを行う場合指定するフラグ")
parser.add_argument("--server", action="store_true",
                    help="サーバモードを利用する場合指定するフラグ")
parser.add_argument("--server-ip", type=str, default="127.0.0.1",
                    help="サーバのIPアドレス(同一ホスト内のプロセス間通信の場合、デフォルト値でOK).")
parser.add_argument("--server-port", type=int, default=10338,
                    help="サーバのListenポート")
args = parser.parse_args()

camera = CameraInterface(args.camera_num)
camera.start_camera()

calibrator = Calibrator(camera, debug=args.debug)
if args.calibrate:
    msg = calibrator.calibrate()
    print(msg)
    exit(0)

line_angle_calculator = LineAngleCalculator(camera, debug=args.debug)


def calibrate_func(cmd: str) -> str:
    """キャリブレーションコマンドを実行する際のコールバック関数.

    Args:
        cmd (str): サーバがこの関数を呼び出す際に渡すデータ

    Returns:
        str: キャリブレーション結果に関するメッセージ
    """
    result = calibrator.calibration_handler(cmd)
    line_angle_calculator.load_params()
    return result


if args.server:
    # サーバの設定
    angle_server = AngleServer(
        server_ip=args.server_ip,
        server_port=args.server_port,
        listen_num=127,
        buf_size=1024,
        debug=args.debug
    )

    angle_server.add_command(
        'angle',
        lambda _: str(line_angle_calculator.calc_yaw_angle())
    )

    angle_server.add_command(
        'calibrate',
        calibrate_func
    )
    angle_server.run()
else:
    line_angle_calculator = LineAngleCalculator(camera, debug=args.debug)
    print(line_angle_calculator.calc_yaw_angle())
