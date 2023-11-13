"""ロボットの動作クラス(経路探索用).

@author: miyashita64 YKhm20020
"""


class Motion:
    """ロボットの動作を表わすクラス."""

    def __init__(self, params):
        """コンストラクタ.

        Args:
            params ([key: value]): 動作のパラメータ
        """
        self.params = params

    def make_command(self) -> str:
        """この動作を表わすコマンドを返す.

        Returns:
            command (str): 動作コマンド
        """
        command = ""
        for key in self.params:
            command += f"{self.params[key]},"
        return command


class Straight(Motion):
    """交点の直進動作."""

    def __init__(self, color, comment=""):
        """コンストラクタ.

        Args:
            color (str): 交点サークルまでライントレースする際に目標とする色
            comment (str): 動作のコメント
        """
        self.params = {
            "IS_command": "IS",
            "IS_comment": "交点内直進",
            "command": "\nCC",
            "line_trace_color": color,
            "adjust_brightness": 0,
            "comment": comment,
        }
        self.cost = 30  # TODO: 実際にかかる時間を計測する


class Turn(Motion):
    """交点の方向転換動作."""

    def __init__(self, angle, can_xr, comment=""):
        """コンストラクタ.

        Args:
            angle (int): 回頭角度[deg]
            comment (str): 動作のコメント
        """
        command = ""
        if abs(angle) == 180:
            # 後ろを向く
            command = "BR"
        elif angle > 0:
            # 右折
            command = "IR"
        else:
            # 左折
            command = "IL"

        self.params = {
            "command": command,
            "comment": comment,
        }

        # 回頭補正可能な場合のみ、パラメータを追記
        if can_xr:
            xr_params = {
                "xr_command": "\nXR",
                "xr_target_angle": 0,
                "xr_spped": 100,
                "xr_comment": "回頭補正",
            }
            # 辞書を結合
            self.params = {**self.params, **xr_params}

        self.cost = 5 + abs(angle)  # TODO: 実際にかかる時間を計測する
