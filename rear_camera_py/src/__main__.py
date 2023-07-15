"""mainモジュール.

プロジェクトルートで、以下のコマンドを実行すると最初に呼び出されるファイル
> python -m rear_camera

必要最低限のコードのみを記述するようにする
@author: kawanoichi
"""


import argparse
from camera_interface import CameraInterface
from datetime import datetime
from PIL import Image

def main():
    """リアカメラシステム."""
    now = datetime.now()
    now_time = now.strftime("%Y-%m-%d %H:%M:%S")
    parser = argparse.ArgumentParser(description="リアカメラに関するプログラム")
    parser.add_argument("--camera-num", type=int, default=0,
                        help="カメラID")
    args = parser.parse_args()

    camera = CameraInterface(args.camera_num)
    camera.start_camera()
    # 画像を取得
    img = camera.capture_image()

    # カラーチャンネルの順序を変換（BGRからRGB）
    image_data_rgb = img[:, :, ::-1]

    pil_img = Image.fromarray(image_data_rgb)
    pil_img.save(now_time+".png")


if __name__ == "__main__":
    print("Rear Camera Start!!")
    main()
