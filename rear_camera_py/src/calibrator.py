"""Arucoマーカを用いたリアカメラのキャリブレーションを行うモジュール.

@author aridome222 kodama0720
"""

import datetime
import json
import os
from typing import Tuple, List

import cv2
from cv2 import aruco
import numpy as np

from src.camera_interface import CameraInterface


class Calibrator:
    """リアカメラのキャリブレーションを行うクラス."""

    def __init__(
        self, camera_interface: CameraInterface,
        trans_mat_file: str = "rear_camera_param.npy",
        distance_file: str = "rear_camera_distance_param.json",
        debug: bool = True,
        debug_dir="debug"
    ) -> None:
        """コンストラクタ.

        Args:
            camera_interface (CameraInterface, optional): リアカメラのCameraInterfaceインスタンス.
                                                          Defaults to CameraInterface().
            trans_mat_file (str, optional): 射影変換用パラメータファイル名. Defaults to "rear_camera_param.npy".
            distance_file (str, optional): 射影変換後の画像座標と走行体の中心からの距離等の関係を保持するパラメータファイル名.
                                           Defaults to "rear_camera_distance_param.json".
            debug (bool, optional): Trueに設定するとデバッグ用の画像ファイルが生成される. Defaults to False.
            debug_dir (str, optional): デバッグ用の画像ファイルを生成するディレクトリ(存在しない場合は自動で生成される).
                                       Defaults to "debug".
        """
        self.__camera_interface = camera_interface
        self.__aruco_dictionary = aruco.getPredefinedDictionary(
            aruco.DICT_4X4_50)
        self.__trans_mat_file = trans_mat_file
        self.__distance_file = distance_file
        self.__debug = debug
        self.__debug_dir = debug_dir
        now = datetime.datetime.now()
        self.__debug_time = now.strftime("%Y-%m-%d_%H-%M-%S.%f")
        self.__create_debug_dir()

    def _calc_param(self, img: np.ndarray) -> Tuple[np.ndarray, float, float]:
        """各種パラメータを計算する関数.

        Args:
            img (np.ndarray): キャリブレーション用画像.

        Raises:
            RuntimeError: キャリブレーション用のArUcoマーカを検出できなかった場合に発生.

        Returns:
            Tuple[np.ndarray, float, float]: 各種パラメータ(射影変換パラメータ、pixとmmの縮尺パラメータ、
                                                画像中心点と4つのキャリブレーション用ArUcoマーカで結んだ中心点の距離[pix]).
        """
        # 各マーカーの角4点を格納した二次元配列(corners)と、検出したマーカーIDを格納した1次元配列(ids)を取得
        # idsマーカIDの並びはcornersのに格納されているマーカの座標の並びと一致する
        corners, ids, _ = aruco.detectMarkers(
            img, self.__aruco_dictionary)
        if ids is None:
            raise RuntimeError("Could not find markers.")

        h, w = img.shape[:2]

        for i, id in enumerate(ids):
            # マーカーのインデックス検索
            if (id[0] == 1):
                frame_coordinates = corners[i][0]
                break

        # NOTE: 射影変換後の画像において、マーカID：1のArUcoマーカの4点が一致する正方形を考える.
        # - distance_from_center_20mm
        #     上記正方形の中心点から辺までの距離(pix)、(40/2 mm).
        # - height_offset_from_center
        #     上記正方形の中心点のオフセット、
        #     0の場合上記正方形の中心点と射影変換後の画像の中心点が一致する(pix).
        distance_from_center_20mm = h/52.5

        height_offset_from_center = h/3

        # 上記正方形の射影変換後の画像における中心座標を求める(pix)
        cx, cy = w/2, h/2 + height_offset_from_center
        target_coordinates = np.float32(
            [[cx-distance_from_center_20mm, cy-distance_from_center_20mm],
             [cx+distance_from_center_20mm, cy-distance_from_center_20mm],
             [cx+distance_from_center_20mm, cy+distance_from_center_20mm],
             [cx-distance_from_center_20mm, cy+distance_from_center_20mm]])
        trans_mat = cv2.getPerspectiveTransform(
            frame_coordinates, target_coordinates)
        return trans_mat, distance_from_center_20mm, height_offset_from_center

    def calibrate(self) -> None:
        """キャリブレーションを行い、結果をパラメータファイルとして出力する関数.

        Raises:
            RuntimeError: 画像を取得できない際に発生する
        """
        now = datetime.datetime.now()
        self.__debug_time = now.strftime("%Y-%m-%d_%H-%M-%S.%f")
        img = self.__camera_interface.capture_image()
        if img is None:
            raise RuntimeError("Could not capture camera image.")
        self.__save_debug_img(img, 'calibration', 'captured.png')
        trans_mat, distance_from_center_20mm, height_offset_from_center = self._calc_param(img)
        np.save(self.__trans_mat_file, trans_mat)
        distance_data = {
            "distance_from_center_20mm": distance_from_center_20mm,
            "height_offset_from_center": height_offset_from_center
        }
        with open(self.__distance_file, mode="w") as fp:
            json.dump(distance_data, fp)

    def calibration_handler(self, cmd: str) -> str:
        """calibrate関数のラッパ(サーバに渡すコールバック関数として利用).

        Args:
            cmd (str): サーバがこの関数を呼び出す際に渡すデータ

        Returns:
            str: キャリブレーション結果に関するメッセージ
        """
        try:
            self.calibrate()
        except Exception as e:
            return "[ERROR] %s" % e
        return "[Info] Successfully calibrated"

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
        self.__create_debug_dir()
        if self.__debug:
            img_fname = "%s_%s_%s" % (prefix, self.__debug_time, suffix)
            debug_img_path = os.path.join(self.__debug_dir, img_fname)
            cv2.imwrite(debug_img_path, img)
