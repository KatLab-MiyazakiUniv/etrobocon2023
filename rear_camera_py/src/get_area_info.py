"""ブロックでとレジャーのエリア情報を取得するモジュール

参考資料:
NOTE:HSV空間についての参考ページ
https://www.peko-step.com/html/hsv.html

NOTE:HSV値調べる
https://yanohirota.com/image-spuit/

実行コマンド:
rear_camera/ 直下で実行することを想定
$ make get_area
"""
import numpy as np
import cv2
import os
from enum import Enum

script_dir = os.path.dirname(os.path.abspath(__file__))  # /src
PROJECT_DIR_PATH = os.path.dirname(script_dir)  # /rear_camera_py


class Color(Enum):
    """色クラス."""
    BLACK = [0, 0, 0]
    RED = [0, 0, 255]
    YELLOW = [0, 255, 255]
    GREEN = [0, 255, 0]
    BLUE = [255, 0, 0]
    WHITE = [255, 255, 255]


class GetAreaInfo:
    RED1 = [(0, 15), (90, 255), (100, 255)]
    YELLOW = [(16, 30), (50, 255), (150, 255)]
    GREEN = [(31, 100), (60, 255), (40, 255)]
    BLUE = [(101, 150), (110, 255), (70, 255)]
    RED2 = [(151, 180), (80, 255), (90, 255)]

    # OpenCVのHSV空間の上限はどれも255
    WHITE_threshold = [100, 50, 145]  # 彩度 HSV (以上,以下,以上)
    BLACK_threshold = [100, 50, 145]  # 彩度 90以下

    # RGB値(黒、赤、黄、緑、青、白)

    def __init__(self, image_name, image_dir_path) -> None:
        """GetAreaInfoのコンストラクタ."""
        self.image_name = image_name
        self.image_dir_path = image_dir_path
        self.color_id_img = []  # カラーIDを格納する配列を宣言

    def start(self) -> None:
        """画像を6色画像に変換する関数.

        Args:
            game_area_img (cv2.Mat): ゲームエリア画像
            save_path (str): 出力画像ファイルの保存パス
        """

        ## 色の変換 ##
        # 画像読み込み
        image_path = os.path.join(self.image_dir_path, self.image_name)
        game_area_img = cv2.imread(image_path)

        y_size = game_area_img.shape[0]  # 入力画像の縦サイズ
        x_size = game_area_img.shape[1]  # 入力画像の横サイズ
        color_size = game_area_img.shape[2]  # RGBの3次元
        
        # 元画像で射影変換の結果を確認するための調整用動作
        #""" 
        #original = np.float32([(432, 676), (1360, 691), (0, 807), (1636, 846)]) # ダブルループ
        original = np.float32([(2, 958), (542, 677), (1230, 986), (1412, 721)]) # 赤の端点サークル
        trans = np.float32([(0, 0), (1636, 0), (0, 1200), (1636, 1200)])

        trans_mat = cv2.getPerspectiveTransform(original, trans)
        trans_img = cv2.warpPerspective(game_area_img, trans_mat, (x_size, y_size))
        save_path = os.path.join(self.image_dir_path, "trans_"+self.image_name)
        cv2.imwrite(save_path, trans_img)
        #"""
        

        # # BGR色空間からHSV色空間への変換
        hsv = cv2.cvtColor(game_area_img, cv2.COLOR_BGR2HSV)

        # 処理結果を保持する配列を宣言(色を白で初期化)
        # changed_color_img = np.full((y_size, x_size, color_size), [255, 255, 255], dtype=np.uint8)
        # 処理結果を保持する配列を宣言(色を黒で初期化)
        changed_color_img = np.zeros((y_size, x_size, color_size), dtype=np.uint8)

        # 赤1,2
        changed_color_img[np.where((self.RED1[0][0] <= hsv[:, :, 0]) & (hsv[:, :, 0] <= self.RED1[0][1]) &
                                   (self.RED1[1][0] <= hsv[:, :, 1]) & (hsv[:, :, 1] <= self.RED1[1][1]) &
                                   (self.RED1[2][0] <= hsv[:, :, 2]) & (hsv[:, :, 2] <= self.RED1[2][1]))
                          ] = Color.WHITE.value

        changed_color_img[np.where((self.RED2[0][0] <= hsv[:, :, 0]) & (hsv[:, :, 0] <= self.RED2[0][1]) &
                                   (self.RED2[1][0] <= hsv[:, :, 1]) & (hsv[:, :, 1] <= self.RED2[1][1]) &
                                   (self.RED2[2][0] <= hsv[:, :, 2]) & (hsv[:, :, 2] <= self.RED2[2][1]))
                          ] = Color.WHITE.value

        # 黄
        changed_color_img[np.where((self.YELLOW[0][0] <= hsv[:, :, 0]) & (hsv[:, :, 0] <= self.YELLOW[0][1]) &
                                   (self.YELLOW[1][0] <= hsv[:, :, 1]) & (hsv[:, :, 1] <= self.YELLOW[1][1]) &
                                   (self.YELLOW[2][0] <= hsv[:, :, 2]) & (hsv[:, :, 2] <= self.YELLOW[2][1]))
                          ] = Color.WHITE.value

        # 緑
        changed_color_img[np.where((self.GREEN[0][0] <= hsv[:, :, 0]) & (hsv[:, :, 0] <= self.GREEN[0][1]) &
                                   (self.GREEN[1][0] <= hsv[:, :, 1]) & (hsv[:, :, 1] <= self.GREEN[1][1]) &
                                   (self.GREEN[2][0] <= hsv[:, :, 2]) & (hsv[:, :, 2] <= self.GREEN[2][1]))
                          ] = Color.WHITE.value

        # 青
        changed_color_img[np.where((self.BLUE[0][0] <= hsv[:, :, 0]) & (hsv[:, :, 0] <= self.BLUE[0][1]) &
                                   (self.BLUE[1][0] <= hsv[:, :, 1]) & (hsv[:, :, 1] <= self.BLUE[1][1]) &
                                   (self.BLUE[2][0] <= hsv[:, :, 2]) & (hsv[:, :, 2] <= self.BLUE[2][1]))
                          ] = Color.WHITE.value

        # 白
        # changed_color_img[np.where(hsv[:,:,0] >= self.WHITE_threshold[0]) and \
        #             np.where(hsv[:,:,1] <= self.WHITE_threshold[1]) and \
        #             np.where(hsv[:,:,2] >= self.WHITE_threshold[2])] = Color.WHITE.value

        # 黒
        # changed_color_img[np.where(hsv[:,:,2] <= self.BLACK_threshold)] = Color.BLACK.value
        # changed_color_img[np.where(hsv[:,:,2] <= self.BLACK_threshold)] = Color.WHITE.value
        # changed_color_img[np.where(hsv[:,:,0] >= self.BLACK_threshold[0]) and \
        #             np.where(hsv[:,:,1] <= self.BLACK_threshold[1]) and \
        #             np.where(hsv[:,:,2] >= self.BLACK_threshold[2])] = Color.WHITE.value

        # 6色画像を保存
        save_path = os.path.join(self.image_dir_path, "changed_color_"+self.image_name)
        cv2.imwrite(save_path, changed_color_img)

        ## 線分検出 ##
        fast_line_detector = cv2.ximgproc.createFastLineDetector(
            length_threshold=200,
            distance_threshold=1.41421356,
            canny_th1=50,
            canny_th2=50,
            canny_aperture_size=3,
            do_merge=False)

        # グレースケール
        gray = cv2.cvtColor(changed_color_img, cv2.COLOR_BGR2GRAY)

        lines = fast_line_detector.detect(gray)
        if lines is None:
            print("Error: lines is None")
            exit()

        # 赤線を引く
        result_line_img = np.full((y_size, x_size, color_size), [255, 255, 255], dtype=np.uint8)
        max_dist = 0
        max_index = 0
        for i, line in enumerate(lines):
            x1, y1, x2, y2 = map(int, line[0])
            dist = np.linalg.norm(np.array((x1, y1))-np.array((x2, y2)))
            if np.linalg.norm(np.array((x1, y1))-np.array((x2, y2))) > max_dist:
                max_dist = dist
                max_index = i
            red_line_img = cv2.line(result_line_img, (x1, y1), (x2, y2), (0, 0, 255), 3)
        save_path = os.path.join(self.image_dir_path, "detected_line_"+self.image_name)
        cv2.imwrite(save_path, red_line_img)

        ## いらない場所の削除 ##
        # 最も長い線だけを描画
        result_long_line_img = np.full((y_size, x_size, color_size), [
                                       255, 255, 255], dtype=np.uint8)
        x1, y1, x2, y2 = map(int, lines[max_index][0])
        red_line_img = cv2.line(result_long_line_img, (x1, y1), (x2, y2), (0, 0, 255), 3)
        save_path = os.path.join(self.image_dir_path, "detected_long_line_"+self.image_name)
        cv2.imwrite(save_path, red_line_img)

        changed_color_img[np.min([y1, y2])-20:, :] = Color.BLACK.value  # 画像下側削除
        changed_color_img[:np.min([y1, y2])-220, :] = Color.BLACK.value  # 画像上側削除
        save_path = os.path.join(self.image_dir_path, "changed_color2_"+self.image_name)
        cv2.imwrite(save_path, changed_color_img)

        #"""
        # 射影変換を行いたい！！
        #original = np.float32([(432, 676), (1360, 691), (0, 807), (1636, 846)]) # ダブルループ
        original = np.float32([(2, 958), (542, 677), (1230, 986), (1412, 721)]) # 赤の端点サークル
        trans = np.float32([(0, 0), (1636, 0), (0, 1200), (1636, 1200)])
        
        row, column, _ = changed_color_img.shape

        trans_mat = cv2.getPerspectiveTransform(original, trans)
        trans_img = cv2.warpPerspective(changed_color_img, trans_mat, (column, row))
        gray_trans_img = cv2.cvtColor(trans_img, cv2.COLOR_BGR2GRAY)
        save_path = os.path.join(self.image_dir_path, "gray_trans_"+self.image_name)
        cv2.imwrite(save_path, gray_trans_img)
        
        # 赤と青の物体検知を行う閾値
        blue_lower = np.array([100, 100, 100])
        blue_upper = np.array([130, 255, 255])
        red_lower = np.array([0, 100, 100])
        red_upper = np.array([180, 255, 255])

        red_mask = cv2.inRange(hsv, red_lower, red_upper)
        blue_mask = cv2.inRange(hsv, blue_lower, blue_upper)

        # 赤、青の物体を各色の矩形で囲む
        blue_contours, _ = cv2.findContours(blue_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        red_contours, _ = cv2.findContours(red_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        blue_blocks = []
        red_blocks = []
        for contour in blue_contours:
            x, y, w, h = cv2.boundingRect(contour)
            # 面積が小さすぎる、もしくは大きすぎる矩形を除外
            if(cv2.contourArea(contour) < 1000) or (cv2.contourArea(contour) > 1000000):
                continue
            
            # アスペクト比が一定以上の場合を除外
            aspect_ratio = float(w) / h if h > 0 else 0
            if aspect_ratio > 2.0:
                continue
            blue_blocks.append((x, y, x + w, y + h))
            cv2.rectangle(game_area_img, (x, y), (x + w, y + h), (255, 0, 0), 2)
        print(f'blue_blocks: {blue_blocks}')
        
        for contour in red_contours:
            x, y, w, h = cv2.boundingRect(contour)
            # 面積が小さすぎる、もしくは大きすぎる矩形を除外
            if(cv2.contourArea(contour) < 1000) or (cv2.contourArea(contour) > 1000000):
                continue
            
            # アスペクト比が一定以上の場合を除外
            aspect_ratio = float(w) / h if h > 0 else 0
            if aspect_ratio > 2.0:
                continue
            
            skip_flag = False
            for i in range(len(blue_blocks)):
                if abs(x - blue_blocks[i][0]) < 10:
                    skip_flag = True
                    break
            if skip_flag == True:
                continue
            red_blocks.append((x, y, x + w, y + h))
            cv2.rectangle(game_area_img, (x, y), (x + w, y + h), (0, 0, 255), 2)
        print(f'red_blocks: {red_blocks}')

        save_path = os.path.join(self.image_dir_path, "circles_"+self.image_name)
        cv2.imwrite(save_path, game_area_img)
        
        #"""
        
        # サークルを見つけていく
        circles = cv2.HoughCircles(gray_trans_img, cv2.HOUGH_GRADIENT, dp=1, minDist=20,
                                   param1=100, param2=60, minRadius=0, maxRadius=0)
        print("circles\n", circles)
        if circles is None:
            print("Error: circles is None")
            exit()

        circles = np.uint16(np.around(circles))

        detect_circle_img = np.full((y_size, x_size, color_size), [255, 255, 255], dtype=np.uint8)

        for circle in circles[0, :]:
            # 円周を描画する
            cv2.circle(detect_circle_img, (circle[0], circle[1]), circle[2], (0, 165, 255), 5)
            # 中心点を描画する
            cv2.circle(detect_circle_img, (circle[0], circle[1]), 2, (0, 0, 255), 3)

        save_path = os.path.join(self.image_dir_path, "changed_color2_"+self.image_name)
        cv2.imwrite(save_path, detect_circle_img)
        # """

if __name__ == "__main__":
    # 処理時間計測用
    import time
    start = time.time()

    # 作業用の読み込みや保存用のディレクトリパス
    work_dir_path = os.path.join(PROJECT_DIR_PATH, "work_image_data")

    # 画像ファイル名
    image_name = "test.png"                # ダブルループ
    # image_name = "2023-10-06_13-26-37.png" # 赤の端点サークル
    # 画像ファイルパス
    image_path = os.path.join(work_dir_path, image_name)

    # 画像があるかのチェック
    if not os.path.exists(image_path):
        print("画像がありません")
        print("image_path:", image_path)
        exit()

    cc = GetAreaInfo(image_name, work_dir_path)
    cc.start()

    # 処理時間計測用
    execute_time = time.time() - start
    print(f"実行時間: {str(execute_time)[:5]}s")

    print("完了！")
