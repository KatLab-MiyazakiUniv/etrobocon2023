"""ロボットの状態を保持するクラス(経路探索用).

@author: miyashita64 YKhm20020
"""

from enum import Enum

class Robot:
    """ロボットの状態を保持するクラス."""
    def __init__(self, y, x, direction, motions=[], cost=0):
        """コンストラクタ.

        Args:
            y (int): ロボットのマップ上のy座標
            x (int): ロボットのマップ上のx座標
            direction (Direction): ロボットの向く方角(進行方向)
            motions ([Motion]): この状態に遷移するための動作のリスト
            cost (int): 動作のリストの総コスト
        """
        self.y = y
        self.x = x
        self.direction = direction
        self.motions = motions
        self.cost = cost

    def __eq__(self, other) -> bool:
        """同地比較(==)の定義.

        Returns:
            (bool): 2つのRobotが同値であるかどうか
        """
        # 座標、方角、コストが同じであることを同じRobotであることの条件とする.
        return self.y == other.y and self.x == other.x \
               and self.direction == other.direction \
               and self.cost == other.cost
    
    def __hash__(self) -> int:
        """ロボットの状態に対しユニークな値を返す."""
        return hash(self.y + self.x * 10 + self.direction.value * 100 + self.cost * 1000)

    def __lt__(self, other):
        """"""
        # self < other
        return self.cost < other.cost

    def get_coord(self) -> (int, int):
        """自身の座標を返す.

        Returns:
            y (int): ロボットのマップ上のy座標
            x (int): ロボットのマップ上のx座標
        """
        return (self.y, self.x)

    def get_transitionable_robots(self, map_shape):
        """一つの動作で遷移可能なロボットの状態のリストを返す.

        Args:
            map_shape ([int]): マップ情報の次元
        Returns:
            transitionable_robots ([Robot]): 遷移可能なロボットのリスト
        """
        transitionable_robots = []
        max_y, max_x, *_ = map_shape
        # 各方角に対する動作について
        for direct in Direction:
            if direct == self.direction:
                # 進行方向の方角については、前進することを考慮する
                forward_y, forward_x = self.get_forward_coord()
                ### TODO: マップから色情報を取得する ###
                color = "BLUE"
                comment = f"({forward_y} {forward_x} {self.direction.name})"
                motion = Straight(color, comment)
                next_robot = Robot(forward_y, forward_x, self.direction, self.motions + [motion], self.cost + motion.cost)
            else:
                # 進行方向以外の方角については、方角を変えることを考慮する
                rotation_angle = self.direction - direct
                comment = f"({self.y} {self.x} {direct.name})"
                motion = Curve(rotation_angle, comment)
                next_robot = Robot(self.y, self.x, direct, self.motions + [motion], self.cost + motion.cost)
            # 座標がマップ上であれば
            if 0 <= next_robot.y < max_y and 0 <= next_robot.x < max_x:
                # 1つの動作を追加したロボットの状態を保持する
                transitionable_robots += [next_robot]
        return transitionable_robots

    def get_forward_coord(self) -> (int, int):
        """進行方向に進んだ座標を返す.

        Returns:
            forward_y (int): 進行方向に進んだ場合のy座標
            forward_x (int): 進行方向に進んだ場合のx座標
        """
        forward_y = self.y
        forward_x = self.x
        if self.direction == Direction.N:
            forward_y -= 1
        elif self.direction == Direction.S:
            forward_y += 1
        elif self.direction == Direction.E:
            forward_x += 1
        elif self.direction == Direction.W:
            forward_x -= 1
        return forward_y, forward_x

class Direction(Enum):
    """方角を保持するクラス."""
    N = 0     # 北
    E = 1     # 東
    S = 2     # 南
    W = 3     # 西

    def __sub__(self, other) -> int:
        """差分(-)の定義.

        Returns:
            angle (int): 方角間の差を返す.
        """
        diff = self.value - other.value
        angle = diff * 360 / len(Direction)
        return angle

class Color(Enum):
    """色を保持するクラス."""
    RED    = 0  # 赤
    GREEN  = 1  # 緑
    BLUE   = 2  # 青
    YELLOW = 3  # 黄

class Motion:
    """ロボットの動作を表わすクラス."""
    def __init__(self, params):
        """コンストラクタ.

        Args:
            params ([key: value]): 動作のためのパラメータ.
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
    """"交点の直進動作."""
    def __init__(self, color, comment=""):
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
        self.cost = 10  # TODO: 実際にかかる時間を計測する

class Curve(Motion):
    """交点の右左折動作."""
    def __init__(self, angle, comment=""):
        self.params = {
            "command": "IR" if angle > 0 else "IL",
            "straight_distance": 65.0,
            "straight_speed": 200,
            "rotation_angle": abs(angle),
            "rotation_pwm": 100,
            "comment": comment,
        }
        self.cost = 5 + abs(angle) * 0.1  # TODO: 実際にかかる時間を計測する