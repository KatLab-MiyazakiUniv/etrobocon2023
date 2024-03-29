"""コースの直線に対する機体の角度算出するモジュール.

@author: aridome222 kodama0720
"""

import datetime
import json
import os
from typing import Tuple, Union
import cv2
import numpy as np

from src.black_extractor import BlackExtractor
from src.camera_interface import CameraInterface


class LineAngleCalculator:
    """コースの直線に対する機体の角度算出するクラス."""

    def __init__(
        self,
        camera_interface: CameraInterface,
        trans_mat_file: str = "rear_camera_param.npy",
        distance_file: str = "rear_camera_distance_param.json",
        running_body_to_4_ArUco_center: float = 312.5,
        debug: bool = True,
        debug_dir="debug"
    ) -> None:
        """コンストラクタ.

        Args:
            camera_interface (CameraInterface, optional): リアカメラ画像を取得するためのCameraInterfaceインスタンス.
                                                          Defaults to CameraInterface().
            trans_mat_file (str, optional): 射影変換用パラメータファイル名. Defaults to "rear_camera_param.npy".
            distance_file (str, optional): 射影変換後の画像座標と走行体の中心からの距離等の関係を保持するパラメータファイル名.
                                           Defaults to "rear_camera_distance_param.json".
            running_body_to_4_ArUco_center (float): 走行体の座標からマーカID:1のArUcoマーカの中心点までの距離(mm)
            debug (bool, optional): Trueに設定するとデバッグ用の画像ファイルが生成される. Defaults to False.
            debug_dir (str, optional): デバッグ用の画像ファイルを生成するディレクトリ(存在しない場合は自動で生成される).
                                       Defaults to "debug".

        Raises:
            FileNotFoundError: 各種パラメータファイルが見つからない場合に発生.
            KeyError: 射影変換後の画像座標と走行体の中心からの距離等の関係を保持するパラメータファイルのデータが一部でも欠損している場合に発生.
        """
        self.__camera_interface = camera_interface
        self.__rbody_to_4Acenter = running_body_to_4_ArUco_center

        # リアカメラのキャリブレーション情報を読み込む
        self.__trans_mat_file = trans_mat_file
        self.__distance_file = distance_file
        self.load_params()

        # デバッグモード関連の設定
        self.__debug = debug
        self.__debug_dir = debug_dir
        self.__update_debug_time()
        self.__create_debug_dir()

    def load_params(self) -> None:
        """パラメータファイルを読み込む.

        Raises:
            FileNotFoundError: パラメータファイルが見つからない場合に発生
            KeyError: パラメータファイルの内容に期待するキーのデータが無い場合に発生
        """
        if not os.path.isfile(self.__trans_mat_file):
            raise FileNotFoundError("file name: '%s'" % self.__trans_mat_file)
        if not os.path.isfile(self.__distance_file):
            raise FileNotFoundError("file name: '%s'" % self.__distance_file)

        self.__trans_mat = np.load(self.__trans_mat_file)

        # NOTE: 射影変換後の画像において、マーカID：1のArUcoマーカの4点が一致する正方形を考える.
        # - distance_from_center_20mm
        #     上記正方形の中心点から辺までの距離(pix)、(40/2 mm).
        # - height_offset_from_center
        #     上記正方形の中心点のオフセット、
        #     0の場合上記正方形の中心点と射影変換後の画像の中心点が一致する(pix).
        with open(self.__distance_file) as fp:
            distance_data = json.load(fp)
        key = "distance_from_center_20mm"
        if key not in distance_data:
            raise KeyError("key not found: '%s', file: %s" %
                           (key, self.__distance_file))
        self.__distance_from_center_20mm = distance_data[key]
        key = "height_offset_from_center"
        if key not in distance_data:
            raise KeyError("key not found: '%s', file: %s" %
                           (key, self.__distance_file))
        self.__height_offset_from_center = distance_data[key]

    def calc_yaw_angle(
        self,
        debug_img_fname_prefix: str = "angle"
    ) -> Union[float, None]:
        """検出した直線と機体の中心線とのなす角を算出する.

        Args:
            debug_img_fname_prefix (str, optional): デバッグ用画像の接頭辞. Defaults to "angle".

        Returns:
            Union[float, None]: 検出した直線と機体の中心線とのなす角、直線を検出できなかった場合は`None`を返す.
        """
        now = datetime.datetime.now()
        self.__debug_time = now.strftime("%Y-%m-%d_%H-%M-%S.%f")

        img = self.__camera_interface.capture_image()

        if img is None:
            return None

        self.__save_debug_img(img, debug_img_fname_prefix, "captured.png")

        # 射影変換
        img_transformed = self.get_transformed_image(img)
        self.__save_debug_img(img_transformed, debug_img_fname_prefix, "transformed.png")

        # 画像から黒線を抽出(白黒画像に変換)
        binary_img = BlackExtractor.extract_black(img_transformed)

        # 線分を検出(線分上の2点座標[x1,y1,x2,y2])
        detected_line = self.detect_line_segment(binary_img)

        if detected_line is None:
            return None

        # 単位ベクトルに変換[x1-x2, y1-y2]
        a = np.array([detected_line[0] - detected_line[2],
                      detected_line[1] - detected_line[3]])
        x = np.linalg.norm(a)
        your_line = a / x

        # 走行体のベクトル
        y_axis = np.array([0, 1])

        # 内積を求める
        dot_product = np.dot(y_axis, your_line)

        # なす角を求める
        angle_2_y = np.arccos(dot_product) * 180 / np.pi

        if angle_2_y > 90:
            angle_2_y -= 180

        if self.__debug:
            # 検出画像の保存
            debug_img = self.draw_detected_lines_on_the_image(
                img_transformed, detected_line, angle_2_y)
            self.__save_debug_img(debug_img, debug_img_fname_prefix, "detected.png")

        return float(angle_2_y)

    def get_transformed_image(self, img: np.ndarray, borderValue=(255, 255, 255)) -> np.ndarray:
        """射影変換を行った画像を取得する.

        Args:
            img (np.ndarray): 射影変換前の画像.
            borderValue (tuple, optional): 射影変換後の画像における余白部分の色. Defaults to (255, 255, 255).

        Returns:
            np.ndarray: 射影変換後の画像.
        """
        return cv2.warpPerspective(
            img, self.__trans_mat, (img.shape[1], img.shape[0]), borderValue=borderValue)

    def detect_line_segment(
        self,
        img,
        length_threshold_mm: float = 62.5,
        distance_threshold: float = 1.41421356,
        canny_th1: int = 50,
        canny_th2: int = 50,
        canny_aperture_size: int = 7,  # 3~7
        do_merge: bool = False
    ) -> Union[np.ndarray, None]:
        """画像上の線分を検出する.

        Args:
            img (np.ndarray): 射影変換後の画像.

            NOTE:以下の引数の詳細: https://emotionexplorer.blog.fc2.com/blog-entry-128.html
                                  https://nsr-9.hatenablog.jp/entry/2021/08/12/200000
            length_threshold_mm (float): 線分を検出する際の長さ閾値(これより短い線は除外)
                                       検出したい線分の長さ = 交点to交点の線分の長さの4分の1（※交点サークル内の距離は除く）
                                                          = 62.5
                                       射影変換によってできた元画像の枠(直線)と同一とみなす距離
            distance_threshold (float): 距離閾値(この値より遠い座標は、同一線ではない)
                                        1.41421356はcv2.ximgproc.createFastLineDetectorの規定値
            canny_th1 (int): Cannyヒステリシス1
            canny_th2 (int): Cannyヒステリシス2
            canny_aperture_size (int): Cannyソベルオペレータ アパチャーサイズ
            do_merge (bool): 増分マージオプション

        Returns:
            Union[np.ndarray, None]: 検出した線分、線分を検出できなかった場合は`None`を返す.
        """
        # length_threshold_mmをpix値に変換(0.9倍補正)
        length_threshold_pix = int(self.mm_to_pix(length_threshold_mm) * 0.9)

        # 線分検出モジュールのインスタンス化
        fld = cv2.ximgproc.createFastLineDetector(
            length_threshold_pix,
            distance_threshold,
            canny_th1,
            canny_th2,
            canny_aperture_size,
            do_merge
        )

        # 線分の2点座標を検出 ※型:[[[x1,y1,x2,y2]],...]
        lines = fld.detect(img)

        if lines is None:
            return None

        # 検出する範囲の境界線(ignore_border(pix))を、detect_range_from_rbody_mmから求める
        # detect_range_from_rbody_mm: 線分検出したい範囲を走行体からの距離で指定する(mm)
        img_h, img_w = img.shape[:2]
        detect_range_from_rbody_mm = 350*np.sqrt(2)*1.3  # 交点to交点*√２*余幅補正 (mm) （※ここでは交点サークル内の距離を含む）
        _, ignore_border = self.runner_base_coordinate_to_image_base_coordinate_pix(
            0, detect_range_from_rbody_mm, img_h, img_w)

        # 線分のy座標が2点とも境界線より大きい線分を取得(pix) ※型:[[x1,y1,x2,y2],...]
        lines = lines[np.where((lines[:, :, 1] > ignore_border) & (lines[:, :, 3] > ignore_border))]

        if len(lines) == 0:
            return None

        # 射影変換で得られたコース画像の両端と下の枠上に存在する点
        # NOTE: 枠(直線)を線分検出で実際に取得した座標を使用
        straight_line_right = np.array([1595.393, 0.235, 939.959, 1093.975])
        straight_line_left = np.array([698.343, 1096.039, 10.602, 0.249])
        straight_line_lower = np.array([940.000, 1095.000, 701.000, 1095.000])

        # 画像の枠上(右左下)に線分があれば削除する
        delete_list = []
        for i in range(len(lines)):
            # 枠(直線)との距離が distance_threshold より小さい座標を除外する
            if LineAngleCalculator.calc_distance(
                    straight_line_right, lines[i, 0], lines[i, 1]) < distance_threshold\
                    and LineAngleCalculator.calc_distance(
                    straight_line_right, lines[i, 2], lines[i, 3]) < distance_threshold:
                delete_list.append(i)

            elif LineAngleCalculator.calc_distance(
                    straight_line_left, lines[i, 0], lines[i, 1]) < distance_threshold\
                    and LineAngleCalculator.calc_distance(
                    straight_line_left, lines[i, 2], lines[i, 3]) < distance_threshold:
                delete_list.append(i)

            elif LineAngleCalculator.calc_distance(
                    straight_line_lower, lines[i, 0], lines[i, 1]) < distance_threshold\
                    and LineAngleCalculator.calc_distance(
                    straight_line_lower, lines[i, 2], lines[i, 3]) < distance_threshold:
                delete_list.append(i)

        if len(delete_list) != 0:
            lines = np.delete(lines, delete_list, 0)
            if len(lines) == 0:
                return None

        # 一番手前にある線分を抽出する(y軸最大値)
        calc_lines = lines.copy()  # 選別用
        calc_lines[:, 0:4:2] = 0  # x座標部分に0を代入
        return lines[np.unravel_index(np.argmax(calc_lines), lines.shape)[0]]

    def draw_detected_lines_on_the_image(
        self,
        img: np.ndarray,
        detected_line: np.ndarray,
        angle: float
    ) -> np.ndarray:
        """検出した線分及び、その線分と機体の中心線のなす角を線分の検出用いた画像へ描画するデバッグ用関数.

        Args:
            img (np.ndarray): 線分の検出に用いた画像データ.
            detected_line (np.ndarray): 描画する線分の座標 [x1,y1,x2,y2]
            angle (float): 検出した線分と機体の中心線とのなす角.

        Returns:
            np.ndarray: 検出した線分及び、その線分と機体の中心線のなす角を描画した画像データ.
        """
        # 検出した線分を描画
        cv2.line(img,
                 (int(detected_line[0]), int(detected_line[1])),
                 (int(detected_line[2]), int(detected_line[3])),
                 (0, 0, 255),  # BGR
                 thickness=4)  # 描画する線分の太さ

        # 線分の角度を描画
        tx, ty = int((detected_line[0]+detected_line[2]) / 2), \
            int((detected_line[1]+detected_line[3])/2)
        cv2.putText(img, "%.2f" % angle, (tx, ty),
                    cv2.FONT_HERSHEY_COMPLEX, 1.5, (0, 255, 0), thickness=2)
        return img

    @staticmethod
    def calc_distance(straight_line: np.ndarray, x: float, y: float) -> float:
        """直線と座標の距離を計算する関数.

        Args:
            straight_line (float): 直線上の2点座標を格納した配列[x1, y1, x2, y2]
            x (float): 直線との距離を求めたい座標x
            y (float): 直線との距離を求めたい座標y

        Returns:
            float: 点と直線の距離
        """
        u = np.array([straight_line[2] - straight_line[0], straight_line[3] - straight_line[1]])
        v = np.array([x - straight_line[0], y - straight_line[1]])
        return abs(np.cross(u, v) / np.linalg.norm(u))

    def runner_base_coordinate_to_image_base_coordinate_pix(
        self,
        rx: Union[int, float],
        ry: Union[int, float],
        img_h: int,
        img_w: int
    ) -> Tuple[int, int]:
        """機体の中心を原点とした座標を画像座標に変換する.

        Args:
            rx (Union[int, float]): 走行体の中心を原点としたX座標[mm].
            ry (Union[int, float]): 走行体の中心を原点としたY座標[mm].
            img_h (int): 画像の縦幅[pix].
            img_w (int): 画像の横幅[pix].

        Returns:
            Tuple[int, int]: 変換後の座標(x, y)[pix].
        """
        # 画像上のx座標(pix_x)を求める
        pix_x = self.mm_to_pix(rx) + img_w/2

        # 画像上のy座標(pix_y)を求める
        running_body_to_image_lower = self.__rbody_to_4Acenter - \
            self.pix_to_mm(img_h/2 - self.__height_offset_from_center)
        pix_y = img_h - self.mm_to_pix(ry - running_body_to_image_lower)

        return pix_x, pix_y

    def mm_to_pix(self, mm: Union[int, float]) -> float:
        """mmをpixに変換する.

        Args:
            mm (Union[int, float]): 変換したい値[mm]

        Returns:
            float: 変換結果[pix]
        """
        return mm * self.__distance_from_center_20mm / 20

    def pix_to_mm(self, pix: Union[int, float]) -> float:
        """pixをmmに変換する.

        Args:
            pix (Union[int, float]): 変換したい値[pix]

        Returns:
            float: 変換結果[mm]
        """
        return pix * 20 / self.__distance_from_center_20mm

    def __create_debug_dir(self) -> None:
        if not self.__debug:
            return
        if not os.path.exists(self.__debug_dir):
            os.makedirs(self.__debug_dir)

    def __save_debug_img(self, img: np.ndarray, prefix: str, suffix: str) -> None:
        """デバッグ用に画像をファイルに保存する関数.

        デバッグモードがFalseの場合は何も実行しない.
        Args:
            img (np.ndarray): 保存する画像データ
            prefix (str): ファイル名の先頭文字列
            suffix (str): 拡張子等を含めたファイル名の末尾
        """
        if not self.__debug:
            return
        self.__create_debug_dir()
        img_fname = "%s_%s_%s" % (prefix, self.__debug_time, suffix)
        debug_img_path = os.path.join(self.__debug_dir, img_fname)
        cv2.imwrite(debug_img_path, img)

    def __update_debug_time(self) -> None:
        if not self.__debug:
            return
        now = datetime.datetime.now()
        self.__debug_time = now.strftime("%Y-%m-%d_%H-%M-%S.%f")
