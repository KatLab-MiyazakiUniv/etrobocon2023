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
    """色クラス.
    色はBGR
    """
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
    YELLOW = [(16, 30), (50, 255), (150, 255)]
    GREEN = [(31, 100), (60, 255), (40, 255)]
    BLUE = [(101, 150), (110, 255), (70, 255)]
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
        self.processed_img_shape = None

        # 領域閾値
        self.block_area_height = 240

        # ベクトル関連
        # self.basis_vector = None
        self.basis_vector_b = None
        self.green_area_b = None
        self.angle_threshold = 4  # 同じ角度の線だと判断する閾値

        self.red_block_num = 1
        self.blue_block_num = 2
        self.block_count_red = 0
        self.block_count_blue = 0

        # ブロックの位置特定関連
        self.mask_width = None
        self.mask_height = None
        # 大まかな座標[x, y]
        self.block_coordi_red = np.array([0, 0])
        self.block_coordi_blue = np.array([[0, 0], [0, 0]])

        # こまかな座標 [x_min, y_max, y_min,y_max]
        self.block_coordi_red_ditail = np.array([0, 0, 0, 0])
        self.block_coordi_blue_ditail = np.array([[0, 0, 0, 0], [0, 0, 0, 0]])

        # 手前のサークル
        self.course_info_block = np.zeros(16).reshape(4, 4)
        # self.course_info_coordi = np.zeros(4*4*2).reshape(4, 4, 2)
        self.first_column_coordinate = np.array([None, None, None, None])
        # 左上のxyを格納
        self.course_info_coordinate = np.full((4, 4), None)

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

    def angle_between_vectors(self, vector_a, vector_b):
        """ベクトル間のなす角を求める関数.

        ベクトルAとベクトルBのなす角を求める.

        Args:
            vector_a: ベクトルA
            vector_b: ベクトルB
        Return:
            theta_deg: なす角
        """
        dot_product = np.dot(vector_a, vector_b)
        magnitude_a = np.linalg.norm(vector_a)
        magnitude_b = np.linalg.norm(vector_b)

        cos_theta = dot_product / (magnitude_a * magnitude_b)

        # acosはarccosine関数で、cosの逆関数です。
        theta_rad = np.arccos(np.clip(cos_theta, -1.0, 1.0))

        # 弧度法から度数法に変換
        angle = np.degrees(theta_rad)

        return angle

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

        ## コース下の緑(以下)の部分削除 & コース上側を削除 ##
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
            # トリミング
            upper_delete_line = int(min(self.green_area_b - self.block_area_height,
                                        a * course_img.shape[1] + self.green_area_b - self.block_area_height))
            lower_delete_line = int(
                max(self.green_area_b, a * course_img.shape[1] + self.green_area_b))
            course_img = course_img[upper_delete_line:lower_delete_line, :, :]

        ## IoT列車のコースを含めた上側削除 ##
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
            self.change_color(game_area_img2, course_img, self.RED2, Color.WHITE.value)
            self.change_color(game_area_img2, course_img, self.YELLOW, Color.WHITE.value)
            self.change_color(game_area_img2, course_img, self.GREEN, Color.WHITE.value)
            self.change_color(game_area_img2, course_img, self.BLUE, Color.WHITE.value)
            self.change_color(game_area_img2, course_img, self.BLACK, Color.WHITE.value)

        if self.develop:
            save_path = os.path.join(self.save_dir_path, "pre_processed_img.png")
            cv2.imwrite(save_path, course_img)

        return course_img

    def check_coordi_ditail(self,
                            color_2_img,
                            color_5_circle_img,
                            coodinate,
                            color=Color.RED.value):
        """より細かなブロックの位置を特定."""
        detect_flag = 0
        x_max, x_min, y_max, y_min = None, None, None, None
        develop_img = color_2_img.copy()

        try:
            for y in range(coodinate[2], color_2_img.shape[0]):
                develop_img[coodinate[2]:y, coodinate[1]:coodinate[1]+self.mask_width] = Color.BLACK.value
                mask = color_2_img[y, coodinate[1]:coodinate[1]+self.mask_width]
                count_pixcel_red = np.count_nonzero(np.all(mask == color, axis=-1))
                if count_pixcel_red <= int(self.mask_width*0.3) and detect_flag == 1:
                    y_max = y
                    detect_flag = 0
                    break
                elif count_pixcel_red > int(self.mask_width*0.3):
                    detect_flag = 1
                if y == color_2_img.shape[0]-1:
                    if detect_flag == 1:
                        y_max = color_2_img.shape[0]-1
                    else:
                        raise ValueError("ブロック(色)が見つかりません")

            for y in range(y_max, 0, -1):
                mask = color_2_img[y, coodinate[1]:coodinate[1]+self.mask_width]
                count_pixcel_red = np.count_nonzero(np.all(mask == color, axis=-1))
                if count_pixcel_red <= int(self.mask_width*0.3) and detect_flag == 1:
                    y_min = y
                    detect_flag = 0
                    break
                elif count_pixcel_red > int(self.mask_width*0.3):
                    detect_flag = 1
                if y == 1:
                    if detect_flag == 1:
                        y_min = 1
                    else:
                        raise ValueError("ブロック(色)が見つかりません")

            for x in range(coodinate[1]-self.mask_width, color_2_img.shape[1]):
                if x >= color_2_img.shape[1]:
                    x_max = color_2_img.shape[1]
                    break
                mask = color_2_img[coodinate[2]:coodinate[2]+self.mask_height, x]
                count_pixcel_red = np.count_nonzero(np.all(mask == color, axis=-1))
                if count_pixcel_red <= int(self.mask_height*0.25) and detect_flag == 1:
                    x_max = x
                    detect_flag = 0
                    break
                elif count_pixcel_red > int(self.mask_height*0.25):
                    detect_flag = 1
                if x == color_2_img.shape[1]-1:
                    if detect_flag == 1:
                        x_max = color_2_img.shape[1]-1
                    else:
                        raise ValueError("ブロック(色)が見つかりません")

            for x in range(x_max, 0, -1):
                mask = color_2_img[coodinate[2]:coodinate[2]+self.mask_height, x]
                count_pixcel_red = np.count_nonzero(np.all(mask == color, axis=-1))
                if count_pixcel_red <= int(self.mask_height*0.25) and detect_flag == 1:
                    x_min = x
                    break
                elif count_pixcel_red > int(self.mask_height*0.25):
                    detect_flag = 1
                if x == 1:
                    if detect_flag == 1:
                        x_min = 1
                    else:
                        raise ValueError("ブロック(色)が見つかりません")
        finally:
            if self.develop:
                save_path = os.path.join(self.save_dir_path, "search.png")
                cv2.imwrite(save_path, develop_img)

        return x_max, x_min, y_max, y_min

    def check_block_coordi(self, color_2_img, split_num_height, block_thre=100):
        """ブロックのおおまかな場所を特定する関数.

        TODO:ブロックが見つからなかった場合の処理を書く
        """
        develop_block_coodi_img = color_2_img.copy()

        # (ピクセル数, x, y)
        block_coodi_red = np.array([0, 0, 0])
        block_coodi_blue = np.array([[0, 0, 0], [0, 0, 0]])

        flag = 0
        # おおまかなブロックの場所を特定
        for x in range(0, int(color_2_img.shape[1]-self.mask_width), self.mask_width):
            if self.develop:
                develop_block_coodi_img[self.mask_height + 2, x] = Color.BLACK.value
                develop_block_coodi_img[self.mask_height*2+2, x] = Color.BLACK.value
                develop_block_coodi_img[self.mask_height*3+2, x] = Color.BLACK.value
                develop_block_coodi_img[self.mask_height, :] = Color.BLACK.value
                develop_block_coodi_img[:, self.mask_width] = Color.BLACK.value
            masks = []
            for i in range(split_num_height):
                masks.append(color_2_img[self.mask_height *
                             i:self.mask_height*(i+1), x:x+self.mask_width])

            # 画素数のカウント
            for i, mask in enumerate(masks):
                count_pixcel_blue = np.count_nonzero(np.all(mask == Color.BLUE.value, axis=-1))
                count_pixcel_red = np.count_nonzero(np.all(mask == Color.RED.value, axis=-1))

                if count_pixcel_red > block_coodi_red[0] and count_pixcel_red > block_thre:
                    block_coodi_red = np.array([count_pixcel_red, x, i*self.mask_height])

                if count_pixcel_blue > block_coodi_blue[0, 0] and count_pixcel_blue > block_thre:
                    block_coodi_blue[0] = np.array([count_pixcel_blue, x, i*self.mask_height])

                elif count_pixcel_blue > block_coodi_blue[1, 0] and count_pixcel_blue > block_thre:
                    block_coodi_blue[1] = np.array([count_pixcel_blue, x, i*self.mask_height])

        if self.develop:
            if not np.all(block_coodi_red == 0):
                develop_block_coodi_img[block_coodi_red[2]:block_coodi_red[2]+self.mask_height,
                                        block_coodi_red[1]:block_coodi_red[1]+self.mask_width] = Color.BLACK.value
            if not np.all(block_coodi_blue[0] == 0):
                develop_block_coodi_img[block_coodi_blue[0, 2]:block_coodi_blue[0, 2]+self.mask_height,
                                        block_coodi_blue[0, 1]:block_coodi_blue[0, 1]+self.mask_width] = Color.GREEN.value
            if not np.all(block_coodi_blue[1] == 0):
                develop_block_coodi_img[block_coodi_blue[1, 2]:block_coodi_blue[1, 2]+self.mask_height,
                                        block_coodi_blue[1, 1]:block_coodi_blue[1, 1]+self.mask_width] = Color.YELLOW.value

        return block_coodi_red, block_coodi_blue, develop_block_coodi_img

    def get_front_area(self, color_2_img, delete_front_color_5_img, binary_img, black_line_img):
        """エリア情報を生成する関数.

        Args:
            black_lines: 線分を格納した配列(x1, y1, x2, y2)
            changed_color_img: 6色画像
        """
        black_lines, _ = self.detect_line(black_line_img,
                                          "line_black_img.png",
                                          length_threshold=140)
        if black_lines is None:
            # コース情報取得に失敗
            return None

        # 緑のラインの角度と同じくらい&一番手前のラインを検出
        self.angle_threshold = 3
        front_line_index = 0  # 手前のラインが格納されているindex
        max_y = 0  # 一番手前のラインを選ぶための保持y座標
        for i, line in enumerate(black_lines):
            x1, y1, x2, y2 = map(int, line)
            vector = np.array([x2 - x1, y2 - y1])
            # 度数を求める
            if vector[0] == 0:
                a = 0
            else:
                a = vector[1] / vector[0]
            # TODO: self.basis_vectorの取得に失敗した場合を考える
            if self.angle_between_vectors(self.basis_vector, vector) < self.angle_threshold \
                    and max_y < max(y1, y2):
                front_line_index = i
                max_y = max(y1, y2)

        # 手前ライン上のサークルを検出
        x1, y1, x2, y2 = black_lines[front_line_index]
        self.basis_vector = np.array([x2 - x1, y2 - y1])
        # 度数を求める
        if self.basis_vector[0] == 0:
            a = 0
        else:
            a = self.basis_vector[1] / self.basis_vector[0]
        self.basis_vector_b = y1 - a * x1

        mask_width, mask_height = 140, 40
        circle_index_thred = 200
        max_black_index_count = 0
        blue_circle_count = 0
        pre_circle_color = None
        pre_coodinate = None
        may_red_block_in_blue_left = False
        may_blue_block_in_blue_left = False

        # 画像左から見ていく
        for x in range(binary_img.shape[1] - mask_width):
            y = int(a * x + self.basis_vector_b)
            mask = binary_img[int(y-mask_height/2):int(y+mask_height/2), x:x+mask_width]
            black_index_count = np.count_nonzero(np.all(mask == Color.BLACK.value, axis=-1))

            # サークルがあると判断した場合
            if black_index_count > circle_index_thred:
                # できるだけサークルをmask内に収めるための処理
                if max_black_index_count < black_index_count:
                    max_black_index_count = black_index_count
                    max_pixel_x = x  # 画素数の多い座標

            # mask内からサークルがなくなった場合のサークルの場所の決定と初期化
            if (black_index_count == 0 and max_black_index_count != 0) or \
                    x == binary_img.shape[1] - mask_width - 1:

                # サークルの色を調査
                mask1 = delete_front_color_5_img[int(y-mask_height/2):int(y+mask_height/2),
                                                 max_pixel_x:max_pixel_x+mask_width]
                mask2 = delete_front_color_5_img[int(y-mask_height/2)-20:int(y+mask_height/2),
                                                 max_pixel_x:max_pixel_x+mask_width]
                # 画素数をカウント
                blue_index_mask1 = np.count_nonzero(np.all(mask1 == Color.BLUE.value, axis=-1))
                blue_index_mask2 = np.count_nonzero(np.all(mask2 == Color.BLUE.value, axis=-1))
                green_index_mask1 = np.count_nonzero(np.all(mask1 == Color.GREEN.value, axis=-1))
                red_index_mask2 = np.count_nonzero(np.all(mask2 == Color.RED.value, axis=-1))

                # 描画用
                if self.develop and (green_index_mask1 > circle_index_thred or blue_index_mask1 > circle_index_thred):
                    binary_img[int(y-mask_height/2)-20:int(y+mask_height/2),
                               max_pixel_x:max_pixel_x+mask_width] = Color.YELLOW.value
                    binary_img[int(y-mask_height/2):int(y+mask_height/2),
                               max_pixel_x:max_pixel_x+mask_width] = Color.GREEN.value

                # サークルが緑
                if green_index_mask1 > circle_index_thred:
                    # 左側緑サークルについて考える
                    if pre_circle_color == Color.BLUE.name:
                        self.first_column_coordinate[1] = pre_coodinate
                        self.first_column_coordinate[2] = \
                            np.array([max_pixel_x, int(y-mask_height/2)])
                        # ブロックが青
                        if blue_index_mask2 > circle_index_thred:
                            self.course_info_block[3, 2] = self.blue_block_num
                            if self.block_count_blue == 0:
                                self.block_coordi_blue[0] = np.array(
                                    [int(x-mask_width+mask_width*0.25), y-mask_height*2])
                            else:
                                self.block_coordi_blue[1] = np.array(
                                    [int(x-mask_width+mask_width*0.25), y-mask_height*2])
                            self.block_count_blue += 1

                        # ブロックが赤
                        elif pre_circle_color == Color.BLUE.name and red_index_mask2 > circle_index_thred:
                            self.course_info_block[3, 2] = self.red_block_num
                            self.block_coordi_red = np.array(
                                [int(x-mask_width+mask_width*0.25), y-mask_height*2])
                            self.block_count_red += 1

                        # 青ブロックサークルが1つ右側しか見つからなかった場合
                        if blue_circle_count == 1:
                            if may_red_block_in_blue_left:  # 赤
                                self.course_info_block[3, 1] = self.red_block_num
                                self.block_coordi_red = np.array(
                                    [int(x-mask_width+mask_width*0.25), y-mask_height*2])
                                self.block_count_red += 1
                            elif may_blue_block_in_blue_left:  # 青
                                self.course_info_block[3, 1] = self.blue_block_num
                                if self.block_count_blue == 0:
                                    self.block_coordi_blue[0] = np.array(
                                        [int(x-mask_width+mask_width*0.25), y-mask_height*2])
                                else:
                                    self.block_coordi_blue[1] = np.array(
                                        [int(x-mask_width+mask_width*0.25), y-mask_height*2])
                                self.block_count_blue += 1

                    # 右側の緑サークルについて
                    elif pre_circle_color == Color.GREEN.name:
                        self.first_column_coordinate[3] = \
                            np.array([max_pixel_x, int(y-mask_height/2)])
                        if blue_index_mask2 > circle_index_thred:  # ブロックが青
                            self.course_info_block[3, 3] = self.blue_block_num
                            if self.block_count_blue == 0:
                                self.block_coordi_blue[0] = np.array(
                                    [int(x+mask_width*0.25), y-mask_height*2])
                            else:
                                self.block_coordi_blue[1] = np.array(
                                    [int(x+mask_width*0.25), y-mask_height*2])
                            self.block_count_blue += 1
                        if red_index_mask2 > circle_index_thred:  # ブロックが赤
                            self.course_info_block[3, 3] = self.red_block_num
                            self.block_coordi_red = np.array([int(x+mask_width*0.25), y-mask_height*2])
                            self.block_count_red += 1
                        break

                    pre_circle_color = Color.GREEN.name

                # サークルが青
                elif blue_index_mask1 > circle_index_thred:
                    blue_circle_count += 1
                    # 右側青サークルにブロックがある場合
                    if blue_circle_count == 2:
                        self.first_column_coordinate[0] = pre_coodinate
                        self.first_column_coordinate[1] = \
                            np.array([max_pixel_x, int(y-mask_height/2)])
                        if red_index_mask2 > circle_index_thred:  # 赤
                            self.course_info_block[3, 1] = self.red_block_num
                            self.block_coordi_red = np.array(
                                [int(x-mask_width+mask_width*0.25), y-mask_height*2])
                            self.block_count_red += 1
                        elif blue_index_mask2 / blue_index_mask1 > 1.5:  # 青
                            self.course_info_block[3, 1] = self.blue_block_num
                            if self.block_count_blue == 0:
                                self.block_coordi_blue[0] = np.array(
                                    [int(x-mask_width+mask_width*0.25), y-mask_height*2])
                            else:
                                self.block_coordi_blue[1] = np.array(
                                    [int(x-mask_width+mask_width*0.25), y-mask_height*2])
                            self.block_count_blue += 1

                        # 左側青サークルについて考慮
                        if may_red_block_in_blue_left:  # 赤
                            self.course_info_block[3, 0] = self.red_block_num
                            self.block_coordi_red = \
                                np.array([candi_red_block_coordi_x, candi_red_block_coordi_y])
                            self.block_count_red += 1
                            may_red_block_in_blue_left = False
                        elif may_blue_block_in_blue_left:  # 青
                            self.course_info_block[3, 0] = self.blue_block_num
                            if self.block_count_blue == 0:
                                self.block_coordi_blue[0] = \
                                    np.array([candi_blue_block_coordi_x, candi_blue_block_coordi_y])
                            else:
                                self.block_coordi_blue[1] = \
                                    np.array([candi_blue_block_coordi_x, candi_blue_block_coordi_y])
                            self.block_count_blue += 1
                            may_blue_block_in_blue_left = False

                        # 右側青サークルにブロックがある場合
                        if blue_index_mask2 / blue_index_mask1 > 1.3:  # 青
                            self.course_info_block[3, 1] = self.blue_block_num
                            if self.block_count_blue == 0:
                                self.block_coordi_blue[0] = np.array(
                                    [int(x-mask_width+mask_width*0.25), y-mask_height*2])
                            else:
                                self.block_coordi_blue[1] = np.array(
                                    [int(x-mask_width+mask_width*0.25), y-mask_height*2])
                            self.block_count_blue += 1
                        elif red_index_mask2 > 200:  # 赤
                            self.course_info_block[3, 1] = self.red_block_num
                            self.block_coordi_red = np.array(
                                [int(x-mask_width+mask_width*0.25), y-mask_height*2])
                            self.block_count_red += 1

                    # 検出したサークルが左側青の可能性
                    elif blue_circle_count == 1:
                        if red_index_mask2 > 200:
                            may_red_block_in_blue_left = True
                            candi_red_block_coordi_x = int(x - mask_width + mask_width*0.25)
                            candi_red_block_coordi_y = int(y - mask_height * 2)
                        elif blue_index_mask2 / blue_index_mask1 > 1.3:
                            may_blue_block_in_blue_left = True
                            candi_blue_block_coordi_x = int(x - mask_width + mask_width * 0.25)
                            candi_blue_block_coordi_y = int(y - mask_height * 2)

                    pre_circle_color = Color.BLUE.name

                # 初期化
                pre_coodinate = np.array([max_pixel_x, int(y-mask_height/2)])
                max_black_index_count = 0
                max_pixel_x = 0

            # x_max, x_min, y_max, y_min = self.check_coordi_ditail(color_2_img,
            #                                                       delete_front_color_5_img,
            #                                                       block_coodi_red,
            #                                                       color=Color.RED.value)
            # self.block_coordi_red_ditail = np.array([x_max, x_min, y_max, y_min])
            # color_5_circle_img[y_min:y_max, x_min:x_max] = Color.WHITE.value

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
        print(f"image_name: {self.image_name}")
        if not os.path.exists(image_path):
            raise (f"{image_path} is not exist")
        course_img = cv2.imread(image_path)
        self.img_shape = course_img.shape

        # Lコースの場合
        if not isR:
            # 画像を左右反転させる
            course_img = cv2.flip(course_img, 1)

        # 画像上のいらない領域を削除
        processed_course_img = self.pre_process_img(course_img)
        self.processed_img_shape = processed_course_img.shape

        # BGR色空間からHSV色空間への変換
        game_area_img = cv2.cvtColor(processed_course_img, cv2.COLOR_BGR2HSV)

        # 2色画像の作成(赤、青)
        color_2_img = np.full_like(game_area_img, Color.WHITE.value, dtype=np.uint8)
        self.change_color(game_area_img, color_2_img, self.RED1, Color.RED.value)
        self.change_color(game_area_img, color_2_img, self.RED2, Color.RED.value)
        self.change_color(game_area_img, color_2_img, self.BLUE, Color.BLUE.value)
        if self.develop:
            save_path = os.path.join(self.save_dir_path, "color_2_img.png")
            cv2.imwrite(save_path, color_2_img)

        # 5色画像の作成（黒以外）
        color_5_img = np.full_like(game_area_img, Color.WHITE.value, dtype=np.uint8)
        self.change_color(game_area_img, color_5_img, self.RED1, Color.RED.value)
        self.change_color(game_area_img, color_5_img, self.RED2, Color.RED.value)
        self.change_color(game_area_img, color_5_img, self.YELLOW, Color.YELLOW.value)
        self.change_color(game_area_img, color_5_img, self.GREEN, Color.GREEN.value)
        self.change_color(game_area_img, color_5_img, self.BLUE, Color.BLUE.value)
        if self.develop:
            save_path = os.path.join(self.save_dir_path, "color_5_img.png")
            cv2.imwrite(save_path, color_5_img)

        # 5色画像の白黒画像の作成
        binary_img = np.full_like(color_5_img, Color.BLACK.value, dtype=np.uint8)
        binary_img[np.all(color_5_img == Color.WHITE.value, axis=-1)] = Color.WHITE.value
        if self.develop:
            save_path = os.path.join(self.save_dir_path, "binary_img.png")
            cv2.imwrite(save_path, binary_img)

        # 黒ライン画像の作成
        black_line_img = np.full_like(game_area_img, Color.WHITE.value, dtype=np.uint8)
        self.change_color(game_area_img, black_line_img, self.BLACK, Color.BLACK.value)
        if self.develop:
            save_path = os.path.join(self.save_dir_path, "black_line_img.png")
            cv2.imwrite(save_path, black_line_img)

        # ブロックなしの画像を作成する
        # 大まかなブロックの位置を特定
        color_5_circle_img = color_5_img.copy()
        split_num_height = 4
        split_num_width = 30
        self.mask_height = int(game_area_img.shape[0] / split_num_height)
        self.mask_width = int(game_area_img.shape[1] / split_num_width)
        """
        # block_coodi_red = [ピクセル数, x, y]
        # block_coodi_blue = [[ピクセル数, x, y], [ピクセル数, x, y]]
        block_coodi_red, block_coodi_blue, develop_block_coodi_img = \
            self.check_block_coordi(color_2_img, split_num_height)
        if self.develop:
            save_path = os.path.join(self.save_dir_path, "block_coodi_img.png")
            cv2.imwrite(save_path, develop_block_coodi_img)

        # こまかな座標を特定
        if not np.all(block_coodi_red == 0):
            print("OK1")
            x_max, x_min, y_max, y_min = self.check_coordi_ditail(color_2_img,
                                                                  color_5_circle_img,
                                                                  block_coodi_red,
                                                                  color=Color.RED.value)
            self.block_coordi_red_ditail = np.array([x_max, x_min, y_max, y_min])
            color_5_circle_img[y_min:y_max, x_min:x_max] = Color.WHITE.value

        if not np.all(block_coodi_blue[0] == 0):
            print("OK2")
            x_max, x_min, y_max, y_min = self.check_coordi_ditail(color_2_img,
                                                                  color_5_circle_img,
                                                                  block_coodi_blue[0],
                                                                  color=Color.BLUE.value)
            self.block_coordi_blue_ditail[1] = np.array([x_max, x_min, y_max, y_min])
            color_5_circle_img[y_min:y_max, x_min:x_max] = Color.WHITE.value

        if not np.all(block_coodi_blue[1] == 0):
            print("OK3")
            x_max, x_min, y_max, y_min = self.check_coordi_ditail(color_2_img,
                                                                  color_5_circle_img,
                                                                  block_coodi_blue[1],
                                                                  color=Color.BLUE.value)
            self.block_coordi_blue_ditail[1] = np.array([x_max, x_min, y_max, y_min])
            color_5_circle_img[y_min:y_max, x_min:x_max] = Color.WHITE.value
        # """
        try:

            # コース手前の情報を取得
            delete_front_color_5_img = color_5_img.copy()
            self.get_front_area(color_2_img, delete_front_color_5_img, binary_img, black_line_img)
            # if self.block_coordi_red == 1 and  self.block_coordi_blue == 2:
            #     print("ブロックすべて発見")
            #     return self.course_info_block

            if not np.all(self.block_coordi_red == 0):
                color_5_img[:, self.block_coordi_red[0]:self.block_coordi_red[0]+2] = Color.BLACK.value # 開発用
                color_5_img[self.block_coordi_red[1]:self.block_coordi_red[1]+2, :] = Color.BLACK.value # 開発用
                coordi = np.array([0, self.block_coordi_red[0], self.block_coordi_red[1]])
                x_max, x_min, y_max, y_min = self.check_coordi_ditail(color_2_img,
                                                                      delete_front_color_5_img,
                                                                      coordi,
                                                                      color=Color.RED.value)
                self.block_coordi_blue_ditail[1] = np.array([x_max, x_min, y_max, y_min])
                color_5_img[y_min:y_max, x_min:x_max] = Color.WHITE.value

            if not np.all(self.block_coordi_blue[0] == 0):
                color_5_img[:, self.block_coordi_blue[0, 0]:self.block_coordi_blue[0, 0]+2] = Color.BLUE.value # 開発用
                color_5_img[self.block_coordi_blue[0, 1]:self.block_coordi_blue[0, 1]+2, :] = Color.BLUE.value # 開発用
                coordi = np.array([0, self.block_coordi_blue[0, 0], self.block_coordi_blue[0, 1]])
                x_max, x_min, y_max, y_min = self.check_coordi_ditail(color_2_img,
                                                                      delete_front_color_5_img,
                                                                      coordi,
                                                                      color=Color.BLUE.value)
                self.block_coordi_blue_ditail[1] = np.array([x_max, x_min, y_max, y_min])
                color_5_img[y_min:y_max, x_min:x_max] = Color.WHITE.value

            if not np.all(self.block_coordi_blue[1] == 0):
                color_5_img[:, self.block_coordi_blue[1, 0]:self.block_coordi_blue[1, 0]+2] = Color.GREEN.value # 開発用
                color_5_img[self.block_coordi_blue[1, 1]:self.block_coordi_blue[1, 1]+2, :] = Color.GREEN.value # 開発用
                coordi = np.array([0, self.block_coordi_blue[1, 0], self.block_coordi_blue[1, 1]])
                x_max, x_min, y_max, y_min = self.check_coordi_ditail(color_2_img,
                                                                      delete_front_color_5_img,
                                                                      coordi,
                                                                      color=Color.BLUE.value)
                self.block_coordi_blue_ditail[1] = np.array([x_max, x_min, y_max, y_min])
                color_5_img[y_min:y_max, x_min:x_max] = Color.WHITE.value

        finally:
            if self.develop:
                save_path = os.path.join(self.save_dir_path, "binary_img.png")
                cv2.imwrite(save_path, binary_img)
                save_path = os.path.join(self.save_dir_path, "delete_front_color_5_img.png")
                cv2.imwrite(save_path, color_5_img)

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

    # image_name = "0000_0002_0210_0000_6.png"
    # image_name = "0000_0002_0210_0000_8.png"
    # image_name = "0000_0020_0020_0010_10.png"
    # image_name = "0000_2000_0200_0010_12.png"
    # image_name = "0002_0100_0200_0000_13.png"
    # image_name = "1002_0000_0000_2000_15.png"
    image_name = "2002_0000_0000_0001_18.png"
    # image_name = "2002_0000_0000_1000_19.png"
    # image_name = "0000_0000_0000_2012_20.png"
    info = GetAreaInfo(image_name, work_dir_path, save_dir_path, develop=args.develop)
    try:
        info.get_area_info(args.isR)
    finally:
        print(f"最終結果--------------------------------------------------------------")
        print(f"self.block_count_red : {info.block_count_red}")
        print(f"self.block_coordi_red : {info.block_coordi_red}")
        print(f"self.block_count_blue: {info.block_count_blue}")
        print(f"self.block_coordi_blue: {info.block_coordi_blue[0], info.block_coordi_blue[1]}")
        print(f"self.course_info_block:\n{info.course_info_block}")
        print(f"self.first_column_coordinate:\n{info.first_column_coordinate}")
        print(f"---------------------------------------------------------------------")

    # 処理時間計測用
    execute_time = time.time() - start

    print(f"実行時間: {str(execute_time)[:5]}s")
    print("完了！")
