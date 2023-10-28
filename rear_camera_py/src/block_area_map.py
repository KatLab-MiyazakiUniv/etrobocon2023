import numpy as np

class BlockAreaMap:
    """ブロックエリアのマップを定義するクラス."""
    
    def __init__(self, block_area_map):
        """コンストラクタ.

        Args:
            block_area_map (np.array): コース情報
        """
        self.dummy_block_coords = [(0, 3), (3, 3)]
        self.treasure_block_coord = (3, 0)
        # TODO: ブロックエリア検知 -> block_area_map と受け取るよう修正する
        is_left_course = False
        
        if is_left_course:
            self.start_coord = (2, 3)
            self.end_coord = (1, 0)
        else:
            self.start_coord = (2, 0)
            self.end_coord = (1, 3)
        
        ### 削除対象箇所
        self.block_area_map = self.initialize_map(is_left_course, self.treasure_block_coord, self.dummy_block_coords)
        ### 削除対象箇所
        
        self.circle_color_mapping = self.get_circle_color_mapping(is_left_course)
    
    ### 削除対象箇所
    def initialize_map(self, is_left_course, treasure_block_coord, dummy_block_coords):
        # マップ初期化 (TODO: ブロックエリア検知機能と連携次第、削除)
        length = 4
        self.map_data = np.zeros((length, length))
        
        for (y, x) in dummy_block_coords:
            self.map_data[y][x] = 1
        self.map_data[treasure_block_coord[0], treasure_block_coord[1]] = 2
            
        return self.map_data
    ### 削除対象箇所
    
    def get_circle_color_mapping(self, is_left_course):
        ### TODO: サークルの色や開始位置・終了位置が変わるためLRどちらのコースなのかを認識する必要がある ###
        
        # サークルの色を定義 (Rコースの座標を初期値とする)
        # コース正面を基準、左上原点とする
        red_circles = [(0, 2), (0, 3), (1, 2), (1, 3)]
        blue_circles = [(2, 2), (2, 3), (3, 2), (3, 3)]
        yellow_circles = [(0, 0), (0, 1), (1, 0), (1, 1)]
        green_circles = [(2, 0), (2, 1), (3, 0), (3, 1)]
        
        if is_left_course:
            self.map_data = np.fliplr(self.map_data)
            tmp = red_circles[0:].copy()
            tmp2 = blue_circles[0:].copy()
            red_circles[0:] = yellow_circles[0:]
            blue_circles[0:] = green_circles[0:]
            yellow_circles[0:] = tmp
            green_circles[0:] = tmp 
            
        # サークルの色情報を格納した辞書
        circle_color_mapping = {
            "RED": red_circles,
            "BLUE": blue_circles,
            "YELLOW": yellow_circles,
            "GREEN": green_circles
        }
            
        return circle_color_mapping
    
    def display_map(self):
        # マップ表示
        print(self.map_data)

    ### !!! この時点でのmapがコース情報取得から得られるはず                  !!! ###
    ### !!!  => これより上に値について、map以外は使用できないことを前提とする !!! ###

def main():
    is_left_course = False

    block_area = BlockAreaMap(is_left_course)
    
    block_area.display_map()


if __name__ == "__main__":
    main()

