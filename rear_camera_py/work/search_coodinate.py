"""作業用ファイル.
GUI上で画像をクリックした場所の座標とHSV値(OpenCV)を表示させる.
大会等で,環境によりブロックの色などの値が違う場合に調整できるように,
このファイルは残しておく.

@author kawanoichi

実行コマンド:
rear_camera/ 直下で実行することを想定
$ make coodinate
"""


import cv2
import tkinter as tk
from PIL import Image, ImageTk
import os

script_dir = os.path.dirname(os.path.abspath(__file__)) # /work
PROJECT_DIR_PATH = os.path.dirname(script_dir) #/rear_camera_py


class CameraCoordinateCalibrator:
    """カメラ画像から座標とHSV値を取得するクラス."""

    def __init__(self, img_path) -> None:
        """CameraCoordinateCalibratorのコンストラクタ.        

        Args:
            img_path(str): 画像パス
        """
        self.img = cv2.imread(img_path)
        self.past_massage = None

    def __set_coordinate(self, event) -> None:
        """マウス操作で取得した座標を各座標リストにセットするコールバック関数.

        Args:
            event: マウスイベント
        """
        hsv = cv2.cvtColor(self.img, cv2.COLOR_BGR2HSV)
        point_hsv = hsv[event.y, event.x]
        coodinate_massege = f"coordinate:({event.x},{event.y})\n"
        coodinate_massege = "{:<15}".format(coodinate_massege)
        hsv_massege = f"HSV[{point_hsv[0]},{point_hsv[1]},{point_hsv[2]}]"
        hsv_massege = "{:<15}".format(hsv_massege)

        self.__message["text"] = coodinate_massege + hsv_massege

        if self.past_massage is not None:
            self.__message2["text"] += self.past_massage

        self.past_massage = coodinate_massege + hsv_massege + "\n\n"

        # 古い順に削除(一回が32文字分)
        show_num = 12
        if len(self.__message2["text"]) > 32*(show_num+1):
            self.__message2["text"] = self.__message2["text"][32:]

    def __complete_input_coordinate(self, event) -> None:
        """Finishボタンで画面を閉じるためのコールバック関数.

        Args:
            event: マウスイベント(引数に記述する必要あり)
        """
        # ウィンドウを閉じる
        self.__window.destroy()

    def show_window(self) -> None:
        """画像取得ツールを起動する関数."""
        img_height = self.img.shape[0]  # 画像の高さ
        img_width = self.img.shape[1]  # 画像の横幅

        # ウィンドウを定義する
        self.__window = tk.Tk()
        self.__window.title("Search Coordinate")

        photo = ImageTk.PhotoImage(Image.fromarray(self.img))


        # ウィンドウサイズ: (画像の横幅 + UIの横幅) x 画像の高さ
        ui_size_x = 200
        ui_center_x = img_width + ui_size_x/2

        self.__window.geometry(f"{img_width+ui_size_x}x{img_height}")

        # UI座標
        text_box_size_x = 180
        text_box_size_y = 60
        text_box_x = ui_center_x - text_box_size_x/2
        text_box_y = 120
        
        ## 座標textを配置する
        # title
        self.__message_title = tk.Label(
            self.__window, text="指定座標\n", font=("", 10), bg="#ccc")
        self.__message_title.place(x=text_box_x,
                                   y=text_box_y,
                                   width=text_box_size_x,
                                   height=text_box_size_y*0.8)

        # coodinate
        self.__message = tk.Label(
            self.__window, font=("", 10), bg="#ccc")
        self.__message.place(x=text_box_x,
                             y=text_box_y + text_box_size_y*0.8,
                             width=text_box_size_x,
                             height=text_box_size_y)

        ## 座標履歴textを配置する
        # title
        self.__message2_title = tk.Label(
            self.__window, text="履歴\n", font=("", 10), bg="#ccc", anchor=tk.N)
        self.__message2_title.place(x=text_box_x,
                              y=text_box_y+text_box_size_y*1.6+20,
                              width=text_box_size_x,
                              height=text_box_size_y*0.8)

        # coodinate
        self.__message2 = tk.Label(
            self.__window, font=("", 10), bg="#ccc", anchor=tk.N)
        self.__message2.place(x=text_box_x,
                              y=text_box_y+text_box_size_y*2.4+20,
                              width=text_box_size_x,
                              height=text_box_size_y*11)

        # OpenCVで取得した画像を変換する
        img_rgb = cv2.cvtColor(self.img,
                               cv2.COLOR_BGR2RGB)  # imreadはBGRなのでRGBに変換
        img_pil = Image.fromarray(img_rgb)    # RGBからPILフォーマットへ変換
        img_tk = ImageTk.PhotoImage(img_pil)  # ImageTkフォーマットへ変換

        # ウィジェットを定義する
        # 画像を表示するCanvasを定義する
        canvas = tk.Canvas(self.__window, width=img_width,
                           height=img_height)
        
        canvas.create_image(0, 0, anchor=tk.NW, image=photo)


        # コールバック関数を指定する ("<Button-1>"は左クリックボタン)
        canvas.bind("<Button-1>", self.__set_coordinate)
        # Canvasを配置する
        canvas.pack(side=tk.LEFT)
        # Canvasに画像を設置する
        canvas.create_image(0, 0, image=img_tk, anchor=tk.NW)

        # Finish用のButtonを定義する
        finish_button = tk.Button(self.__window, text="Finish")
        # コールバック関数を指定する ("<Button-1>"は左クリックボタン)
        finish_button.bind("<Button-1>", self.__complete_input_coordinate)
        # Finishボタンを配置する
        button_size_x = 180
        finish_button.place(x=ui_center_x-button_size_x/2, y=20, width=button_size_x, height=50)

        """
        NOTE:スクロールすると座標が変わってしまう

        # スクロールバーの作成
        # 水平方向
        scrollbar_x = tk.Scrollbar(self.__window, orient=tk.HORIZONTAL, command=canvas.xview)
        scrollbar_x.pack(side="bottom", fill="x")
        # canvas.configure(xscrollcommand=scrollbar_x.set)

        # 垂直方向
        scrollbar_y = tk.Scrollbar(self.__window, orient=tk.VERTICAL, command=canvas.yview)
        scrollbar_y.pack(side="right", fill="y")

        # スクロールバーのスライダーが動かされた時に実行する処理を設定
        scrollbar_x.config(
            command=canvas.xview
        )
        scrollbar_y.config(
            command=canvas.yview
        )

        # キャンバススクロール時に実行する処理を設定
        canvas.config(
            xscrollcommand=scrollbar_x.set
        )
        canvas.config(
            yscrollcommand=scrollbar_y.set
        )
        # """

        # ウィンドウを表示する
        self.__window.mainloop()


if __name__ == "__main__":
    # 作業用の読み込みや保存用のディレクトリパス
    work_dir_path = os.path.join(PROJECT_DIR_PATH, "work_image_data")

    # 画像ファイル名
    image_name = "test.png"
    # 画像ファイルパス
    image_path = os.path.join(work_dir_path, image_name)

    # 画像があるかのチェック
    if not os.path.exists(image_path):
        print("画像がありません")
        print("image_path:", image_path)
        exit()

    coord = CameraCoordinateCalibrator(image_path)
    coord.show_window()
