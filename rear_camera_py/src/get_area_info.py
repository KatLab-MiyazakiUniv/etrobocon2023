"""ブロックでとレジャーのエリア情報を取得するモジュール.

@author: kawanoichi
"""
import numpy as np
import cv2
import os
from enum import Enum
import sys

from line_calculator import LineCalculator

script_dir = os.path.dirname(os.path.abspath(__file__))  # /src
PROJECT_DIR_PATH = os.path.dirname(script_dir)  # /rear_camera_py


class RgbColor(Enum):
    """色クラス."""

    BLACK = [0, 0, 0]
    RED = [0, 0, 255]
    YELLOW = [0, 255, 255]
    GREEN = [0, 255, 0]
    BLUE = [255, 0, 0]
    WHITE = [255, 255, 255]


class GetAreaInfo:
    """ブロックdeトレジャーのエリア情報を取得するクラス."""

    # 色統一のための各色のHSV値の範囲
    # OpenCVのHSV空間の上限はどれも255
    RED1 = [(0, 15), (90, 255), (100, 255)]
    YELLOW = [(16, 37), (50, 255), (120, 255)]
    GREEN = [(38, 95), (40, 255), (40, 255)]
    BLUE = [(96, 150), (95, 255), (80, 255)]
    RED2 = [(171, 190), (80, 255), (90, 255)]

    BLACK = [(0, 255), (0, 50), (0, 140)]

    # 各ブロックを表す値
    RED_BLOCK_NUM = 1
    BLUE_BLOCK_NUM = 2

    def __init__(self,
                 image_name,
                 image_dir_path,
                 save_dir_path=None,
                 develop=None):
        """GetAreaInfoのコンストラクタ.

        Args:
            image_name: ブロックエリア画像
            image_dir_path: ブロックエリア画像保存しているディレクトリパス
            save_dir_path: 開発用画像の保存先ディレクトリパス
            develop: 開発用かの判断フラグ
        """
        # 画像パス・保存関連
        self.image_name = image_name
        self.image_dir_path = image_dir_path
        self.save_dir_path = save_dir_path if save_dir_path is not None else image_dir_path
        self.develop = develop

        # ベクトル関連
        self.basis_vector = None  # 緑の線の傾き
        self.green_area_b = None  # 緑の線の切片

        # 探すブロックの数
        self.block_max_count_red = 1
        self.block_max_count_blue = 2

        # 探したブロックの数
        self.block_count_red = 0
        self.block_count_blue = 0

        # ブロック情報
        self.block_coordi_red = np.zeros(4)
        self.block_coordi_blue1 = np.zeros(4)
        self.block_coordi_blue2 = np.zeros(4)
        self.course_info_block = np.zeros((4, 4))

        # サークル関連
        self.course_info_coordinate = np.zeros((4, 4, 4))

        # コース情報関連
        # 行
        self.row_verctor_0 = np.array([None, None])  # [傾き, 切片]
        self.row_verctor_1 = np.array([None, None])
        self.row_verctor_2 = np.array([None, None])
        self.row_verctor_3 = np.array([None, None])
        # 列
        self.column_verctor_0 = np.array([None, None])  # [傾き, 切片]
        self.column_verctor_1 = np.array([None, None])
        self.column_verctor_2 = np.array([None, None])
        self.column_verctor_3 = np.array([None, None])

    def change_color(self, hsv_img, write_img, search_color, color_value) -> None:
        """一定の範囲のHSV値を指定したHSV値に統一する関数.

        Args:
            hsv_img (np.ndarray): HSV値に変換した画像
            write_img (np.ndarray): 統一した値を書き込む画像
            search_color (list): 一定の範囲を示すHSV値 (self.RED1などを指定)
            color_value (list): 統一するHSV値 (RgbColor.RED.valueなど)
        """
        write_img[np.where((search_color[0][0] <= hsv_img[:, :, 0]) &
                           (hsv_img[:, :, 0] <= search_color[0][1]) &
                           (search_color[1][0] <= hsv_img[:, :, 1]) &
                           (hsv_img[:, :, 1] <= search_color[1][1]) &
                           (search_color[2][0] <= hsv_img[:, :, 2]) &
                           (hsv_img[:, :, 2] <= search_color[2][1]))
                  ] = color_value

    def detect_line(self, img, length_threshold=200) -> (np.ndarray, int):
        """線分検出を行う関数.

        Args:
            img (np.ndarray): 線分検出を行う画像
            length_threshold (int): 線分と認識する線の長さの閾値
        Return:
            lines: 検出したライン
            max_index: linesの中の最長ラインのインデックス
        """
        # グレースケール化
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        length_threshold_pix = length_threshold
        distance_threshold = 1.41421356
        canny_th1 = 50
        canny_th2 = 50
        canny_aperture_size = 3
        do_merge = False

        # 線分検出モジュールのインスタンス化
        fast_line_detector = cv2.ximgproc.createFastLineDetector(
            length_threshold_pix,
            distance_threshold,
            canny_th1,
            canny_th2,
            canny_aperture_size,
            do_merge
        )

        lines = fast_line_detector.detect(gray)
        if lines is None:
            return None, None

        # 最も長い線分を見つける
        max_length = 0
        max_length_index = -1
        for i, line in enumerate(lines):
            x1, y1, x2, y2 = map(int, line[0])
            length = np.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2)
            if length > max_length:
                max_length = length
                max_length_index = i

        return lines[:, 0], max_length_index

    def change_coordi_color(self, img, coordi, color=RgbColor.WHITE.value, padding_value_x=0, padding_value_y=0):
        """指定した座標(ブロック状)の色を変換する関数.

        Args:
            coordi: 消したい場所の範囲を示す座標
        """
        mask = coordi.copy()
        mask[0] += padding_value_x
        mask[1] -= padding_value_x
        mask[2] += padding_value_y
        mask[3] -= padding_value_y
        if mask[0] >= img.shape[1]:
            mask[0] = img.shape[1] - 1
        if mask[1] < 0:
            mask[1] = 0
        if mask[2] >= img.shape[0]:
            mask[2] = img.shape[0] - 1
        if mask[3] < 0:
            mask[3] = 0
        img[mask[3]:mask[2],
            mask[1]:mask[0]] = color

    def pre_process_img(self, course_img) -> None:
        """画像のいらない部分を削除する関数.

        画像上の以下を削除(白色で上書き)
        - コース下の緑(以下)の部分削除
        - コース上側を削除
        - IoT列車のコースを含めた上側削除

        Args:
            course_img (np.ndarray): コース画像(BGR)
        """
        block_area_height = 240
        # BGR色空間からHSV色空間への変換
        game_area_img = cv2.cvtColor(course_img, cv2.COLOR_BGR2HSV)

        # 処理結果を保持する配列を宣言(色を白で初期化)
        green_area_img = np.full_like(game_area_img, RgbColor.WHITE.value, dtype=np.uint8)

        # コース下の緑(以下)の部分削除 & コース上側を削除
        # 画像の緑の部分だけを抽出
        self.change_color(game_area_img, green_area_img, self.GREEN, RgbColor.GREEN.value)

        # 線分を検出
        lines, max_index = self.detect_line(green_area_img, length_threshold=200)

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
                course_img[int(a * x + self.green_area_b):, x] = RgbColor.WHITE.value
                # コースの上側削除
                course_img[:int(a * x + self.green_area_b - block_area_height), x] \
                    = RgbColor.WHITE.value
            # トリミング
            upper_delete_line = \
                int(min(self.green_area_b - block_area_height,
                        a * course_img.shape[1] + self.green_area_b - block_area_height))
            lower_delete_line = int(
                max(self.green_area_b, a * course_img.shape[1] + self.green_area_b))
            course_img = course_img[upper_delete_line:lower_delete_line, :, :]

        else:
            return None, None

        """
        IoT列車のコースを含めた上側削除
        """
        # BGR色空間からHSV色空間への変換(下側削除Ver)
        game_area_img = cv2.cvtColor(course_img, cv2.COLOR_BGR2HSV)

        # 処理結果を保持する配列を宣言(色を白で初期化)
        blue_area_img = np.full_like(game_area_img, RgbColor.WHITE.value, dtype=np.uint8)

        # 画像の青い部分だけを抽出
        self.change_color(game_area_img, blue_area_img, self.BLUE, RgbColor.BLUE.value)

        # 線分を検出
        lines, max_index = self.detect_line(blue_area_img, length_threshold=150)
        train_area_img = game_area_img.copy()
        if lines is not None:
            # 最も長い線を使用
            x1, y1, x2, y2 = map(int, lines[max_index])
            vector = np.array([x2 - x1, y2 - y1])

            if vector[0] == 0:
                a = 0
            else:
                a = vector[1] / vector[0]
            b = y1 - a * x1 + 10

            for x in range(course_img.shape[1]):
                # 「course_img.shape[1]*0.3」はIoT列車領域の右端
                if int(a * x + b) < 0 or x >= course_img.shape[1]*0.26:
                    # if int(a * x + b) < 0:
                    train_area_img[:, x:] = RgbColor.WHITE.value
                    break
                if int(a * x + b) < course_img.shape[0]:
                    train_area_img[int(a * x + b+5):, x] = RgbColor.WHITE.value
            self.change_color(train_area_img, course_img, self.RED1, RgbColor.WHITE.value)
            self.change_color(train_area_img, course_img, self.YELLOW, RgbColor.WHITE.value)
            self.change_color(train_area_img, course_img, self.GREEN, RgbColor.WHITE.value)
            self.change_color(train_area_img, course_img, self.BLACK, RgbColor.WHITE.value)
            self.change_color(train_area_img, course_img, self.BLUE, RgbColor.WHITE.value)

        return course_img, train_area_img

    def check_coordi_about(self,
                           color_3_img,
                           search_color=RgbColor.RED.value,
                           split_num_height=4,
                           split_num_width=30,
                           thre=10,
                           develop_img=None,
                           ) -> (np.ndarray, np.ndarray):
        """色(ブロックやサークル)のおおまかな場所を特定する関数.

        Args:
            color_3_img (np.ndarray): コース画像
            search_color (list): 探すブロックの色
            split_num_height (int): 画像の縦を何等分するかの値
            split_num_width (int): 画像の横を何等分するかの値
            thre (int): ブロックがあると認識するピクセルの閾値
            develop_img (np.ndarray): ブロックの位置を描画する画像(開発用)

        Return:
            mask_coordi: ブロックの位置の大まかな座標(x, y)
            develop_img: ブロックの位置を描画した画像(開発用)
        """
        mask_height = int(color_3_img.shape[0] / split_num_height)
        mask_width = int(color_3_img.shape[1] / split_num_width)

        # (ピクセル数, x, y)
        mask_coordi = np.array([0, 0])
        max_pixcel = 0

        # おおまかなブロックの場所を特定
        for x in range(0, int(color_3_img.shape[1]), mask_width):
            masks = []
            for y in range(split_num_height):
                masks.append(color_3_img[mask_height *
                             y:mask_height*(y+1), x:x+mask_width])

            # 画素数のカウント
            for i, mask in enumerate(masks):
                count_pixcel = np.count_nonzero(np.all(mask == search_color, axis=-1))
                if count_pixcel > max_pixcel and count_pixcel > thre:
                    mask_coordi = np.array([x, i*mask_height])
                    max_pixcel = count_pixcel

        if np.all(mask_coordi == 0):
            return None

        if self.develop and develop_img is not None:
            # マスクの大きさを表す線
            develop_img[mask_height, :] = RgbColor.BLACK.value
            develop_img[:, mask_width] = RgbColor.BLACK.value
            # ブロックを囲んだマスクの位置を描画
            cv2.rectangle(develop_img,
                          (mask_coordi[0], mask_coordi[1]),  # 左上
                          (mask_coordi[0]+mask_width, mask_coordi[1]+mask_height),  # 右下
                          RgbColor.BLACK.value, 2)
        return mask_coordi

    def check_coordi_detail(self,
                            color_img,
                            coordinate,
                            block_color=RgbColor.RED.value,
                            split_num_height=4,
                            split_num_width=30,
                            thre=5,
                            develop_img=None
                            ) -> (np.ndarray):
        """色(ブロックやサークル)の正確な位置を特定.

        Args:
            color_img (np.ndarray): 二色画像(赤、青)
            coordinate (np.ndarray): ブロックの大まかな座標
            develop_img (np.ndarray): 実行結果を書き込む画像
            block_color (list): 対象とするブロックの色
            split_num_height (int): 縦の分割数
            split_num_width (int): 横の分割数
            thre (int): 閾値

        Return:
            detail_coordi: ブロックの正確な座標([x_max, x_min, y_max, y_min])
        """
        mask_height = int(color_img.shape[0] / split_num_height)
        mask_width = int(color_img.shape[1] / split_num_width)

        is_detected = False
        x_max, x_min, y_max, y_min = None, None, None, None

        # 下
        end = int(coordinate[1]+mask_height*1.5)
        for y in range(coordinate[1], end):  # 上→下
            # 画像からはみ出したら終了
            if y == color_img.shape[0]-1 or y == end-1:
                if is_detected is False:
                    break
                elif y == color_img.shape[0]-1:
                    y_max = color_img.shape[0]-1
                    break
                elif y == end-1:
                    y_max = end-1
                    break
                else:
                    if self.develop:
                        raise ValueError("Error1:色が見つかりません")
                    return None

            # ピクセル数検出
            mask = color_img[y, coordinate[0]:coordinate[0]+mask_width]
            count_pixcel = np.count_nonzero(np.all(mask == block_color, axis=-1))

            # 探している物体(ブロックなど)の領域に突入
            if count_pixcel > thre:
                is_detected = True

            # 色が検知できなくなった時がx座標の最大
            elif count_pixcel < thre and is_detected:
                y_max = y
                is_detected = False

        if y_max is None:
            return None

        # 上
        end = int(coordinate[1]-mask_height*0.75)
        for y in range(y_max, end, -1):  # 下→上
            # 画像からはみ出したら終了
            if y == 0 or y == end + 1:
                if is_detected is False:
                    break
                elif y == 0:
                    y_min = color_img.shape[1] - 1
                    break
                elif y == end + 1:
                    y_min = end + 1
                    break
                else:
                    if self.develop:
                        raise ValueError("Error2:色が見つかりません")
                    return None

            # ピクセル数検出
            mask = color_img[y, coordinate[0]:coordinate[0]+mask_width]
            count_pixcel = np.count_nonzero(np.all(mask == block_color, axis=-1))

            # 探している物体(ブロックなど)の領域に突入
            if count_pixcel > thre:
                is_detected = True

            # 色が検知できなくなった時がx座標の最大
            elif count_pixcel < thre and is_detected:
                y_min = y
                is_detected = False

        if y_min is None:
            return None

        # 右
        end = int(coordinate[0]+mask_width*1.5)
        for x in range(coordinate[0]-mask_width, end):  # 左→右
            # 画像からはみ出したら終了
            if x >= color_img.shape[1]-1 or x == end-1:
                if is_detected is False:
                    break
                elif x >= color_img.shape[1]-1:
                    x_max = color_img.shape[1] - 1
                    break
                elif x == end-1:
                    x_max = end-1
                    break
                else:
                    if self.develop:
                        raise ValueError("Error3:色が見つかりません")
                    return None

            # ピクセル数検出
            mask = color_img[y_min-2:y_max+2, x]
            count_pixcel = np.count_nonzero(np.all(mask == block_color, axis=-1))
            # 探している物体(ブロックなど)の領域に突入
            if count_pixcel > thre:
                is_detected = True
            # 色が検知できなくなった時がx座標の最大
            elif count_pixcel < thre and is_detected:
                x_max = x
                is_detected = False

        if x_max is None:
            return None

        # 左
        end = coordinate[0]-mask_width
        for x in range(x_max, end, -1):  # 右→左
            # 画像からはみ出したら終了
            if x == 0 or x == end + 1:
                if is_detected is False:
                    break
                elif x == 0:
                    x_min = 0
                    break
                elif x == end+1:
                    x_min = end + 1
                    break
                else:
                    if self.develop:
                        raise ValueError("Error4:色が見つかりません")
                    return None

            # ピクセル数検出
            mask = color_img[y_min-2:y_max+2, x]
            count_pixcel = np.count_nonzero(np.all(mask == block_color, axis=-1))

            # 探している物体(ブロックなど)の領域に突入
            if count_pixcel > thre:
                is_detected = True

            # 色が検知できなくなった時がx座標の最大
            elif count_pixcel < thre and is_detected:
                x_min = x
                is_detected = False

        if x_min is None:
            return None

        if self.develop and develop_img is not None:
            cv2.rectangle(develop_img,
                          (x_min, y_min), (x_max, y_max),
                          RgbColor.BLACK.value, 2)

        detail_coordi = np.array([x_max, x_min, y_max, y_min])

        return detail_coordi

    def detect_circle(self,
                      color_5_circle_img,
                      circle_color=RgbColor.BLUE.value,
                      split_num_height=10,
                      split_num_width=10
                      ) -> (np.ndarray):
        """サークルの位置を発見する関数.

        Args:
            color_5_circle_img (np.ndarray): コース画像
            circle_color (list): 探すサークルの色
            split_num_height (int): 画像の縦を何等分するかの値
            split_num_width (int): 画像の横を何等分するかの値
        Return:
            detail_coordi (np.ndarray): サークルの座標
        """
        # サークル座標の作成
        # 大まかな座標を取得
        about_coordi = self.check_coordi_about(
            color_3_img=color_5_circle_img,
            search_color=circle_color,
            split_num_height=split_num_height,
            split_num_width=split_num_width,
            thre=5)

        # 細かな座標を取得
        if about_coordi is not None:
            detail_coordi = self.check_coordi_detail(
                color_img=color_5_circle_img,
                coordinate=about_coordi,
                block_color=circle_color,
                split_num_height=split_num_height,
                split_num_width=split_num_width,
                thre=2)
        else:
            return None

        return detail_coordi

    def check_block_exist(self, circle_coordi, thre_x=100, thre_y=50, develop_img=None) -> int:
        """サークル上にブロックがあるかチェックする関数.

        Args:
            circle_coordi (np.ndarray): サークル座標([x_max, x_min, y_max, y_min])
        Return:
            int: サークル上にあるブロックを示す値
        """
        circle_posi_x = int(circle_coordi[1] + (abs(circle_coordi[0] - circle_coordi[1]))/2)
        circle_posi_y = circle_coordi[2]
        if develop_img is not None:
            develop_img[circle_coordi[3]:circle_coordi[2],
                        circle_posi_x:circle_posi_x+10] = RgbColor.BLACK.value

        # 赤
        if self.block_coordi_red is not None and \
            np.all(self.block_coordi_red != 0) and \
                self.block_count_red <= self.block_max_count_red:
            block_posi_x = (self.block_coordi_red[0] + self.block_coordi_red[1]) / 2
            block_posi_y = self.block_coordi_red[2]
            diff_x = abs(circle_posi_x - block_posi_x)
            diff_y = abs(circle_posi_y - block_posi_y)
            if diff_x < thre_x and diff_y < thre_y:
                self.block_coordi_red = None
                self.block_count_red += 1
                return self.RED_BLOCK_NUM
        # 青1
        if self.block_coordi_blue1 is not None and \
            np.all(self.block_coordi_blue1 != 0) and \
                self.block_count_blue <= self.block_max_count_blue:
            block_posi_x = int((self.block_coordi_blue1[0] + self.block_coordi_blue1[1]) / 2)
            block_posi_y = self.block_coordi_blue1[2]
            diff_x = abs(circle_posi_x - block_posi_x)
            diff_y = abs(circle_posi_y - block_posi_y)

            if diff_x < thre_x and diff_y < thre_y:
                self.block_count_blue += 1
                self.block_coordi_blue1 = None
                return self.BLUE_BLOCK_NUM
        # 青2
        if self.block_coordi_blue2 is not None and \
            np.all(self.block_coordi_blue2 != 0) and \
                self.block_count_blue <= self.block_max_count_blue:
            block_posi_x = int((self.block_coordi_blue2[0] + self.block_coordi_blue2[1]) / 2)
            block_posi_y = self.block_coordi_blue2[2]
            diff_x = abs(circle_posi_x - block_posi_x)
            diff_y = abs(circle_posi_y - block_posi_y)

            if diff_x < thre_x and diff_y < thre_y:
                self.block_coordi_blue2 = None
                self.block_count_blue += 1
                return self.BLUE_BLOCK_NUM

        return 0

    def get_info_row_3(self, front_course_img, second_from_front_img, coordi_circle_img) -> None:
        """ブロックエリアの1行目の情報を取得する関数.

        Args:
            front_course_img (np.ndarray): 3行目のサークルのみ表示したコース画像(検出用)
            second_from_front_img (np.ndarray): 3行目のサークルを削除していくコース画像
            coordi_circle_img (np.ndarray): サークルの場所を検出できているかの確認用画像(開発用)
        """
        # 青サークル1
        # サークル検出
        circle_blue_coordi1 = self.detect_circle(front_course_img, RgbColor.BLUE.value, 10, 10)
        if circle_blue_coordi1 is not None:
            # ブロックの存在確認
            self.course_info_block[3, 1] = self.check_block_exist(circle_blue_coordi1)
            # 座標の追加
            self.course_info_coordinate[3, 1] = circle_blue_coordi1
            # 画像上のサークル削除
            self.change_coordi_color(front_course_img,
                                     circle_blue_coordi1,
                                     padding_value_x=30,
                                     padding_value_y=5)
            self.change_coordi_color(second_from_front_img,
                                     circle_blue_coordi1,
                                     padding_value_x=30,
                                     padding_value_y=5)
            if self.develop:
                cv2.rectangle(coordi_circle_img,
                              (circle_blue_coordi1[1], circle_blue_coordi1[3]),
                              (circle_blue_coordi1[0], circle_blue_coordi1[2]),
                              RgbColor.BLACK.value, 2)
            if self.develop:
                save_path = os.path.join(self.save_dir_path, "front_course_img01.png")
                cv2.imwrite(save_path, front_course_img)

        # 青サークル2
        circle_blue_coordi2 = self.detect_circle(
            front_course_img, RgbColor.BLUE.value, 10, 10)
        if circle_blue_coordi2 is not None:
            # ブロックの存在確認
            self.course_info_block[3, 0] = self.check_block_exist(circle_blue_coordi2)
            # 座標の追加
            self.course_info_coordinate[3, 0] = circle_blue_coordi2
            # 画像上のサークル削除
            mask = self.change_coordi_color(front_course_img,
                                            circle_blue_coordi2,
                                            padding_value_x=30,
                                            padding_value_y=5)
            self.change_coordi_color(second_from_front_img,
                                     circle_blue_coordi2,
                                     padding_value_x=30,
                                     padding_value_y=5)
            if self.develop:
                cv2.rectangle(coordi_circle_img,
                              (circle_blue_coordi2[1], circle_blue_coordi2[3]),
                              (circle_blue_coordi2[0], circle_blue_coordi2[2]),
                              RgbColor.BLACK.value, 2)

            if self.develop:
                save_path = os.path.join(self.save_dir_path, "front_course_img02.png")
                cv2.imwrite(save_path, front_course_img)

        # 座標が逆だった場合(3行目青)
        if circle_blue_coordi1 is not None and circle_blue_coordi2 is not None and \
                circle_blue_coordi2[0] > circle_blue_coordi1[0]:
            self.course_info_block[3, 0], self.course_info_block[3, 1] = \
                self.course_info_block[3, 1], self.course_info_block[3, 0]
            self.course_info_coordinate[3, 0] = circle_blue_coordi1
            self.course_info_coordinate[3, 1] = circle_blue_coordi2

        # 緑サークル1
        circle_green_coordi1 = self.detect_circle(
            front_course_img, RgbColor.GREEN.value, 10, 8)
        if circle_green_coordi1 is not None:
            # ブロックの存在確認
            self.course_info_block[3, 2] = self.check_block_exist(circle_green_coordi1)
            # 座標の追加
            self.course_info_coordinate[3, 2] = circle_green_coordi1
            # 画像上のサークル削除
            front_course_img[
                circle_green_coordi1[3]-5:circle_green_coordi1[2]+5,
                circle_green_coordi1[1]-30:circle_green_coordi1[0]+30] = RgbColor.WHITE.value
            second_from_front_img[
                circle_green_coordi1[3]-5:circle_green_coordi1[2]+5,
                circle_green_coordi1[1]-30:circle_green_coordi1[0]+30] = RgbColor.WHITE.value
            if self.develop:
                cv2.rectangle(coordi_circle_img,
                              (circle_green_coordi1[1], circle_green_coordi1[3]),
                              (circle_green_coordi1[0], circle_green_coordi1[2]),
                              RgbColor.BLACK.value, 2)

        # 緑サークル2
        circle_green_coordi2 = self.detect_circle(
            front_course_img, RgbColor.GREEN.value, 10, 8)
        if circle_green_coordi2 is not None:
            # ブロックの存在確認
            self.course_info_block[3, 3] = self.check_block_exist(circle_green_coordi2)
            # 座標の追加
            self.course_info_coordinate[3, 3] = circle_green_coordi2
            # 画像上のサークル削除
            self.change_coordi_color(second_from_front_img,
                                     circle_green_coordi2,
                                     padding_value_x=30,
                                     padding_value_y=5)

            if self.develop:
                self.change_coordi_color(front_course_img,
                                         circle_green_coordi2,
                                         padding_value_x=30,
                                         padding_value_y=5)
                cv2.rectangle(coordi_circle_img,
                              (circle_green_coordi2[1], circle_green_coordi2[3]),
                              (circle_green_coordi2[0], circle_green_coordi2[2]),
                              RgbColor.BLACK.value, 2)

        # 座標が逆だった場合(3行目緑)
        if circle_green_coordi1 is not None and \
            circle_green_coordi2 is not None and \
                circle_green_coordi1[0] > circle_green_coordi2[0]:
            self.course_info_block[3, 2], self.course_info_block[3, 3] = \
                self.course_info_block[3, 3], self.course_info_block[3, 2]
            self.course_info_coordinate[3, 2] = circle_green_coordi2
            self.course_info_coordinate[3, 3] = circle_green_coordi1

    def get_info_row_2(self, second_are_img, second_detect_circle_img) -> None:
        """2列目のサークル情報を取得する関数.

        Args:
            second_from_front_img (np.ndarray): 3行目を消したコース画像
            second_detect_circle_img (np.ndarray): サークルを消すことができたかの確認用画像(開発用)
        """
        # Second: 青サークル1
        second_circle_blue_coordi1 = \
            self.detect_circle(second_are_img, RgbColor.BLUE.value, 10, 10)  # サークル検出
        if second_circle_blue_coordi1 is not None:
            # ブロックの存在確認
            self.course_info_block[2, 1] = self.check_block_exist(
                second_circle_blue_coordi1, 100, 30)
            # 座標の追加
            self.course_info_coordinate[2, 1] = second_circle_blue_coordi1
            # 画像上のサークル削除
            second_are_img[
                second_circle_blue_coordi1[3]-5:second_circle_blue_coordi1[2]+5,
                second_circle_blue_coordi1[1]-30:second_circle_blue_coordi1[0]+30] \
                = RgbColor.WHITE.value
            # 2つめのサークルを見つけた時点で。上側に青があるのがおかしいので、上側の青を削除
            if self.develop:
                cv2.rectangle(second_detect_circle_img,
                              (second_circle_blue_coordi1[1], second_circle_blue_coordi1[3]),
                              (second_circle_blue_coordi1[0], second_circle_blue_coordi1[2]),
                              RgbColor.BLACK.value, 2)
        else:
            if self.develop:
                print("サークルなし1")

        # Second: 青サークル2
        second_circle_blue_coordi2 = \
            self.detect_circle(second_are_img, RgbColor.BLUE.value, 10, 10)  # サークル検出
        if second_circle_blue_coordi2 is not None:
            # ブロックの存在確認
            self.course_info_block[2, 0] = self.check_block_exist(
                second_circle_blue_coordi2, 100, 30)
            # 座標の追加
            self.course_info_coordinate[2, 0] = second_circle_blue_coordi2
            # 画像上のサークル削除
            second_are_img[second_circle_blue_coordi2[3]-5:second_circle_blue_coordi2[2]+5,
                           second_circle_blue_coordi2[1]-30:second_circle_blue_coordi2[0]+30] \
                = RgbColor.WHITE.value
            if self.develop:
                cv2.rectangle(second_detect_circle_img,
                              (second_circle_blue_coordi2[1], second_circle_blue_coordi2[3]),
                              (second_circle_blue_coordi2[0], second_circle_blue_coordi2[2]),
                              RgbColor.BLACK.value, 2)
        else:
            if self.develop:
                print("サークルなし2")

        # 座標が逆だった場合(2行目青)
        if second_circle_blue_coordi1 is not None and second_circle_blue_coordi2 is not None and \
                second_circle_blue_coordi2[0] > second_circle_blue_coordi1[0]:
            self.course_info_block[2, 0], self.course_info_block[2, 1] = \
                self.course_info_block[2, 1], self.course_info_block[2, 0]
            self.course_info_coordinate[2, 0] = second_circle_blue_coordi1
            self.course_info_coordinate[2, 1] = second_circle_blue_coordi2

        # すべてのブロックを手前の列で見つけた場合、終了
        if self.block_count_red + self.block_count_blue >= \
                self.block_max_count_red + self.block_max_count_blue:
            return

        # 2列目の緑サークルを検出
        # いらない色を削除する範囲の設定
        # Second: 緑サークル1
        second_circle_green_coordi1 = \
            self.detect_circle(second_are_img, RgbColor.GREEN.value, 10, 10)  # サークル検出
        if second_circle_green_coordi1 is not None:
            # ブロックの存在確認
            self.course_info_block[2, 2] = self.check_block_exist(
                second_circle_green_coordi1, 100, 30)
            # 座標の追加
            self.course_info_coordinate[2, 2] = second_circle_green_coordi1
            # 画像上のサークル削除
            second_are_img[second_circle_green_coordi1[3]-5:second_circle_green_coordi1[2]+5,
                           second_circle_green_coordi1[1]-30:second_circle_green_coordi1[0]+30] \
                = RgbColor.WHITE.value
            # 2つめのサークルを見つけた時点で。上側に青があるのがおかしいので、上側の青を削除
            y = second_circle_green_coordi1[2] - 30  # 30は補正値
            if self.develop:
                cv2.rectangle(second_detect_circle_img,
                              (second_circle_green_coordi1[1], second_circle_green_coordi1[3]),
                              (second_circle_green_coordi1[0], second_circle_green_coordi1[2]),
                              RgbColor.BLACK.value, 2)
        else:
            if self.develop:
                print("サークルなし3")

        # Second: 緑サークル2
        second_circle_green_coordi2 = \
            self.detect_circle(second_are_img, RgbColor.GREEN.value, 10, 10)  # サークル検出
        if second_circle_green_coordi2 is not None:
            # ブロックの存在確認
            self.course_info_block[2, 3] = self.check_block_exist(
                second_circle_green_coordi2, 100, 30)
            # 座標の追加
            self.course_info_coordinate[2, 3] = second_circle_green_coordi2
            # 画像上のサークル削除
            second_are_img[second_circle_green_coordi2[3]-5:second_circle_green_coordi2[2]+5,
                           second_circle_green_coordi2[1]-30:second_circle_green_coordi2[0]+30] \
                = RgbColor.WHITE.value
            if self.develop:
                cv2.rectangle(second_detect_circle_img,
                              (second_circle_green_coordi2[1], second_circle_green_coordi2[3]),
                              (second_circle_green_coordi2[0], second_circle_green_coordi2[2]),
                              RgbColor.BLACK.value, 2)
        else:
            if self.develop:
                print("サークルなし4")

        # 座標が逆だった場合(2行目緑)
        if second_circle_green_coordi1 is not None and \
            second_circle_green_coordi2 is not None and \
                second_circle_green_coordi1[0] > second_circle_green_coordi2[0]:
            self.course_info_block[2, 2], self.course_info_block[2, 3] = \
                self.course_info_block[2, 3], self.course_info_block[2, 2]
            self.course_info_coordinate[2, 2] = second_circle_green_coordi2
            self.course_info_coordinate[2, 3] = second_circle_green_coordi1

    def predict_block_locate(self, block_coordi) -> (int, int):
        """ブロックと直線の距離を判別する.

        Args:
            block_coordi (np.ndarray): 調べるブロックの座標

        Return:
            row: ブロックの存在する行
            column: ブロックの存在する列
        """
        # 0行目 or 1行目
        _, diff_y_row_0 = LineCalculator.calc_distance_line_to_coordi(
            block_coordi, self.row_verctor_0)
        _, diff_y_row_1 = LineCalculator.calc_distance_line_to_coordi(
            block_coordi, self.row_verctor_1)

        if (diff_y_row_0 is not None and diff_y_row_1 is None) or \
                diff_y_row_0 <= diff_y_row_1:
            row = 0
        elif (diff_y_row_0 is None and diff_y_row_1 is not None) or \
                diff_y_row_0 > diff_y_row_1:
            row = 1
        else:
            return None, None

        diff_x_arr = np.array([])
        # 列
        # NOTE: noqaにしないとpycodestyleでエラー
        if np.all(self.column_verctor_0 != None):  # noqa
            diff_x_column_0, _ = LineCalculator.calc_distance_line_to_coordi(
                block_coordi, self.column_verctor_0)
            diff_x_arr = np.append(diff_x_arr, diff_x_column_0)
        else:
            diff_x_arr = np.append(diff_x_arr, 2000)  # 画像横サイズより大きい数

        if np.all(self.column_verctor_1 != None):  # noqa
            diff_x_column_1, _ = LineCalculator.calc_distance_line_to_coordi(
                block_coordi, self.column_verctor_1)
            diff_x_arr = np.append(diff_x_arr, diff_x_column_1)
        else:
            diff_x_arr = np.append(diff_x_arr, 2000)  # 画像横サイズより大きい数

        if np.all(self.column_verctor_2 != None):  # noqa
            diff_x_column_2, _ = LineCalculator.calc_distance_line_to_coordi(
                block_coordi, self.column_verctor_2)
            diff_x_arr = np.append(diff_x_arr, diff_x_column_2)
        else:
            diff_x_arr = np.append(diff_x_arr, 2000)  # 画像横サイズより大きい数

        if np.all(self.column_verctor_3 != None):  # noqa
            diff_x_column_3, _ = LineCalculator.calc_distance_line_to_coordi(
                block_coordi, self.column_verctor_3)
            diff_x_arr = np.append(diff_x_arr, diff_x_column_3)
        else:
            diff_x_arr = np.append(diff_x_arr, 2000)  # 画像横サイズより大きい数

        min_index = np.argmin(diff_x_arr)
        # 直線が一部見つけれなかったときのフィルター
        if diff_x_arr[min_index] > 100:
            if np.all(self.column_verctor_0 == None) and \
                    block_coordi[0] < (block_coordi[1] - self.column_verctor_1[1]) / self.column_verctor_1[0]:
                return row, 0
            if np.all(self.column_verctor_3 == None) and \
                    block_coordi[0] > (block_coordi[1] - self.column_verctor_2[1]) / self.column_verctor_2[0]:
                return row, 3
            return row, None
        else:
            return row, min_index  # min_index(=column)

    def get_area_info(self, isL=True) -> None:
        """画像からコース情報を取得する関数.

        Args:
            game_area_img (np.ndarray): ゲームエリア画像
            save_path (str): 出力画像ファイルの保存パス

        Return:
            np.ndarray: コース情報 (Noneは取得失敗)
        """
        # 開発用変数
        img_count = 0

        # 下準備
        image_path = os.path.join(self.image_dir_path, self.image_name)
        course_img = cv2.imread(image_path)

        if course_img is None:
            if self.develop:
                print("[Error] 画像読み込み失敗")
            return self.course_info_block

        if self.develop:
            img_count += 1
            save_path = os.path.join(self.save_dir_path, str(img_count)+"_"+self.image_name)
            cv2.imwrite(save_path, course_img)

        # Rコースの場合
        if not isL:
            # 画像を左右反転させる
            course_img = cv2.flip(course_img, 1)

        # 画像上のいらない領域を削除
        processed_course_img, train_area_img = self.pre_process_img(course_img)

        if processed_course_img is None:
            if self.develop:
                print("[Error] 緑のエリアの線分を取得失敗")
            return self.course_info_block

        # BGR色空間からHSV色空間への変換
        game_area_img = cv2.cvtColor(processed_course_img, cv2.COLOR_BGR2HSV)

        # 3色画像の作成(赤、青、白)
        color_3_img = np.full_like(game_area_img, RgbColor.WHITE.value, dtype=np.uint8)
        self.change_color(game_area_img, color_3_img, self.RED1, RgbColor.RED.value)
        self.change_color(game_area_img, color_3_img, self.RED2, RgbColor.RED.value)
        self.change_color(game_area_img, color_3_img, self.BLUE, RgbColor.BLUE.value)

        # 5色画像の作成（黒以外）
        color_5_img = np.full_like(game_area_img, RgbColor.WHITE.value, dtype=np.uint8)
        self.change_color(game_area_img, color_5_img, self.RED1, RgbColor.RED.value)
        self.change_color(game_area_img, color_5_img, self.RED2, RgbColor.RED.value)
        self.change_color(game_area_img, color_5_img, self.YELLOW, RgbColor.YELLOW.value)
        self.change_color(game_area_img, color_5_img, self.GREEN, RgbColor.GREEN.value)
        self.change_color(game_area_img, color_5_img, self.BLUE, RgbColor.BLUE.value)

        # 画像の保存
        if self.develop:
            img_count += 1
            save_path = os.path.join(self.save_dir_path,
                                     str(img_count)+"_train_area_img.png")
            cv2.imwrite(save_path, train_area_img)
            img_count += 1
            save_path = os.path.join(self.save_dir_path,
                                     str(img_count)+"_course.png")
            cv2.imwrite(save_path, course_img)
            img_count += 1
            save_path = os.path.join(self.save_dir_path,
                                     str(img_count)+"_pre_processed_img.png")
            cv2.imwrite(save_path, processed_course_img)
            img_count += 1
            save_path = os.path.join(self.save_dir_path,
                                     str(img_count)+"_color_3_img.png")
            cv2.imwrite(save_path, color_3_img)
            img_count += 1
            save_path = os.path.join(self.save_dir_path,
                                     str(img_count)+"_color_5_img.png")
            cv2.imwrite(save_path, color_5_img)

        """
        ブロックの位置を特定する
        """
        # ブロック特定用の画像を生成
        color_2_under_delete_img = color_3_img.copy()
        # 下側削除
        for x in range(color_2_under_delete_img.shape[1]):
            y = int((self.basis_vector[1] / self.basis_vector[0]) * x * 0.5 + 200)
            color_2_under_delete_img[y:y+30, x] = RgbColor.WHITE.value
        if self.develop:
            img_count += 1
            save_path = os.path.join(self.save_dir_path,
                                     str(img_count)+"_color_2_under_delete.png")
            cv2.imwrite(save_path, color_2_under_delete_img)

        color_5_circle_img = color_5_img.copy()

        # 赤ブロック
        # 大まかな座標を取得
        block_location_about_img = color_2_under_delete_img.copy()
        about_coordi = self.check_coordi_about(
            color_2_under_delete_img, RgbColor.RED.value, 4, 30, 10, block_location_about_img)

        # 細かな座標を取得 ([x_max, x_min, y_max, y_min])
        block_location_img = color_3_img.copy()
        self.block_coordi_red = self.check_coordi_detail(
            color_3_img, about_coordi, RgbColor.RED.value, thre=10, develop_img=block_location_img)
        if self.block_coordi_red is not None:
            color_5_circle_img[
                self.block_coordi_red[3]:self.block_coordi_red[2],
                self.block_coordi_red[1]:self.block_coordi_red[0]] = RgbColor.WHITE.value
        else:
            self.block_max_count_red -= 1
            if self.develop:
                print("赤ブロックが見つかりません")

        # 青1ブロック
        # 大まかな座標を取得
        about_coordi = self.check_coordi_about(
            color_2_under_delete_img,
            RgbColor.BLUE.value,
            4, 30, 50,
            block_location_about_img)
        # 細かな座標を取得
        self.block_coordi_blue1 = self.check_coordi_detail(
            color_3_img, about_coordi, RgbColor.BLUE.value, thre=20, develop_img=block_location_img)
        if self.block_coordi_blue1 is not None:
            color_5_circle_img[
                self.block_coordi_blue1[3]:self.block_coordi_blue1[2],
                self.block_coordi_blue1[1]:self.block_coordi_blue1[0]] = RgbColor.WHITE.value
            color_2_under_delete_img[
                self.block_coordi_blue1[3]:self.block_coordi_blue1[2],
                self.block_coordi_blue1[1]:self.block_coordi_blue1[0]] = RgbColor.WHITE.value
        else:
            self.block_max_count_blue -= 1
            if self.develop:
                print("青ブロックが見つかりません")

        # 青2ブロック
        # 大まかな座標を取得
        about_coordi = self.check_coordi_about(
            color_2_under_delete_img, RgbColor.BLUE.value,
            4, 30, 50, block_location_about_img)
        # 細かな座標を取得
        self.block_coordi_blue2 = self.check_coordi_detail(
            color_3_img, about_coordi, RgbColor.BLUE.value, thre=20, develop_img=block_location_img)
        if self.block_coordi_blue2 is not None:
            color_5_circle_img[
                self.block_coordi_blue2[3]:self.block_coordi_blue2[2],
                self.block_coordi_blue2[1]:self.block_coordi_blue2[0]] = RgbColor.WHITE.value
        else:
            self.block_max_count_blue -= 1
            if self.develop:
                print("青ブロックが見つかりません")
        if self.develop:
            img_count += 1
            save_path = os.path.join(self.save_dir_path, str(img_count)+"_block_location_about.png")
            cv2.imwrite(save_path, block_location_about_img)
            img_count += 1
            save_path = os.path.join(self.save_dir_path, str(img_count)+"_block_location.png")
            cv2.imwrite(save_path, block_location_img)
            img_count += 1
            save_path = os.path.join(self.save_dir_path, str(img_count)+"_circle_only.png")
            cv2.imwrite(save_path, color_5_circle_img)

        """
        1列目のサークルを見つけていく
        """
        # 2列目のサークルのコース画像を作成していく
        front_course_img = color_5_circle_img.copy()
        coordi_circle_img = color_5_circle_img.copy()
        second_from_front_img = color_5_circle_img.copy()
        # 手前のサークルの領域だけの画像を作成
        for x in range(front_course_img.shape[1]):
            y = (self.basis_vector[1] / self.basis_vector[0] * x * 0.6 + 175)
            front_course_img[:int(y), x] = RgbColor.WHITE.value

        # 1列目のサークル情報を取得する
        self.get_info_row_3(front_course_img, second_from_front_img, coordi_circle_img)

        if self.develop:
            img_count += 1
            save_path = os.path.join(self.save_dir_path, str(img_count)+"_coordi_circle.png")
            cv2.imwrite(save_path, coordi_circle_img)
            img_count += 1
            save_path = os.path.join(self.save_dir_path, str(img_count)+"_second_from_front.png")
            cv2.imwrite(save_path, second_from_front_img)

        # 使わない変数(画像)を削除
        del front_course_img, coordi_circle_img

        # すべてのブロックを3行目で見つけた場合、終了
        if self.block_count_red + self.block_count_blue >= \
                self.block_max_count_red + self.block_max_count_blue:
            if self.develop:
                print("ブロックすべて発見1")
            return self.course_info_block

        """
        2列目のサークルを見つけていく
        """
        # 2列目のサークル検出のための画像作成(いらない場所を白で埋めて、誤検出のリスクを減らす)
        second_are_img = second_from_front_img.copy()

        # いらない色を削除する範囲の設定
        # コース0,1列以外の範囲
        range_1_left = int(self.course_info_coordinate[3, 0, 1])  # 左青のx_min
        range_1_right = int(self.course_info_coordinate[3, 2, 1])  # 左緑のx_min
        # 色条件
        condition1_1 = np.all(second_are_img[:, range_1_right:]
                              == RgbColor.BLUE.value, axis=-1, keepdims=True)
        condition1_2 = np.all(second_are_img[:, :range_1_left]
                              == RgbColor.BLUE.value, axis=-1, keepdims=True)
        condition2_1 = np.all(second_are_img[:, range_1_right:]
                              == RgbColor.RED.value, axis=-1, keepdims=True)
        condition2_2 = np.all(second_are_img[:, :range_1_left]
                              == RgbColor.RED.value, axis=-1, keepdims=True)
        # 青と赤の削除(白に変換)
        if range_1_left != 0:
            second_are_img[:, :range_1_left] = \
                np.where(condition1_2 | condition2_2, RgbColor.WHITE.value,
                         second_are_img[:, :range_1_left])
        if range_1_right != 0:
            second_are_img[:, range_1_right:] = \
                np.where(condition1_1 | condition2_1, RgbColor.WHITE.value,
                         second_are_img[:, range_1_right:])

        # コース3,4列以外の範囲
        range_2_left = int(self.course_info_coordinate[3, 1, 0])  # 右青のx_maxより左側
        range_2_right = int(self.course_info_coordinate[3, 3, 0])  # 右緑のx_maxより右側
        # 色条件
        condition3_1 = np.all(second_are_img[:, range_2_right:]
                              == RgbColor.GREEN.value, axis=-1, keepdims=True)
        condition3_2 = np.all(second_are_img[:, :range_2_left]
                              == RgbColor.GREEN.value, axis=-1, keepdims=True)
        condition4_1 = np.all(second_are_img[:, range_2_right:]
                              == RgbColor.YELLOW.value, axis=-1, keepdims=True)
        condition4_2 = np.all(second_are_img[:, :range_2_left]
                              == RgbColor.YELLOW.value, axis=-1, keepdims=True)
        # 緑と黄の削除(白に変換)
        if range_2_left != 0:
            second_are_img[:, :range_2_left] = \
                np.where(condition3_2 | condition4_2, RgbColor.WHITE.value,
                         second_are_img[:, :range_2_left])
        if range_2_right != 0:
            second_are_img[:, range_2_right:] = \
                np.where(condition3_1 | condition4_1, RgbColor.WHITE.value,
                         second_are_img[:, range_2_right:])

        # 上側
        y_list = [y for y in self.course_info_coordinate[3, :, 3] if y > 0]
        if len(y_list) > 0:
            range_upper = int(min(y_list)) - 60
            condition5 = np.all(second_are_img[:range_upper, :] ==
                                RgbColor.BLUE.value, axis=-1, keepdims=True)
            condition6 = np.all(second_are_img[:range_upper, :] ==
                                RgbColor.GREEN.value, axis=-1, keepdims=True)
            second_are_img[:range_upper, :] = \
                np.where(condition5, RgbColor.WHITE.value, second_are_img[:range_upper, :])
            second_are_img[:range_upper, :] = \
                np.where(condition6, RgbColor.WHITE.value, second_are_img[:range_upper, :])

        # 真ん中のロゴを消す
        center_range_left = int(self.course_info_coordinate[3, 1, 0])  # 右青x_max
        center_range_right = int(self.course_info_coordinate[3, 2, 1])  # 左緑x_min
        if not (center_range_left == 0 or center_range_right == 0):
            condition7 = np.all(
                second_are_img[:, center_range_left:center_range_right] == RgbColor.GREEN.value,
                axis=-1,
                keepdims=True)
            second_are_img[:, center_range_left:center_range_right] = \
                np.where(condition7,
                         RgbColor.WHITE.value,
                         second_are_img[:, center_range_left:center_range_right])

        if self.develop:
            img_count += 1
            save_path = os.path.join(self.save_dir_path, str(img_count)+"_second_are.png")
            cv2.imwrite(save_path, second_are_img)

        # 2列目のサークル情報を取得する
        second_detect_circle_img = color_5_circle_img.copy()
        self.get_info_row_2(second_are_img, second_detect_circle_img)

        if self.develop:
            img_count += 1
            save_path = os.path.join(self.save_dir_path, str(img_count)+"_second_detect_circle.png")
            cv2.imwrite(save_path, second_detect_circle_img)
            img_count += 1
            save_path = os.path.join(self.save_dir_path, str(img_count)+"_second_delete_circle.png")
            cv2.imwrite(save_path, second_are_img)

        del second_detect_circle_img

        # すべてのブロックを2行目で見つけた場合、終了
        if self.block_count_red + self.block_count_blue >= \
                self.block_max_count_red + self.block_max_count_blue:
            if self.develop:
                print("ブロックすべて発見2")
            return self.course_info_block

        # 3列目以降の情報を取得していく
        back_area_img = color_5_circle_img.copy()

        # コース画像の青と緑を消す
        condition1 = np.all(back_area_img == RgbColor.BLUE.value, axis=-1, keepdims=True)
        condition2 = np.all(back_area_img == RgbColor.GREEN.value, axis=-1, keepdims=True)
        back_area_img = np.where(condition1, RgbColor.WHITE.value, back_area_img)
        back_area_img = np.where(condition2, RgbColor.WHITE.value, back_area_img)

        # コース画像の赤と黄を消す
        # 赤
        range_1_left = int(self.course_info_coordinate[2, 0, 1])  # 左青のx_min
        range_1_right = int(self.course_info_coordinate[2, 2, 1])  # 左緑のx_min
        condition1_1 = np.all(back_area_img[:, :range_1_left]
                              == RgbColor.RED.value, axis=-1, keepdims=True)
        condition1_2 = np.all(back_area_img[:, range_1_right:]
                              == RgbColor.RED.value, axis=-1, keepdims=True)
        if range_1_left != 0:
            back_area_img[:, :range_1_left] = \
                np.where(condition1_1, RgbColor.WHITE.value,
                         back_area_img[:, :range_1_left])
        if range_1_right != 0:
            back_area_img[:, range_1_right:] = \
                np.where(condition1_2, RgbColor.WHITE.value,
                         back_area_img[:, range_1_right:])

        # 黄
        range_2_left = int(self.course_info_coordinate[2, 1, 0])  # 右青のx_max
        range_2_right = int(self.course_info_coordinate[2, 3, 0])  # 右緑のx_max
        condition2_1 = np.all(back_area_img[:, :range_2_left]
                              == RgbColor.YELLOW.value, axis=-1, keepdims=True)
        condition2_2 = np.all(back_area_img[:, range_2_right:]
                              == RgbColor.YELLOW.value, axis=-1, keepdims=True)
        if range_2_left != 0:
            back_area_img[:, :range_2_left] = \
                np.where(condition2_1, RgbColor.WHITE.value,
                         back_area_img[:, :range_2_left])
        if range_2_right != 0:
            back_area_img[:, range_2_right:] = \
                np.where(condition2_2, RgbColor.WHITE.value,
                         back_area_img[:, range_2_right:])

        if self.develop:
            img_count += 1
            save_path = os.path.join(self.save_dir_path, str(img_count)+"_back_area.png")
            cv2.imwrite(save_path, back_area_img)

        # 2列目のサークルの平均y座標を求める
        row_area_img = color_5_img.copy()

        """
        行の直線を求める
        """
        # 3行目(手前)のサークルの直線の式を求める
        x = (self.course_info_coordinate[3, :, 0] +
             self.course_info_coordinate[3, :, 1])/2
        y = (self.course_info_coordinate[3, :, 2] +
             self.course_info_coordinate[3, :, 3])/2
        # 傾きと切片を求める(a:傾き, b:切片)
        self.row_verctor_3[0], self.row_verctor_3[1] = \
            LineCalculator.calc_from_coordi_4(
                x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3])
        # 線を記述する
        if self.develop:
            y1 = int(self.row_verctor_3[1])
            y2 = int(self.row_verctor_3[0]*row_area_img.shape[1]+self.row_verctor_3[1])
            cv2.line(row_area_img,
                     pt1=(0, y1),  # 始点
                     pt2=(row_area_img.shape[1], y2),  # 終点
                     color=RgbColor.BLACK.value,
                     thickness=2)

        # 2行目のサークルの直線の式を求める
        x = (self.course_info_coordinate[2, :, 0]
             + self.course_info_coordinate[2, :, 1])/2
        y = (self.course_info_coordinate[2, :, 2]
             + self.course_info_coordinate[2, :, 3])/2
        # 傾きと切片を求める(a:傾き, b:切片)
        self.row_verctor_2[0], self.row_verctor_2[1] \
            = LineCalculator.calc_from_coordi_4(
                x[0], y[0], x[1], y[1], x[2], y[2], x[3], y[3])
        # 線を記述する
        if self.develop:
            y1 = int(self.row_verctor_2[1])
            y2 = int(self.row_verctor_2[0]*row_area_img.shape[1]+self.row_verctor_2[1])
            cv2.line(row_area_img,
                     pt1=(0, y1),
                     pt2=(row_area_img.shape[1], y2),
                     color=RgbColor.BLACK.value,
                     thickness=2)

        # 1行目のサークルの直線の式を予想する
        self.row_verctor_1[0] = \
            ((self.row_verctor_3[0] + self.row_verctor_2[0]) / 2) * 0.9  # 0.9は補正値
        self.row_verctor_1[1] = self.row_verctor_2[1] - \
            ((self.row_verctor_3[1] - self.row_verctor_2[1]) * 0.7)  # 0.5は補正値
        # 線を記述する
        if self.develop:
            y1 = int(self.row_verctor_1[1])
            y2 = int(self.row_verctor_1[0]*row_area_img.shape[1]+self.row_verctor_1[1])
            cv2.line(row_area_img,
                     pt1=(0, y1),
                     pt2=(row_area_img.shape[1], y2),
                     color=RgbColor.BLUE.value,
                     thickness=2)

        # 0行目のサークルの直線の式を予想する(横)
        self.row_verctor_0[0] = self.row_verctor_1[0] * 0.9  # 0.9は補正値
        self.row_verctor_0[1] = self.row_verctor_2[1] - \
            ((self.row_verctor_3[1] - self.row_verctor_2[1]) * 0.95)  # 0.7は補正値
        # 線を記述する
        if self.develop:
            y1 = int(self.row_verctor_0[1])
            y2 = int(self.row_verctor_0[0]*row_area_img.shape[1]+self.row_verctor_0[1])
            cv2.line(row_area_img,
                     pt1=(0, y1),
                     pt2=(row_area_img.shape[1], y2),
                     color=RgbColor.BLUE.value,
                     thickness=2)

        """
        列の直線を求める
        """
        # 0列目(左)のサークルの直線の式を算出する
        if np.any(self.course_info_coordinate[3, 0] != 0) and \
                np.any(self.course_info_coordinate[2, 0] != 0):
            x1 = np.average(self.course_info_coordinate[3, 0, [0, 1]])
            y1 = np.average(self.course_info_coordinate[3, 0, [2, 3]])
            x2 = np.average(self.course_info_coordinate[2, 0, [0, 1]])
            y2 = np.average(self.course_info_coordinate[2, 0, [2, 3]])

            self.column_verctor_0[0], self.column_verctor_0[1] = \
                LineCalculator.calc_from_coordi_2(x1, y1, x2, y2)
            # 線を記述する
            if self.develop and np.all(self.column_verctor_0 != None):
                y1 = int(self.column_verctor_0[1])
                y2 = int(self.column_verctor_0[0]*row_area_img.shape[1]+self.column_verctor_0[1])
                cv2.line(row_area_img,
                         pt1=(0, y1),
                         pt2=(row_area_img.shape[1], y2),
                         color=RgbColor.RED.value,
                         thickness=2)

        # 1列目(左)のサークルの直線の式を算出する
        if np.any(self.course_info_coordinate[3, 1] != 0) and \
                np.any(self.course_info_coordinate[2, 1] != 0):
            x1 = np.average(self.course_info_coordinate[3, 1, [0, 1]])
            y1 = np.average(self.course_info_coordinate[3, 1, [2, 3]])
            x2 = np.average(self.course_info_coordinate[2, 1, [0, 1]])
            y2 = np.average(self.course_info_coordinate[2, 1, [2, 3]])
            self.column_verctor_1[0], self.column_verctor_1[1] = \
                LineCalculator.calc_from_coordi_2(x1, y1, x2, y2)
            # 線を記述する
            if self.develop and np.all(self.column_verctor_1 != None):
                y1 = int(self.column_verctor_1[1])
                y2 = int(self.column_verctor_1[0]*row_area_img.shape[1]+self.column_verctor_1[1])
                cv2.line(row_area_img,
                         pt1=(0, y1),
                         pt2=(row_area_img.shape[1], y2),
                         color=RgbColor.RED.value,
                         thickness=2)

        # 2列目(左)のサークルの直線の式を算出する
        if np.any(self.course_info_coordinate[3, 2] != 0) and \
                np.any(self.course_info_coordinate[2, 2] != 0):
            x1 = np.average(self.course_info_coordinate[3, 2, [0, 1]])
            y1 = np.average(self.course_info_coordinate[3, 2, [2, 3]])
            x2 = np.average(self.course_info_coordinate[2, 2, [0, 1]])
            y2 = np.average(self.course_info_coordinate[2, 2, [2, 3]])
            self.column_verctor_2[0], self.column_verctor_2[1] = \
                LineCalculator.calc_from_coordi_2(x1, y1, x2, y2)
            # 線を記述する
            if self.develop and np.all(self.column_verctor_2 != None):
                y1 = int(self.column_verctor_2[1])
                y2 = int(self.column_verctor_2[0]*row_area_img.shape[1]+self.column_verctor_2[1])
                cv2.line(row_area_img,
                         pt1=(0, y1),
                         pt2=(row_area_img.shape[1], y2),
                         color=RgbColor.RED.value,
                         thickness=2)

        # 3列目(左)のサークルの直線の式を算出する
        if np.any(self.course_info_coordinate[3, 3] != 0) and \
                np.any(self.course_info_coordinate[2, 3] != 0):
            x1 = np.average(self.course_info_coordinate[3, 3, [0, 1]])
            y1 = np.average(self.course_info_coordinate[3, 3, [2, 3]])
            x2 = np.average(self.course_info_coordinate[2, 3, [0, 1]])
            y2 = np.average(self.course_info_coordinate[2, 3, [2, 3]])
            self.column_verctor_3[0], self.column_verctor_3[1] = \
                LineCalculator.calc_from_coordi_2(x1, y1, x2, y2)
            # 線を記述する
            if self.develop and np.all(self.column_verctor_3 != None):
                y1 = int(self.column_verctor_3[1])
                y2 = int(self.column_verctor_3[0]*row_area_img.shape[1]+self.column_verctor_3[1])
                cv2.line(row_area_img,
                         pt1=(0, y1),
                         pt2=(row_area_img.shape[1], y2),
                         color=RgbColor.RED.value,
                         thickness=2)
                img_count += 1
                save_path = os.path.join(self.save_dir_path, str(img_count)+"_row_area.png")
                cv2.imwrite(save_path, row_area_img)

        """
        見つかっていないブロックの座標を確認していく(0,1行目)
        """
        """赤ブロックが見つかっていない場合"""
        if self.block_coordi_red is not None and self.block_count_red < self.block_max_count_red:
            red_x = np.average(self.block_coordi_red[:2])
            red_y = self.block_coordi_red[2]
            row, column = self.predict_block_locate(np.array([red_x, red_y]))
            if row is None or column is None:
                pass
            else:
                if self.course_info_block[row, column] == 0:
                    self.course_info_block[row, column] = 1
                    self.block_count_red += 1
                if self.develop:
                    cv2.line(row_area_img,
                             pt1=(int(red_x), 0),  # 始点
                             pt2=(int(red_x), row_area_img.shape[0]),  # 終点
                             color=RgbColor.BLUE.value,
                             thickness=2)
                    cv2.line(row_area_img,
                             pt1=(0, int(red_y)),  # 始点
                             pt2=(row_area_img.shape[1], int(red_y)),  # 終点
                             color=RgbColor.BLUE.value,
                             thickness=2)

                # 赤ブロックが最後のブロックだった場合、終了
                if self.block_count_red + self.block_count_blue >= \
                    self.block_max_count_red + self.block_max_count_blue or \
                        (np.all(self.block_coordi_blue1 == 0) and
                         np.all(self.block_coordi_blue2 == 0)):
                    if self.develop:
                        print("ブロックすべて発見3")
                    return self.course_info_block

        """青ブロックが見つかっていない場合"""
        # """
        # ブロック座標算出
        if self.block_coordi_blue1 is not None:
            blue1_x = int(np.average(self.block_coordi_blue1[:2]))
            blue1_y = int(self.block_coordi_blue1[2])
            if self.develop:
                cv2.line(row_area_img,
                         pt1=(blue1_x, 0),  # 始点
                         pt2=(blue1_x, row_area_img.shape[0]),  # 終点
                         color=RgbColor.BLUE.value,
                         thickness=2)
                cv2.line(row_area_img,
                         pt1=(0, blue1_y),  # 始点
                         pt2=(row_area_img.shape[1], blue1_y),  # 終点
                         color=RgbColor.BLUE.value,
                         thickness=2)

        else:
            blue1_x, blue1_y = None, 1000  # yは画像縦サイズより大きい値

        if self.block_coordi_blue2 is not None:
            blue2_x = int(np.average(self.block_coordi_blue2[:2]))
            blue2_y = int(self.block_coordi_blue2[2])
            if self.develop:
                cv2.line(row_area_img,
                         pt1=(blue2_x, 0),  # 始点
                         pt2=(blue2_x, row_area_img.shape[0]),  # 終点
                         color=RgbColor.BLUE.value)
                cv2.line(row_area_img,
                         pt1=(0, blue2_y),  # 始点
                         pt2=(row_area_img.shape[1], blue2_y),  # 終点
                         color=RgbColor.BLUE.value)
        else:
            blue2_x, blue2_y = None, 1000  # yは画像縦サイズより大きい値

        # 片方は見つかっている場合、y座標が小さい方(コース奥側)のブロックの場所を特定する
        arr = np.array([[blue1_x, blue1_y], [blue2_x, blue2_y]])
        sorted_arr = arr[arr[:, 1].argsort()]  # yの小さい順にソート

        for coordi in sorted_arr:
            # ブロックが片方しか見つからない or ブロックがすべて見つかった場合
            if coordi[0] is None or self.block_count_blue >= self.block_max_count_blue:
                break

            row, column = self.predict_block_locate(coordi)
            if row is None or column is None:
                continue

            # 手前のサークルが画像上になくて、サークルのベクトルを求めることができなかった場合を考慮
            elif column is None:
                index = np.where(np.all(self.course_info_coordinate[3] == [0, 0, 0, 0], axis=1))
                # 要素が見つかった場合
                if len(index[0]) > 0:
                    column = index[0][0]
                else:
                    break

            if self.course_info_block[row, column] == 0:
                self.course_info_block[row, column] = 2
                self.block_count_blue += 1

        # 終了
        return self.course_info_block


