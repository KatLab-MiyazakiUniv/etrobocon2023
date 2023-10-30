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
            command += f"{self.params[key]}, "
        return command


class Straight(Motion):
    """交点の直進動作."""

    def __init__(self, color, comment=""):
        """コンストラクタ.

        Args:
            color (str): 直進でライントレースする際に目標とする色
            comment (str): 動作のコメント
        """
        self.params = {
            "command": "CC",
            "line_trace_color": color,
            "line_trace_speed": 200,
            "line_trace_brightness": -10,
            "line_trace_kp": 0.33,
            "line_trace_ki": 0.12,
            "line_trace_kd": 0.12,
            "comment": comment,
        }
        self.cost = 30  # TODO: 実際にかかる時間を計測する


class Curve(Motion):
    """交点の右左折動作."""

    def __init__(self, angle, comment=""):
        """コンストラクタ.

        Args:
            angle (int): 回頭角度[deg]
            comment (str): 動作のコメント
        """
        self.params = {
            "command": "IR" if angle > 0 else "IL",
            "straight_distance": 65.0,
            "straight_speed": 200,
            "rotation_angle": abs(angle),
            "rotation_pwm": 100,
            "comment": comment,
        }
        self.cost = 5 + abs(angle)  # TODO: 実際にかかる時間を計測する
