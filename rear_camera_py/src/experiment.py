"""試走会用、リアカメラ

@author kawanoichi
"""
import argparse
import os
from camera_interface import CameraInterface
from datetime import datetime


def chack_folder(save_path):
    """データを保存するフォルダの作成."""
    if not os.path.exists(save_path):
        os.mkdir(save_path)

def capture_img(camera, data_name):
    """画像取得を行う関数."""
    print("画像を取得")
    chack_folder('./img_data')
    img = camera.capture_image()
    img.save(data_name)

def capture_movie(camera, data_name):
    """動画取得を行う関数."""
    print("動画を取得")
    chack_folder('./movie_data')
    movie = camera.capture_movie()
    
    # あってる？
    movie.save(data_name)

if __name__ == "__main__":
    print(' --- start experiment.py ---')
    parser = argparse.ArgumentParser(description="使用例\n"
                                                 " 画像を取得する\n"
                                                 " $ python experiment.py i\n"
                                                 " 動画を取得する\n"
                                                 " $ python experiment.py m",
                                     formatter_class=argparse.RawTextHelpFormatter)

    parser.add_argument('arg1', nargs='?', help='i:imageを取得, m:movieを取得', default='i')
    args = parser.parse_args()

    now = datetime.now()
    data_name = now.strftime("%Y-%m-%d_%H:%M:%S")
    # """
    camera = CameraInterface()
    camera.start_camera()

    if args.arg1 == "i":
        capture_img(camera, data_name)

    elif args.arg1 == "m":
        capture_movie(camera, data_name)
    # """