if __name__ == "__main__":
    # 処理時間計測用
    import time
    import argparse
    start = time.time()

    parser = argparse.ArgumentParser(description="リアカメラに関するプログラム")
    parser.add_argument("-isR", "--isL", default=True, type=bool,
                        help='コースが右(True)か左か(False)')
    parser.add_argument("-develop", "--develop", action='store_true',
                        help='開発モード')
    args = parser.parse_args()

    work_dir_path = os.path.join(PROJECT_DIR_PATH, "tests", "test_data", "block_area_img")
    save_dir_path = os.path.join(PROJECT_DIR_PATH, "work_image_data")

    # """作業用
    # NOTE:画像番号が一桁は"_"をつける
    image_number = "_1"
    image_number = "17"
    # image_number = "22"
    # image_number = "_7"
    # image_number = "23"

    test_images = os.listdir(work_dir_path)
    for img in test_images:
        if img[-6:-4] == image_number:
            image_name = img
    # """

    """走行体用
    work_dir_path = os.path.join(PROJECT_DIR_PATH, "image_data")
    save_dir_path = os.path.join(PROJECT_DIR_PATH, "image_data")
    image_name = "BlockDeTreasure.png"
    # """
    info = GetAreaInfo(image_name, work_dir_path, save_dir_path, develop=args.develop)
    try:
        info.get_area_info(args.isL)
    finally:
        print(f"最終結果--------------------------------------------------------------")
        print(f"image_name         : {image_name}")
        print(f"block_coordi_red   : {info.block_coordi_red}")
        print(f"block_coordi_blue1 : {info.block_coordi_blue1}")
        print(f"block_coordi_blue2 : {info.block_coordi_blue2}")
        print(f"block_count_red    : {info.block_count_red}")
        print(f"block_count_blue   : {info.block_count_blue}")
        print(f"column_verctor_0   : {info.column_verctor_0}")
        print(f"column_verctor_1   : {info.column_verctor_1}")
        print(f"column_verctor_2   : {info.column_verctor_2}")
        print(f"column_verctor_3   : {info.column_verctor_3}")
        print(f"[[x_max, x_min, y_max, y_min]]")
        print(f"course_info_coordinate :\n{info.course_info_coordinate[2:]}")
        print(f"course_info_block :\n{info.course_info_block}")
        print(f"---------------------------------------------------------------------")

    # 処理時間計測用
    execute_time = time.time() - start

    print(f"実行時間: {str(execute_time)[:5]}s")
    print("完了！")
