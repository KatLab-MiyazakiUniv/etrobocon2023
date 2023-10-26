import numpy as np

class Map:
    """ブロックエリアのマップを定義するクラス."""
    
    def __init__(self, is_left_course):
        """コンストラクタ.

        Args:
            is_left_course (boolean): True: Lコース, False: Rコース
        """
        self.map = self.initialize_map(is_left_course)
    
    def initialize_map(self, is_left_course):
        # マップ初期化
        length = 4
        self.map = np.zeros((length, length))
        
        ### TODO: サークルの色や開始位置・終了位置が変わるためLRどちらのコースなのかを認識する必要がある ###
        
        # サークルの色を定義 (Rコースの座標を初期値とする)
        # コース正面を基準、左上原点とする
        red_circles = [(0, 2), (0, 3), (1, 2), (1, 3)]
        blue_circles = [(2, 2), (2, 3), (3, 2), (3, 3)]
        yellow_circles = [(0, 0), (0, 1), (1, 0), (1, 1)]
        green_circles = [(2, 0), (2, 1), (3, 0), (3, 1)]
        
        # サークルの色情報を格納した辞書を作成
        circle_color_mapping = {
            "RED": red_circles,
            "BLUE": blue_circles,
            "YELLOW": yellow_circles,
            "GREEN": green_circles
        }
        
        if is_left_course:
            self.map = np.fliplr(self.map)
            tmp = red_circles[0:].copy()
            tmp2 = blue_circles[0:].copy()
            red_circles[0:] = yellow_circles[0:]
            blue_circles[0:] = green_circles[0:]
            yellow_circles[0:] = tmp
            green_circles[0:] = tmp
            
        return self.map
    
    def get_dummy_block_coords(self):
        return self.dummy_block_coords
    
    def get_treasure_block_coord(self):
        return self.treasure_block_coord
    
    def display_map(self):
        # マップ表示
        print(self.map)
        
    def get_map(self):
        return self.map

    ### !!! この時点でのmapがコース情報取得から得られるはず                  !!! ###
    ### !!!  => これより上に値について、map以外は使用できないことを前提とする !!! ###

def main():
    is_left_course = False
    
    # ブロックの配置
    # dummy_block_coords = [(0, 0), (1, 1)]
    # treasure_block_coord = (1, 3)
    dummy_block_coords = [(0, 3), (3, 3)]
    treasure_block_coord = (3, 0)
    
    block_area_map = Map(is_left_course)
    
    map_data = block_area_map.get_map()

    for (y, x) in dummy_block_coords:
        map_data[y][x] = 1
    map_data[treasure_block_coord[0], treasure_block_coord[1]] = 2
    
    block_area_map.display_map()


if __name__ == "__main__":
    main()

