"""ロボットの状態を保持するクラス(経路探索用).

@author: miyashita64 YKhm20020
"""

from enum import Enum
from motion import Motion, Straight, Curve
from block_area_map import BlockAreaMap

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

    def __hash__(self) -> int:
        """ロボットの状態に対しユニークな値を返す.

        Returns:
            (int): 座標,方角,コストを用いたハッシュ値
        """
        return hash(self.y + self.x * 10 + self.direction.value * 100 + self.cost * 1000)

    def __eq__(self, other) -> bool:
        """同値(==)の定義.

        Args:
            other (Robot): 比較対象のロボット
        Returns:
            (bool): 2つのRobotが同値であるかどうか
        """
        # 座標、方角、コストが同じであることを同じRobotであることの条件とする.
        return self.y == other.y and self.x == other.x \
            and self.direction == other.direction \
            and self.cost == other.cost

    def __lt__(self, other) -> None:
        """より小さい(self < other)の定義.

        Args:
            other (Robot): 比較対象のロボット
        Returns:
            (bool): self < other かどうか
        """
        return self.cost < other.cost

    def get_coord(self) -> (int, int):
        """自身の座標を返す.

        Returns:
            y (int): ロボットのマップ上のy座標
            x (int): ロボットのマップ上のx座標
        """
        return (self.y, self.x)

    def get_transitionable_robots(self, map_shape, circle_color_mapping):
        """一つの動作で遷移可能なロボットの状態のリストを返す.

        Args:
            map_shape ([int]): マップ情報の次元
            circle_color_mapping ({(str):(int, int)}): サークルの色を示す辞書
        Returns:
            transitionable_robots ([Robot]): 遷移可能なロボットのリスト
        """
        transitionable_robots = []
        max_y, max_x, *_ = map_shape
        color = None
        # 各方角に対する動作について
        for direct in Direction:
            if direct == self.direction:
                # 進行方向の方角については、前進することを考慮する
                forward_y, forward_x = self.get_forward_coord()
                next_circle = (forward_x, forward_y)
                for target_color, circle_coords in circle_color_mapping.items():
                    if next_circle in circle_coords:
                        color = target_color
                
                comment = f"({forward_y} {forward_x} {self.direction.name})"
                motion = Straight(color, comment)
                next_robot = Robot(forward_y, forward_x, self.direction,
                                   self.motions + [motion], self.cost + motion.cost)
            else:
                # 進行方向以外の方角については、方角を変えることを考慮する
                rotation_angle = direct - self.direction
                comment = f"({self.y} {self.x} {direct.name})"
                motion = Curve(rotation_angle, comment)
                next_robot = Robot(self.y, self.x, direct,
                                   self.motions + [motion], self.cost + motion.cost)
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
        """差(-)の定義.

        Returns:
            angle (int): 方角間の角度の差
        """
        diff = (self.value - other.value) % len(Direction)
        if diff <= len(Direction) / 2:
            # 右回りの場合
            angle = diff * 360 / len(Direction)
        else:
            # 左周りの場合
            angle = (diff - len(Direction)) * 360 / len(Direction)
        return angle


class Color(Enum):
    """色を保持するクラス."""
    RED = 0  # 赤
    GREEN = 1  # 緑
    BLUE = 2  # 青
    YELLOW = 3  # 黄
