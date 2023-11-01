#!/usr/bin/env python
"""ブロックでとレジャーのエリア情報を取得するモジュール.

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
    """ブロックdeトレジャーのエリア情報を取得するクラス."""

    # OpenCVのHSV空間の上限はどれも255
    RED1 = [(0, 15), (90, 255), (100, 255)]
    YELLOW = [(16, 37), (50, 255), (120, 255)]
    GREEN = [(38, 95), (40, 255), (40, 255)]
    BLUE = [(96, 150), (95, 255), (80, 255)]
    RED2 = [(171, 190), (80, 255), (90, 255)]

    BLACK = [(0, 255), (0, 50), (0, 140)]

    def __init__(self,
                 image_name,
                 image_dir_path,
                 save_dir_path=None,
                 develop=None) -> None:
        """GetAreaInfoのコンストラクタ."""
        # 画像パス・保存関連
        self.image_name = image_name
        self.image_dir_path = image_dir_path
        self.save_dir_path = image_dir_path if save_dir_path is None else save_dir_path
        self.develop = develop
        self.img_shape = None

        # 領域閾値
        self.block_area_height = 240

        # ベクトル関連
        self.basis_vector = None  # 緑の線の傾き
        # self.basis_vector_b = None
        self.green_area_b = None
        self.angle_threshold = 4  # 同じ角度の線だと判断する閾値

        # ブロックの数
        self.red_block_num = 1
        self.blue_block_num = 2
        self.block_count_red = 0
        self.block_count_blue = 0

        # ブロックの座標 [x_min, y_max, y_min,y_max]
        self.block_coordi_red = np.zeros(4)
        self.block_coordi_blue1 = np.zeros(4)
        self.block_coordi_blue2 = np.zeros(4)

        # 手前のサークル
        self.course_info_block = np.zeros(16).reshape(4, 4)
        # self.course_info_coordi = np.zeros(4*4*2).reshape(4, 4, 2)
        # self.first_column_coordinate = np.array([None, None, None, None])
        # 左上のxyを格納
        self.course_info_coordinate = np.zeros((4, 4, 4))

    def change_color(self, hsv_img, write_img, search_color, color_value):
        """一定の範囲のHSV値を指定したHSV値に統一する関数.

        Args:
            hsv_img : HSV値に変換した画像
            write_img: 統一した値を書き込む画像
            search_color: 一定の範囲を示すHSV値 (self.RED1などを指定)
            color_value: 統一するHSV値 (Color.RED.valueなど)
        """
        write_img[np.where((search_color[0][0] <= hsv_img[:, :, 0]) &
                           (hsv_img[:, :, 0] <= search_color[0][1]) &
                           (search_color[1][0] <= hsv_img[:, :, 1]) &
                           (hsv_img[:, :, 1] <= search_color[1][1]) &
                           (search_color[2][0] <= hsv_img[:, :, 2]) &
                           (hsv_img[:, :, 2] <= search_color[2][1]))
                  ] = color_value
        return write_img

    def detect_line(self, img, save_img_name=None, length_threshold=200):
        """線分検出を行う関数.

        Args:
            img: 線分検出を行う画像
            save_img_name: 線分検出を行った保存画像の名前

        Return:
            lines: 検出したライン
            max_index: linesの中の最長ラインのインデックス
        """
        # グレースケール化
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # 線分検出
        fast_line_detector = cv2.ximgproc.createFastLineDetector(
            length_threshold=length_threshold,
            distance_threshold=1.41421356,
            canny_th1=50,
            canny_th2=50,
            canny_aperture_size=3,
            do_merge=False)
        lines = fast_line_detector.detect(gray)
        if lines is None:
            return None, None

        # 検出した線を描画する配列を作成(色を白で初期化)
        result_line_img = np.full_like(img, Color.WHITE.value, dtype=np.uint8)

        # 最も長い線分を見つける
        max_length = 0
        max_length_index = -1
        for i, line in enumerate(lines):
            x1, y1, x2, y2 = map(int, line[0])
            length = np.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2)
            if length > max_length:
                max_length = length
                max_length_index = i

        # 検出した線の描画
        if self.develop and save_img_name is not None:
            for i, line in enumerate(lines):
                x1, y1, x2, y2 = map(int, line[0])
                if i == max_length_index:
                    # 最も長い線は赤で表示
                    cv2.line(result_line_img, (x1, y1), (x2, y2), (0, 0, 255), 3)
                else:
                    # 他の線を青で表示
                    cv2.line(result_line_img, (x1, y1), (x2, y2), (255, 0, 0), 3)
            save_path = os.path.join(self.save_dir_path, save_img_name)
            # cv2.imwrite(save_path, result_line_img)

        return lines[:, 0], max_length_index

    def pre_process_img(self, course_img) -> None:
        """画像のいらない部分を削除する関数.

        画像上の以下を削除(白色で上書き)
        - コース下の緑(以下)の部分削除
        - コース上側を削除
        - IoT列車のコースを含めた上側削除

        Args:
            img (np.ndarray): コース画像(BGR)
        """
        # BGR色空間からHSV色空間への変換
        game_area_img = cv2.cvtColor(course_img, cv2.COLOR_BGR2HSV)

        # 処理結果を保持する配列を宣言(色を白で初期化)
        green_area_img = np.full_like(game_area_img, Color.WHITE.value, dtype=np.uint8)

        # コース下の緑(以下)の部分削除 & コース上側を削除
        # 画像の緑の部分だけを抽出
        self.change_color(game_area_img, green_area_img, self.GREEN, Color.GREEN.value)
        if self.develop:
            save_path = os.path.join(self.save_dir_path, "green_area_img.png")
            cv2.imwrite(save_path, green_area_img)

        # 線分を検出
        lines, max_index = self.detect_line(green_area_img,
                                            "line_green_area_img.png",
                                            length_threshold=200)
        if lines is not None:
            # 最も長い線を使用
            x1, y1, x2, y2 = map(int, lines[max_index])
            # 角度を算出
            self.basis_vector = np.array([x2 - x1, y2 - y1])

            # 直線の方程式 y = ax + b を解く
            if self.basis_vector[0] == 0:
                a = 0
            else:
                a = self.basis_vector[1] / self.basis_vector[0]
            self.green_area_b = y1 - a * x1 - 5

            for x in range(course_img.shape[1]):
                # 直線より下の部分を削除
                course_img[int(a * x + self.green_area_b):, x] = Color.WHITE.value
                # コースの上側削除
                course_img[:int(a * x + self.green_area_b - self.block_area_height), x] \
                    = Color.WHITE.value
                # TODO: 上側削除、パラメータが0.52でいいか検証すべき
                course_img[:int(course_img.shape[0]*0.52), x] = Color.WHITE.value
            # トリミング
            upper_delete_line = \
                int(min(self.green_area_b - self.block_area_height,
                        a * course_img.shape[1] + self.green_area_b - self.block_area_height))
            lower_delete_line = int(
                max(self.green_area_b, a * course_img.shape[1] + self.green_area_b))
            course_img = course_img[upper_delete_line:lower_delete_line, :, :]

        # IoT列車のコースを含めた上側削除
        # BGR色空間からHSV色空間への変換(下側削除Ver)
        game_area_img = cv2.cvtColor(course_img, cv2.COLOR_BGR2HSV)

        # 処理結果を保持する配列を宣言(色を白で初期化)
        blue_area_img = np.full_like(game_area_img, Color.WHITE.value, dtype=np.uint8)

        # 画像の青い部分だけを抽出
        self.change_color(game_area_img, blue_area_img, self.BLUE, Color.BLUE.value)

        if self.develop:
            save_path = os.path.join(self.save_dir_path, "blue_area_img.png")
            cv2.imwrite(save_path, blue_area_img)

        # 線分を検出
        lines, max_index = self.detect_line(blue_area_img,
                                            "line_blue_area_img.png",
                                            length_threshold=150)

        if lines is not None:
            # 最も長い線を使用
            x1, y1, x2, y2 = map(int, lines[max_index])
            vector = np.array([x2 - x1, y2 - y1])

            if vector[0] == 0:
                a = 0
            else:
                a = vector[1] / vector[0]
            b = y1 - a * x1 + 10

            game_area_img2 = game_area_img.copy()
            for x in range(course_img.shape[1]):
                if int(a * x + b) < 0:
                    game_area_img2[:, x:] = Color.WHITE.value
                    break
                if int(a * x + b) < course_img.shape[0]:
                    game_area_img2[int(a * x + b):, x] = Color.WHITE.value
            self.change_color(game_area_img2, course_img, self.RED1, Color.WHITE.value)
            # self.change_color(game_area_img2, course_img, self.RED2, Color.WHITE.value)
            self.change_color(game_area_img2, course_img, self.YELLOW, Color.WHITE.value)
            self.change_color(game_area_img2, course_img, self.GREEN, Color.WHITE.value)
            self.change_color(game_area_img2, course_img, self.BLUE, Color.WHITE.value)
            self.change_color(game_area_img2, course_img, self.BLACK, Color.WHITE.value)

        return course_img

    def check_coordi_about(self,
                           color_2_img,
                           search_color=Color.RED.value,
                           split_num_height=4,
                           split_num_width=30,
                           thre=10,
                           develop_img=None,
                           draw_color=Color.BLACK.value
                           ):
        """ブロックのおおまかな場所を特定する関数.

        TODO:ブロックが見つからなかった場合の処理を書く
        """
        if develop_img is None:
            develop_img = color_2_img.copy()

        mask_height = int(color_2_img.shape[0] / split_num_height)
        mask_width = int(color_2_img.shape[1] / split_num_width)

        # (ピクセル数, x, y)
        mask_coordi = np.array([0, 0])
        max_pixcel = 0

        flag = 0
        # おおまかなブロックの場所を特定
        for x in range(0, int(color_2_img.shape[1]), mask_width):
            masks = []
            for y in range(split_num_height):
                masks.append(color_2_img[mask_height *
                             y:mask_height*(y+1), x:x+mask_width])

            # 画素数のカウント
            for i, mask in enumerate(masks):
                count_pixcel = np.count_nonzero(np.all(mask == search_color, axis=-1))
                if count_pixcel > max_pixcel and count_pixcel > thre:
                    mask_coordi = np.array([x, i*mask_height])
                    max_pixcel = count_pixcel

        if np.all(mask_coordi == 0):
            return None, None

        if self.develop and draw_color is not None:
            # マスクの大きさを表す線
            develop_img[mask_height, :] = Color.BLACK.value
            develop_img[:, mask_width] = Color.BLACK.value

            develop_img[mask_coordi[1]:mask_coordi[1]+3,
                        mask_coordi[0]:mask_coordi[0]+mask_width] = draw_color  # 上
            develop_img[mask_coordi[1]:mask_coordi[1]+mask_height,
                        mask_coordi[0]+mask_width:mask_coordi[0]+mask_width+3] = draw_color  # 右
            develop_img[mask_coordi[1]+mask_height:mask_coordi[1]+mask_height+3,
                        mask_coordi[0]:mask_coordi[0]+mask_width] = draw_color  # 下
            develop_img[mask_coordi[1]:mask_coordi[1]+mask_height,
                        mask_coordi[0]:mask_coordi[0]+3] = draw_color  # 左
        return mask_coordi, develop_img

    def check_coordi_detail(self,
                            color_img,
                            coordinate,
                            block_color=Color.RED.value,
                            split_num_height=4,
                            split_num_width=30,
                            thre=5,
                            develop_flag=0,
                            develop_img=None
                            ):
        """ブロックの正確な位置を特定.

        Args:
            color_img: 二色画像(赤、青)
            coordinate  : ブロックの大まかな座標
            develop_img: 実行結果を書き込む画像
            block_color: 対象とするブロックの色
            split_num_height: 縦の分割数
            split_num_width: 横の分割数
            thre: 閾値

        Return:
            detail_coordi: ブロックの正確な座標([x_max, x_min, y_max, y_min])
            develop_img: 開発結果を書き込んだ画像
        """
        mask_height = int(color_img.shape[0] / split_num_height)
        mask_width = int(color_img.shape[1] / split_num_width)

        detect_flag = 0
        x_max, x_min, y_max, y_min = 0, 0, 0, 0
        if develop_img is None:
            develop_img = color_img.copy()

        if develop_flag == 1 and self.develop:
            develop_img[:, coordinate[0]:coordinate[0]+3] = Color.RED.value
            develop_img[coordinate[1]:coordinate[1]+3, :] = Color.RED.value
            save_path = os.path.join(self.save_dir_path, "detail_coordi_circle.png")
            cv2.imwrite(save_path, develop_img)
        # 下
        end = int(coordinate[1]+mask_height*1.5)
        for y in range(coordinate[1], end):  # 上→下
            # 画像からはみ出したら終了
            if y == color_img.shape[0]-1 or y == end-1:
                if detect_flag == 0:
                    break
                elif y == color_img.shape[0]-1:
                    y_min = color_img.shape[0]-1
                    break
                elif y == end-1:
                    y_min = end-1
                    break
                else:
                    develop_img[coordinate[1]:coordinate[1]+2, :] = Color.BLUE.value
                    develop_img[:, coordinate[0]:coordinate[0]+2] = Color.BLUE.value
                    save_path = os.path.join(self.save_dir_path, "Error1.png")
                    cv2.imwrite(save_path, develop_img)
                    print(f"block_color: {block_color}")
                    print(f"coordinate : {coordinate}")
                    print(f"thre       : {thre}")
                    raise ValueError("Error1:色が見つかりません")

            # ピクセル数検出
            mask = color_img[y, coordinate[0]:coordinate[0]+mask_width]
            count_pixcel = np.count_nonzero(np.all(mask == block_color, axis=-1))

            # 探している物体(ブロックなど)の領域に突入
            if count_pixcel > thre:
                detect_flag = 1

            # 色が検知できなくなった時がx座標の最大
            elif count_pixcel < thre and detect_flag == 1:
                y_max = y
                detect_flag = 0

        # 上
        end = int(coordinate[1]-mask_height*0.75)
        for y in range(y_max, end, -1):  # 下→上
            # 画像からはみ出したら終了
            if y == 0 or y == end + 1:
                if detect_flag == 0:
                    break
                elif y == 0:
                    y_min = color_img.shape[1] - 1
                    break
                elif y == end + 1:
                    y_min = end + 1
                    break

                else:
                    develop_img[coordinate[1]:coordinate[1]+2, :] = Color.BLUE.value
                    develop_img[:, coordinate[0]:coordinate[0]+2] = Color.BLUE.value
                    save_path = os.path.join(self.save_dir_path, "Error2.png")
                    cv2.imwrite(save_path, develop_img)
                    print(f"block_color: {block_color}")
                    print(f"coordinate: {coordinate}")
                    raise ValueError("Error2:色が見つかりません")

            # ピクセル数検出
            mask = color_img[y, coordinate[0]:coordinate[0]+mask_width]
            count_pixcel = np.count_nonzero(np.all(mask == block_color, axis=-1))

            # 探している物体(ブロックなど)の領域に突入
            if count_pixcel > thre:
                detect_flag = 1

            # 色が検知できなくなった時がx座標の最大
            elif count_pixcel < thre and detect_flag == 1:
                y_min = y
                detect_flag = 0

        if develop_flag == 1 and self.develop:
            develop_img[coordinate[1]:coordinate[1]+3, :] = Color.BLACK.value
            develop_img[coordinate[1]+mask_height*2:coordinate[1] +
                        mask_height*2+3, :] = Color.BLACK.value
            develop_img[y_min:y_min+3, :] = Color.RED.value
            develop_img[y_max:y_max+3, :] = Color.GREEN.value
            save_path = os.path.join(self.save_dir_path, "detail_coordi_circle.png")
            cv2.imwrite(save_path, develop_img)

        # 右
        end = int(coordinate[0]+mask_width*1.5)
        for x in range(coordinate[0]-mask_width, end):  # 左→右
            # 画像からはみ出したら終了
            if x >= color_img.shape[1]-1 or x == end-1:
                if detect_flag == 0:
                    break
                elif x >= color_img.shape[1]-1:
                    x_max = color_img.shape[1] - 1
                    break
                elif x == end-1:
                    x_max = end-1
                    break
                else:
                    print(f"detect_flag: {detect_flag}")
                    develop_img[:, coordinate[0]-mask_width:coordinate[0] -
                                mask_width+2] = Color.BLUE.value
                    develop_img[:, coordinate[0]+mask_width *
                                2:coordinate[0]+mask_width*2+2] = Color.BLUE.value
                    develop_img[:, x:x+5] = Color.BLACK.value
                    save_path = os.path.join(self.save_dir_path, "Error3.png")
                    cv2.imwrite(save_path, develop_img)
                    print(f"block_color: {block_color}")
                    print(f"coordinate : {coordinate}")
                    print(f"thre       : {thre}")
                    raise ValueError("Error3:色が見つかりません")

            # ピクセル数検出
            mask = color_img[y_min-2:y_max+2, x]
            count_pixcel = np.count_nonzero(np.all(mask == block_color, axis=-1))
            # 探している物体(ブロックなど)の領域に突入
            if count_pixcel > thre:
                detect_flag = 1
            # 色が検知できなくなった時がx座標の最大
            elif count_pixcel < thre and detect_flag == 1:
                x_max = x
                detect_flag = 0
                develop_img[:, x_max:x_max+2] = Color.GREEN.value
                develop_img[:, coordinate[0]+mask_width *
                            2:coordinate[0]+mask_width*2+2] = Color.GREEN.value

        # 左
        end = coordinate[0]-mask_width
        for x in range(x_max, end, -1):  # 右→左
            # 画像からはみ出したら終了
            if x == 0 or x == end + 1:
                if detect_flag == 0:
                    break
                elif x == 0:
                    x_min = coordinate[0]+mask_width*2-1
                    break
                elif x == end+1:
                    x_min = end + 1
                    break
                else:
                    develop_img[coordinate[1]:coordinate[1]+2, :] = Color.BLUE.value
                    develop_img[:, coordinate[0]:coordinate[0]+2] = Color.BLUE.value
                    save_path = os.path.join(self.save_dir_path, "Error4.png")
                    cv2.imwrite(save_path, develop_img)
                    print(f"block_color: {block_color}")
                    print(f"coordinate: {coordinate}")
                    raise ValueError("Error4:色が見つかりません")

            # ピクセル数検出
            mask = color_img[y_min-2:y_max+2, x]
            count_pixcel = np.count_nonzero(np.all(mask == block_color, axis=-1))

            # 探している物体(ブロックなど)の領域に突入
            if count_pixcel > thre:
                detect_flag = 1

            # 色が検知できなくなった時がx座標の最大
            elif count_pixcel < thre and detect_flag == 1:
                x_min = x
                detect_flag = 0

        if self.develop:
            develop_img[y_min:y_min+3, x_min:x_max] = Color.BLACK.value  # 上
            develop_img[y_min:y_max, x_max:x_max+3] = Color.BLACK.value  # 右
            develop_img[y_max:y_max+3, x_min:x_max] = Color.BLACK.value  # 下
            develop_img[y_min:y_max, x_min:x_min+3] = Color.BLACK.value  # 左

        detail_coordi = np.array([x_max, x_min, y_max, y_min])

        return detail_coordi, develop_img

    def detect_circle(self,
                      color_5_circle_img,
                      circle_color=Color.BLUE.value,
                      split_num_height=10,
                      split_num_width=10
                      ):
        """サークルの位置を発見する関数."""
        # サークル座標の作成
        # 大まかな座標を取得
        about_coordi, about_coordi_img = self.check_coordi_about(
            color_2_img=color_5_circle_img,
            search_color=circle_color,
            split_num_height=split_num_height,
            split_num_width=split_num_width,
            thre=5,
            develop_img=None,
            draw_color=Color.YELLOW.value)
        if self.develop and about_coordi_img is not None:
            about_coordi_img[about_coordi[1]:about_coordi[1]+2, :] = Color.RED.value
            about_coordi_img[:, about_coordi[0]:about_coordi[0]+2] = Color.RED.value
            save_path = os.path.join(self.save_dir_path, "about_coordi_circle.png")
            cv2.imwrite(save_path, about_coordi_img)

        # 細かな座標を取得
        if about_coordi is not None:
            # x_max, x_min, y_max, y_min
            detail_coordi, detail_coordi_img = self.check_coordi_detail(
                color_img=color_5_circle_img,
                coordinate=about_coordi,
                block_color=circle_color,
                split_num_height=split_num_height,
                split_num_width=split_num_width,
                thre=2,
                develop_flag=1,
                develop_img=None)
            if self.develop:
                save_path = os.path.join(self.save_dir_path, "detail_coordi_circle.png")
                cv2.imwrite(save_path, detail_coordi_img)
        else:
            return None, None

        return detail_coordi, detail_coordi_img

    def check_block_exist(self, circle_coordi, thre_x=100, thre_y=50, exist_block_develop_img=None):
        """サークル上にブロックがあるかチェックする関数.

        Args:
            circle_coordi: サークル座標([x_max, x_min, y_max, y_min])
        Return:
            bool: ブロックがサークル上にあるかどうか
        """
        circle_posi_x = int(circle_coordi[1] + (abs(circle_coordi[0] - circle_coordi[1]))/2)
        circle_posi_y = circle_coordi[2]
        if exist_block_develop_img is not None:
            exist_block_develop_img[circle_coordi[3]:circle_coordi[2],
                                    circle_posi_x:circle_posi_x+10] = Color.BLACK.value

        # 赤
        if np.all(self.block_coordi_red != 0) and \
                self.block_count_red <= self.red_block_num:
            block_posi_x = (self.block_coordi_red[0] + self.block_coordi_red[1]) / 2
            block_posi_y = self.block_coordi_red[2]
            diff_x = abs(circle_posi_x - block_posi_x)
            diff_y = abs(circle_posi_y - block_posi_y)
            if diff_x < thre_x and diff_y < thre_y:
                self.block_count_red += 1
                return self.red_block_num
        # 青1
        if np.all(self.block_coordi_blue1 != 0) and \
                self.block_count_blue <= self.blue_block_num:
            block_posi_x = int((self.block_coordi_blue1[0] + self.block_coordi_blue1[1]) / 2)
            block_posi_y = self.block_coordi_blue1[2]
            diff_x = abs(circle_posi_x - block_posi_x)
            diff_y = abs(circle_posi_y - block_posi_y)

            if diff_x < thre_x and diff_y < thre_y:
                self.block_count_blue += 1
                return self.blue_block_num
        # 青2
        if np.all(self.block_coordi_blue2 != 0) and \
                self.block_count_blue <= self.blue_block_num:
            block_posi_x = int((self.block_coordi_blue2[0] + self.block_coordi_blue2[1]) / 2)
            block_posi_y = self.block_coordi_blue2[2]
            diff_x = abs(circle_posi_x - block_posi_x)
            diff_y = abs(circle_posi_y - block_posi_y)

            if diff_x < thre_x and diff_y < thre_y:
                self.block_count_blue += 1
                return self.blue_block_num

        if self.develop and exist_block_develop_img is not None:
            save_path = os.path.join(self.save_dir_path, "exist_block_on_circle.png")
            cv2.imwrite(save_path, exist_block_develop_img)

        return 0

    def get_area_info(self, isR=True) -> None:
        """画像を6色画像に変換する関数.

        Args:
            game_area_img (np.ndarray): ゲームエリア画像
            save_path (str): 出力画像ファイルの保存パス

        Return:
            np.ndarray: コース情報 (Noneは取得失敗)
        """
        # 下準備
        image_path = os.path.join(self.image_dir_path, self.image_name)
        if not os.path.exists(image_path):
            raise (f"{image_path} is not exist")
        course_img = cv2.imread(image_path)
        if self.develop:
            save_path = os.path.join(self.save_dir_path, self.image_name)
            cv2.imwrite(save_path, course_img)
        self.img_shape = course_img.shape

        # Lコースの場合
        if not isR:
            # 画像を左右反転させる
            course_img = cv2.flip(course_img, 1)

        # 画像上のいらない領域を削除
        processed_course_img = self.pre_process_img(course_img)

        # BGR色空間からHSV色空間への変換
        game_area_img = cv2.cvtColor(processed_course_img, cv2.COLOR_BGR2HSV)

        # 2色画像の作成(赤、青)
        color_2_img = np.full_like(game_area_img, Color.WHITE.value, dtype=np.uint8)
        self.change_color(game_area_img, color_2_img, self.RED1, Color.RED.value)
        self.change_color(game_area_img, color_2_img, self.RED2, Color.RED.value)
        self.change_color(game_area_img, color_2_img, self.BLUE, Color.BLUE.value)

        # 5色画像の作成（黒以外）
        color_5_img = np.full_like(game_area_img, Color.WHITE.value, dtype=np.uint8)
        self.change_color(game_area_img, color_5_img, self.RED1, Color.RED.value)
        self.change_color(game_area_img, color_5_img, self.RED2, Color.RED.value)
        self.change_color(game_area_img, color_5_img, self.YELLOW, Color.YELLOW.value)
        self.change_color(game_area_img, color_5_img, self.GREEN, Color.GREEN.value)
        self.change_color(game_area_img, color_5_img, self.BLUE, Color.BLUE.value)

        # 画像の保存
        if self.develop:
            save_path = os.path.join(self.save_dir_path, "pre_processed_img.png")
            cv2.imwrite(save_path, processed_course_img)
            save_path = os.path.join(self.save_dir_path, "color_2_img.png")
            cv2.imwrite(save_path, color_2_img)
            save_path = os.path.join(self.save_dir_path, "color_5_img.png")
            cv2.imwrite(save_path, color_5_img)

        # ブロックの位置を特定する
        # ブロック特定用の画像を生成
        color_2_img_detect_block = color_2_img.copy()
        for x in range(color_2_img_detect_block.shape[1]):
            # 0.7は補正値
            y = int((self.basis_vector[1] / self.basis_vector[0]) * x * 0.5 + 200)
            color_2_img_detect_block[y:y+30, x] = Color.WHITE.value
        if self.develop:
            save_path = os.path.join(self.save_dir_path, "detect_block.png")
            cv2.imwrite(save_path, color_2_img_detect_block)

        color_5_circle_img = color_5_img.copy()

        # 赤
        # 大まかな座標を取得
        about_coordi, about_coordi_img = self.check_coordi_about(
            color_2_img_detect_block, Color.RED.value, 4, 30, 10, None, Color.BLACK.value)

        # 細かな座標を取得 ([x_max, x_min, y_max, y_min])
        self.block_coordi_red, detail_coordi_img = self.check_coordi_detail(
            color_2_img, about_coordi, Color.RED.value, develop_flag=1)
        if self.block_coordi_red is not None:
            color_5_circle_img[
                self.block_coordi_red[3]:self.block_coordi_red[2],
                self.block_coordi_red[1]:self.block_coordi_red[0]] = Color.WHITE.value
        else:
            print("赤ブロックが見つかりません")

        # 青1
        # 大まかな座標を取得
        about_coordi, about_coordi_img = self.check_coordi_about(
            color_2_img_detect_block,
            Color.BLUE.value,
            4, 30, 50,
            about_coordi_img,
            Color.YELLOW.value)
        # 細かな座標を取得
        self.block_coordi_blue1, detail_coordi_img = self.check_coordi_detail(
            color_2_img, about_coordi, Color.BLUE.value, thre=20, develop_img=detail_coordi_img)
        if self.block_coordi_blue1 is not None:
            color_5_circle_img[
                self.block_coordi_blue1[3]:self.block_coordi_blue1[2],
                self.block_coordi_blue1[1]:self.block_coordi_blue1[0]] = Color.WHITE.value
            color_2_img_detect_block[
                self.block_coordi_blue1[3]:self.block_coordi_blue1[2],
                self.block_coordi_blue1[1]:self.block_coordi_blue1[0]] = Color.WHITE.value
        else:
            print("青ブロックが見つかりません")

        # 青2
        # 大まかな座標を取得
        about_coordi, about_coordi_img = self.check_coordi_about(
            color_2_img_detect_block, Color.BLUE.value,
            4, 30, 50, about_coordi_img, Color.GREEN.value)
        # 細かな座標を取得
        self.block_coordi_blue2, detail_coordi_img = self.check_coordi_detail(
            color_2_img, about_coordi, Color.BLUE.value, thre=20)
        if self.block_coordi_blue2 is not None:
            color_5_circle_img[
                self.block_coordi_blue2[3]:self.block_coordi_blue2[2],
                self.block_coordi_blue2[1]:self.block_coordi_blue2[0]] = Color.WHITE.value
        else:
            print("青ブロックが見つかりません")

        if self.develop:
            save_path = os.path.join(self.save_dir_path, "about_coordi_block.png")
            cv2.imwrite(save_path, about_coordi_img)
            save_path = os.path.join(self.save_dir_path, "search_block.png")
            cv2.imwrite(save_path, detail_coordi_img)
            save_path = os.path.join(self.save_dir_path, "circle_only.png")
            cv2.imwrite(save_path, color_5_circle_img)

        # 2列目のサークルのコース画像を作成していく
        second_from_front_img = color_5_circle_img.copy()

        # 手間のサークルの領域だけの画像を作成
        front_course_img = color_5_circle_img.copy()
        for x in range(front_course_img.shape[1]):
            # y = (self.basis_vector[1] / self.basis_vector[0] * x * 0.6 + 160)
            y = (self.basis_vector[1] / self.basis_vector[0] * x * 0.6 + 175)
            front_course_img[:int(y), x] = Color.WHITE.value

        if self.develop:
            save_path = os.path.join(self.save_dir_path, "front_course_img.png")
            cv2.imwrite(save_path, front_course_img)

        # サークル座標の作成とブロックの存在確認
        # 青サークル1
        exist_block_develop_img = front_course_img.copy()
        coordi_circle = color_5_circle_img.copy()

        # サークル検出
        circle_blue_coordi1, _ = self.detect_circle(front_course_img, Color.BLUE.value, 10, 10)
        if circle_blue_coordi1 is not None:
            # ブロックの存在確認
            self.course_info_block[3, 1] = self.check_block_exist(circle_blue_coordi1)
            # 座標の追加
            self.course_info_coordinate[3, 1] = circle_blue_coordi1
            # 画像上のサークル削除
            front_course_img[
                circle_blue_coordi1[3]-5:circle_blue_coordi1[2]+5,
                circle_blue_coordi1[1]-30:circle_blue_coordi1[0]+30] = Color.WHITE.value
            second_from_front_img[
                circle_blue_coordi1[3]-5:circle_blue_coordi1[2]+5,
                circle_blue_coordi1[1]-30:circle_blue_coordi1[0]+30] = Color.WHITE.value
            if self.develop:
                coordi_circle[circle_blue_coordi1[3]:circle_blue_coordi1[3]+2,
                              circle_blue_coordi1[1]:circle_blue_coordi1[0]] = Color.BLACK.value
                coordi_circle[circle_blue_coordi1[3]:circle_blue_coordi1[2],
                              circle_blue_coordi1[0]:circle_blue_coordi1[0]+2] = Color.BLACK.value
                coordi_circle[circle_blue_coordi1[2]:circle_blue_coordi1[2]+2,
                              circle_blue_coordi1[1]:circle_blue_coordi1[0]] = Color.BLACK.value
                coordi_circle[circle_blue_coordi1[3]:circle_blue_coordi1[2],
                              circle_blue_coordi1[1]:circle_blue_coordi1[1]+2] = Color.BLACK.value
                save_path = os.path.join(self.save_dir_path, "coordi_circle.png")
                cv2.imwrite(save_path, coordi_circle)
                save_path = os.path.join(self.save_dir_path, "delete_circle.png")
                cv2.imwrite(save_path, front_course_img)

        # 青サークル2
        circle_blue_coordi2, _ = self.detect_circle(front_course_img, Color.BLUE.value, 10, 10)
        if circle_blue_coordi2 is not None:
            # ブロックの存在確認
            self.course_info_block[3, 0] = self.check_block_exist(circle_blue_coordi2)
            # 座標の追加
            self.course_info_coordinate[3, 0] = circle_blue_coordi2
            # 画像上のサークル削除
            front_course_img[
                circle_blue_coordi2[3]-5:circle_blue_coordi2[2]+5,
                circle_blue_coordi2[1]-30:circle_blue_coordi2[0]+30] = Color.WHITE.value
            second_from_front_img[
                circle_blue_coordi2[3]-5:circle_blue_coordi2[2]+5,
                circle_blue_coordi2[1]-30:circle_blue_coordi2[0]+30] = Color.WHITE.value
            if self.develop:
                coordi_circle[circle_blue_coordi2[3]:circle_blue_coordi2[3]+2,
                              circle_blue_coordi2[1]:circle_blue_coordi2[0]] = Color.BLACK.value
                coordi_circle[circle_blue_coordi2[3]:circle_blue_coordi2[2],
                              circle_blue_coordi2[0]:circle_blue_coordi2[0]+2] = Color.BLACK.value
                coordi_circle[circle_blue_coordi2[2]:circle_blue_coordi2[2]+2,
                              circle_blue_coordi2[1]:circle_blue_coordi2[0]] = Color.BLACK.value
                coordi_circle[circle_blue_coordi2[3]:circle_blue_coordi2[2],
                              circle_blue_coordi2[1]:circle_blue_coordi2[1]+2] = Color.BLACK.value
                save_path = os.path.join(self.save_dir_path, "coordi_circle.png")
                cv2.imwrite(save_path, coordi_circle)
                save_path = os.path.join(self.save_dir_path, "delete_circle.png")
                cv2.imwrite(save_path, front_course_img)

        # 座標が逆だった場合
        if circle_blue_coordi1 is not None and circle_blue_coordi2 is not None and \
                circle_blue_coordi2[0] > circle_blue_coordi1[0]:
            self.course_info_block[3, 0], self.course_info_block[3, 1] = \
                self.course_info_block[3, 1], self.course_info_block[3, 0]
            self.course_info_coordinate[3, 0] = circle_blue_coordi1
            self.course_info_coordinate[3, 1] = circle_blue_coordi2

        # 緑サークル1
        circle_green_coordi1, _ = self.detect_circle(front_course_img, Color.GREEN.value, 10, 8)
        if circle_green_coordi1 is not None:
            # ブロックの存在確認
            self.course_info_block[3, 2] = self.check_block_exist(
                circle_green_coordi1, exist_block_develop_img=exist_block_develop_img)
            # 座標の追加
            self.course_info_coordinate[3, 2] = circle_green_coordi1
            # 画像上のサークル削除
            front_course_img[
                circle_green_coordi1[3]-5:circle_green_coordi1[2]+5,
                circle_green_coordi1[1]-30:circle_green_coordi1[0]+30] = Color.WHITE.value
            second_from_front_img[
                circle_green_coordi1[3]-5:circle_green_coordi1[2]+5,
                circle_green_coordi1[1]-30:circle_green_coordi1[0]+30] = Color.WHITE.value
            if self.develop:
                coordi_circle[circle_green_coordi1[3]:circle_green_coordi1[3]+2,
                              circle_green_coordi1[1]:circle_green_coordi1[0]] = Color.BLACK.value
                coordi_circle[circle_green_coordi1[3]:circle_green_coordi1[2],
                              circle_green_coordi1[0]:circle_green_coordi1[0]+2] = Color.BLACK.value
                coordi_circle[circle_green_coordi1[2]:circle_green_coordi1[2]+2,
                              circle_green_coordi1[1]:circle_green_coordi1[0]] = Color.BLACK.value
                coordi_circle[circle_green_coordi1[3]:circle_green_coordi1[2],
                              circle_green_coordi1[1]:circle_green_coordi1[1]+2] = Color.BLACK.value
                save_path = os.path.join(self.save_dir_path, "coordi_circle.png")
                cv2.imwrite(save_path, coordi_circle)

                save_path = os.path.join(self.save_dir_path, "delete_circle.png")
                cv2.imwrite(save_path, front_course_img)

        # 緑サークル2
        circle_green_coordi2, _ = self.detect_circle(front_course_img, Color.GREEN.value, 10, 8)
        if circle_green_coordi2 is not None:
            # ブロックの存在確認
            self.course_info_block[3, 3] = self.check_block_exist(
                circle_green_coordi2, exist_block_develop_img=exist_block_develop_img)
            # 座標の追加
            self.course_info_coordinate[3, 3] = circle_green_coordi2
            # 画像上のサークル削除
            front_course_img[
                circle_green_coordi2[3]-5:circle_green_coordi2[2]+5,
                circle_green_coordi2[1]-30:circle_green_coordi2[0]+30] = Color.WHITE.value
            second_from_front_img[
                circle_green_coordi2[3]-5:circle_green_coordi2[2]+5,
                circle_green_coordi2[1]-30:circle_green_coordi2[0]+30] = Color.WHITE.value
            if self.develop:
                coordi_circle[circle_green_coordi2[3]:circle_green_coordi2[3]+2,
                              circle_green_coordi2[1]:circle_green_coordi2[0]] = Color.BLACK.value
                coordi_circle[circle_green_coordi2[3]:circle_green_coordi2[2],
                              circle_green_coordi2[0]:circle_green_coordi2[0]+2] = Color.BLACK.value
                coordi_circle[circle_green_coordi2[2]:circle_green_coordi2[2]+2,
                              circle_green_coordi2[1]:circle_green_coordi2[0]] = Color.BLACK.value
                coordi_circle[circle_green_coordi2[3]:circle_green_coordi2[2],
                              circle_green_coordi2[1]:circle_green_coordi2[1]+2] = Color.BLACK.value
                save_path = os.path.join(self.save_dir_path, "coordi_circle.png")
                cv2.imwrite(save_path, coordi_circle)
                save_path = os.path.join(self.save_dir_path, "delete_circle.png")
                cv2.imwrite(save_path, front_course_img)
                save_path = os.path.join(self.save_dir_path, "second_from_front.png")
                cv2.imwrite(save_path, second_from_front_img)

        # 座標が逆だった場合
        if circle_green_coordi1 is not None and circle_green_coordi2 is not None and \
                circle_green_coordi1[0] > circle_green_coordi2[0]:
            self.course_info_block[3, 2], self.course_info_block[3, 3] = \
                self.course_info_block[3, 3], self.course_info_block[3, 2]
            self.course_info_coordinate[3, 2] = circle_green_coordi2
            self.course_info_coordinate[3, 3] = circle_green_coordi1

        # すべてのブロックを手前の列で見つけた場合、終了
        if self.block_count_red + self.block_count_blue > 2:
            print("ブロックすべて発見1")
            return self.course_info_block

        # 2列目のサークルを見つけていく
        """NOTE
        1列目の場合は2列目以降を削除していたので後ろのサークルを誤検知することはなかったが、
        2列目と3列目は近いため、3列目以降を削除することは危険。
        よって、誤検知も考慮しなければならない
        """
        # 2列目のサークル検出のための画像作成(いらない場所を白で埋めて、誤検出のリスクを減らす)
        second_are_img = second_from_front_img.copy()

        # いらない色を削除する範囲の設定
        # コース0,1列以外の範囲
        range_1_left = int(self.course_info_coordinate[3, 0, 1])  # 左青のx_min
        range_1_right = int(self.course_info_coordinate[3, 2, 1])  # 左緑のx_min
        # 色条件
        condition1_1 = np.all(second_are_img[:, range_1_right:]
                              == Color.BLUE.value, axis=-1, keepdims=True)
        condition1_2 = np.all(second_are_img[:, :range_1_left]
                              == Color.BLUE.value, axis=-1, keepdims=True)
        condition2_1 = np.all(second_are_img[:, range_1_right:]
                              == Color.RED.value, axis=-1, keepdims=True)
        condition2_2 = np.all(second_are_img[:, :range_1_left]
                              == Color.RED.value, axis=-1, keepdims=True)
        # 青と赤の削除(白に変換)
        if range_1_left != 0:
            second_are_img[:, :range_1_left] = \
                np.where(condition1_2 | condition2_2, Color.WHITE.value,
                         second_are_img[:, :range_1_left])
        if range_1_right != 0:
            second_are_img[:, range_1_right:] = \
                np.where(condition1_1 | condition2_1, Color.WHITE.value,
                         second_are_img[:, range_1_right:])

        # コース3,4列以外の範囲
        range_2_left = int(self.course_info_coordinate[3, 1, 0])  # 右青のx_maxより左側
        range_2_right = int(self.course_info_coordinate[3, 3, 0])  # 右緑のx_maxより右側
        # 色条件
        condition3_1 = np.all(second_are_img[:, range_2_right:]
                              == Color.GREEN.value, axis=-1, keepdims=True)
        condition3_2 = np.all(second_are_img[:, :range_2_left]
                              == Color.GREEN.value, axis=-1, keepdims=True)
        condition4_1 = np.all(second_are_img[:, range_2_right:]
                              == Color.YELLOW.value, axis=-1, keepdims=True)
        condition4_2 = np.all(second_are_img[:, :range_2_left]
                              == Color.YELLOW.value, axis=-1, keepdims=True)
        # 緑と黄の削除(白に変換)
        if range_2_left != 0:
            second_are_img[:, :range_2_left] = \
                np.where(condition3_2 | condition4_2, Color.WHITE.value,
                         second_are_img[:, :range_2_left])
        if range_2_right != 0:
            second_are_img[:, range_2_right:] = \
                np.where(condition3_1 | condition4_1, Color.WHITE.value,
                         second_are_img[:, range_2_right:])

        # 上側
        y_list = [y for y in self.course_info_coordinate[3, :, 3] if y > 0]
        if len(y_list) > 0:
            range_upper = int(min(y_list)) - 60
            condition5 = np.all(second_are_img[:range_upper, :] ==
                                Color.BLUE.value, axis=-1, keepdims=True)
            condition6 = np.all(second_are_img[:range_upper, :] ==
                                Color.GREEN.value, axis=-1, keepdims=True)
            second_are_img[:range_upper, :] = \
                np.where(condition5, Color.WHITE.value, second_are_img[:range_upper, :])
            second_are_img[:range_upper, :] = \
                np.where(condition6, Color.WHITE.value, second_are_img[:range_upper, :])

        # 真ん中のロゴを消す
        center_range_left = int(self.course_info_coordinate[3, 1, 0])  # 右青x_max
        center_range_right = int(self.course_info_coordinate[3, 2, 1])  # 左緑x_min
        if not (center_range_left == 0 or center_range_right == 0):
            condition7 = np.all(
                second_are_img[:, center_range_left:center_range_right] == Color.GREEN.value,
                axis=-1,
                keepdims=True)
            second_are_img[:, center_range_left:center_range_right] = \
                np.where(condition7,
                         Color.WHITE.value,
                         second_are_img[:, center_range_left:center_range_right])

        if self.develop:
            save_path = os.path.join(self.save_dir_path, "second_are_before.png")
            cv2.imwrite(save_path, second_from_front_img)
            save_path = os.path.join(self.save_dir_path, "second_are.png")
            cv2.imwrite(save_path, second_are_img)

        # 検出した場所の描画用画像
        if self.develop:
            develop_img = color_5_circle_img.copy()

        # Second: 青サークル1
        second_circle_blue_coordi1, detail_coordi_img = \
            self.detect_circle(second_are_img, Color.BLUE.value, 10, 10)  # サークル検出
        if second_circle_blue_coordi1 is not None:
            # ブロックの存在確認
            self.course_info_block[2, 1] = self.check_block_exist(
                second_circle_blue_coordi1, 100, 30)
            # 座標の追加
            self.course_info_coordinate[2, 1] = second_circle_blue_coordi1
            # 画像上のサークル削除
            front_course_img[
                second_circle_blue_coordi1[3]-5:second_circle_blue_coordi1[2]+5,
                second_circle_blue_coordi1[1]-30:second_circle_blue_coordi1[0]+30] \
                = Color.WHITE.value
            second_are_img[
                second_circle_blue_coordi1[3]-5:second_circle_blue_coordi1[2]+5,
                second_circle_blue_coordi1[1]-30:second_circle_blue_coordi1[0]+30] \
                = Color.WHITE.value
            # 2つめのサークルを見つけた時点で。上側に青があるのがおかしいので、上側の青を削除
            y = second_circle_blue_coordi1[2] - 30  # 30は補正値
            if self.develop:
                develop_img[
                    second_circle_blue_coordi1[3]:second_circle_blue_coordi1[3]+2,
                    second_circle_blue_coordi1[1]:second_circle_blue_coordi1[0]] \
                    = Color.BLACK.value
                develop_img[
                    second_circle_blue_coordi1[3]:second_circle_blue_coordi1[2],
                    second_circle_blue_coordi1[0]:second_circle_blue_coordi1[0]+2] \
                    = Color.BLACK.value
                develop_img[
                    second_circle_blue_coordi1[2]:second_circle_blue_coordi1[2]+2,
                    second_circle_blue_coordi1[1]:second_circle_blue_coordi1[0]] \
                    = Color.BLACK.value
                develop_img[
                    second_circle_blue_coordi1[3]:second_circle_blue_coordi1[2],
                    second_circle_blue_coordi1[1]:second_circle_blue_coordi1[1]+2] \
                    = Color.BLACK.value
                save_path = os.path.join(self.save_dir_path, "second_detect_circle.png")
                cv2.imwrite(save_path, develop_img)
                save_path = os.path.join(self.save_dir_path, "second_delete_circle.png")
                cv2.imwrite(save_path, second_are_img)
        else:
            if self.develop:
                print("サークルなし1")

        # Second: 青サークル2
        second_circle_blue_coordi2, detail_coordi_img = \
            self.detect_circle(second_are_img, Color.BLUE.value, 10, 10)  # サークル検出
        if second_circle_blue_coordi2 is not None:
            # ブロックの存在確認
            self.course_info_block[2, 0] = self.check_block_exist(
                second_circle_blue_coordi2, 100, 30)
            # 座標の追加
            self.course_info_coordinate[2, 0] = second_circle_blue_coordi2
            # 画像上のサークル削除
            front_course_img[second_circle_blue_coordi2[3]-5:second_circle_blue_coordi2[2]+5,
                             second_circle_blue_coordi2[1]-30:second_circle_blue_coordi2[0]+30] \
                = Color.WHITE.value
            second_are_img[second_circle_blue_coordi2[3]-5:second_circle_blue_coordi2[2]+5,
                           second_circle_blue_coordi2[1]-30:second_circle_blue_coordi2[0]+30] \
                = Color.WHITE.value
            if self.develop:
                develop_img[second_circle_blue_coordi2[3]:second_circle_blue_coordi2[3]+2,
                            second_circle_blue_coordi2[1]:second_circle_blue_coordi2[0]] \
                    = Color.BLACK.value
                develop_img[second_circle_blue_coordi2[3]:second_circle_blue_coordi2[2],
                            second_circle_blue_coordi2[0]:second_circle_blue_coordi2[0]+2] \
                    = Color.BLACK.value
                develop_img[second_circle_blue_coordi2[2]:second_circle_blue_coordi2[2]+2,
                            second_circle_blue_coordi2[1]:second_circle_blue_coordi2[0]] \
                    = Color.BLACK.value
                develop_img[second_circle_blue_coordi2[3]:second_circle_blue_coordi2[2],
                            second_circle_blue_coordi2[1]:second_circle_blue_coordi2[1]+2] \
                    = Color.BLACK.value
                save_path = os.path.join(self.save_dir_path, "second_detect_circle.png")
                cv2.imwrite(save_path, develop_img)
                save_path = os.path.join(self.save_dir_path, "second_delete_circle.png")
                cv2.imwrite(save_path, second_are_img)
        else:
            if self.develop:
                print("サークルなし2")

        # 座標が逆だった場合
        if second_circle_blue_coordi1 is not None and second_circle_blue_coordi2 is not None and \
                second_circle_blue_coordi2[0] > second_circle_blue_coordi1[0]:
            self.course_info_block[2, 0], self.course_info_block[2, 1] = \
                self.course_info_block[2, 1], self.course_info_block[2, 0]
            self.course_info_coordinate[2, 0] = second_circle_blue_coordi1
            self.course_info_coordinate[2, 1] = second_circle_blue_coordi2

        # すべてのブロックを手前の列で見つけた場合、終了
        if self.block_count_red + self.block_count_blue > 2:
            print("ブロックすべて発見2")
            return self.course_info_block

        # 2列目の緑サークルを検出
        # いらない色を削除する範囲の設定
        # Second: 緑サークル1
        second_circle_green_coordi1, detail_coordi_img = \
            self.detect_circle(second_are_img, Color.GREEN.value, 10, 10)  # サークル検出
        if second_circle_green_coordi1 is not None:
            # ブロックの存在確認
            self.course_info_block[2, 2] = self.check_block_exist(
                second_circle_green_coordi1, 100, 30)
            # 座標の追加
            self.course_info_coordinate[2, 2] = second_circle_green_coordi1
            # 画像上のサークル削除
            front_course_img[second_circle_green_coordi1[3]-5:second_circle_green_coordi1[2]+5,
                             second_circle_green_coordi1[1]-30:second_circle_green_coordi1[0]+30] \
                = Color.WHITE.value
            second_are_img[second_circle_green_coordi1[3]-5:second_circle_green_coordi1[2]+5,
                           second_circle_green_coordi1[1]-30:second_circle_green_coordi1[0]+30] \
                = Color.WHITE.value
            # 2つめのサークルを見つけた時点で。上側に青があるのがおかしいので、上側の青を削除
            y = second_circle_green_coordi1[2] - 30  # 30は補正値
            if self.develop:
                develop_img[second_circle_green_coordi1[3]:second_circle_green_coordi1[3]+2,
                            second_circle_green_coordi1[1]:second_circle_green_coordi1[0]] \
                    = Color.BLACK.value
                develop_img[second_circle_green_coordi1[3]:second_circle_green_coordi1[2],
                            second_circle_green_coordi1[0]:second_circle_green_coordi1[0]+2] \
                    = Color.BLACK.value
                develop_img[second_circle_green_coordi1[2]:second_circle_green_coordi1[2]+2,
                            second_circle_green_coordi1[1]:second_circle_green_coordi1[0]] \
                    = Color.BLACK.value
                develop_img[second_circle_green_coordi1[3]:second_circle_green_coordi1[2],
                            second_circle_green_coordi1[1]:second_circle_green_coordi1[1]+2] \
                    = Color.BLACK.value
                save_path = os.path.join(self.save_dir_path, "second_detect_circle.png")
                cv2.imwrite(save_path, develop_img)
                save_path = os.path.join(self.save_dir_path, "second_delete_circle.png")
                cv2.imwrite(save_path, second_are_img)
        else:
            if self.develop:
                print("サークルなし3")

        # Second: 緑サークル2
        second_circle_green_coordi2, detail_coordi_img = \
            self.detect_circle(second_are_img, Color.GREEN.value, 10, 10)  # サークル検出
        if second_circle_green_coordi2 is not None:
            # ブロックの存在確認
            self.course_info_block[2, 3] = self.check_block_exist(
                second_circle_green_coordi2, 100, 30)
            # 座標の追加
            self.course_info_coordinate[2, 3] = second_circle_green_coordi2
            # 画像上のサークル削除
            front_course_img[second_circle_green_coordi2[3]-5:second_circle_green_coordi2[2]+5,
                             second_circle_green_coordi2[1]-30:second_circle_green_coordi2[0]+30] \
                = Color.WHITE.value
            second_are_img[second_circle_green_coordi2[3]-5:second_circle_green_coordi2[2]+5,
                           second_circle_green_coordi2[1]-30:second_circle_green_coordi2[0]+30] \
                = Color.WHITE.value
            if self.develop:
                develop_img[second_circle_green_coordi2[3]:second_circle_green_coordi2[3]+2,
                            second_circle_green_coordi2[1]:second_circle_green_coordi2[0]] \
                    = Color.BLACK.value
                develop_img[second_circle_green_coordi2[3]:second_circle_green_coordi2[2],
                            second_circle_green_coordi2[0]:second_circle_green_coordi2[0]+2] \
                    = Color.BLACK.value
                develop_img[second_circle_green_coordi2[2]:second_circle_green_coordi2[2]+2,
                            second_circle_green_coordi2[1]:second_circle_green_coordi2[0]] \
                    = Color.BLACK.value
                develop_img[second_circle_green_coordi2[3]:second_circle_green_coordi2[2],
                            second_circle_green_coordi2[1]:second_circle_green_coordi2[1]+2] \
                    = Color.BLACK.value
                save_path = os.path.join(self.save_dir_path, "second_detect_circle.png")
                cv2.imwrite(save_path, develop_img)
                save_path = os.path.join(self.save_dir_path, "second_delete_circle.png")
                cv2.imwrite(save_path, second_are_img)
        else:
            if self.develop:
                print("サークルなし4")

        # 座標が逆だった場合
        if second_circle_green_coordi1 is not None and second_circle_green_coordi2 is not None and \
                second_circle_green_coordi1[0] > second_circle_green_coordi2[0]:
            self.course_info_block[2, 2], self.course_info_block[2, 3] = \
                self.course_info_block[2, 3], self.course_info_block[2, 2]
            self.course_info_coordinate[2, 2] = second_circle_green_coordi1
            self.course_info_coordinate[2, 3] = second_circle_green_coordi2

        # すべてのブロックを手前の列で見つけた場合、終了
        if self.block_count_red + self.block_count_blue > 2:
            print("ブロックすべて発見3")
            return self.course_info_block

        # 終了
        return self.course_info_block


if __name__ == "__main__":
    # 処理時間計測用
    import time
    import argparse
    start = time.time()

    parser = argparse.ArgumentParser(description="リアカメラに関するプログラム")
    parser.add_argument("-isR", "--isR", default=True, type=bool,
                        help='コースが右(True)か左か(False)')
    parser.add_argument("-develop", "--develop", action='store_true',
                        help='開発モード')
    args = parser.parse_args()

    # 作業用の読み込みや保存用のディレクトリパス
    work_dir_path = os.path.join(PROJECT_DIR_PATH, "tests", "test_data", "block_area_img")
    save_dir_path = os.path.join(PROJECT_DIR_PATH, "work_image_data")

    test_images = os.listdir(work_dir_path)

    image_number = "20"
    image_number = "_9"

    for img in test_images:
        if img[-6:-4] == image_number:
            image_name = img

    info = GetAreaInfo(image_name, work_dir_path, save_dir_path, develop=args.develop)
    try:
        info.get_area_info(args.isR)
    finally:
        # """
        print(f"最終結果--------------------------------------------------------------")
        print(f"block_coordi_red   : {info.block_coordi_red}")
        print(f"block_coordi_blue1 : {info.block_coordi_blue1}")
        print(f"block_coordi_blue2 : {info.block_coordi_blue2}")
        print(f"block_count_red    : {info.block_count_red}")
        print(f"block_count_blue   : {info.block_count_blue}")
        # print(f"course_info_coordinate :\n[[x_max, x_min, y_max, y_min]]\n\
        #       {info.course_info_coordinate[2:]}")
        print(f"course_info_block :\n{info.course_info_block}")
        print(f"---------------------------------------------------------------------")
        # """

    # 処理時間計測用
    execute_time = time.time() - start

    print(f"実行時間: {str(execute_time)[:5]}s")
    print("完了！")
