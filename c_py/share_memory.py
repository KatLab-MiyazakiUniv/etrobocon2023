import multiprocessing.shared_memory as sm
import ctypes

# C言語の構造体に相当するデータ型を定義
class SystemInfo(ctypes.Structure):
    _fields_ = [
                ("state", ctypes.c_char * 256),
                ("skip_camera_action", ctypes.c_bool)]

data = SystemInfo("notReady".encode("utf-8"), False)  # 構造体を作成

# 共有メモリセグメントを作成
shm = sm.SharedMemory(name="system_info_memory", create=True, size=ctypes.sizeof(SystemInfo))

key = None

while key != "q":
    key = input("input any key > ")
    data.state = key.encode("utf-8")
    data.skip_camera_action = not data.skip_camera_action
    # 共有メモリにアクセスすためのビューを生成
    shm_buf = SystemInfo.from_buffer(shm.buf)
    # 共有メモリにデータを書き込む
    shm_buf.state = data.state
    shm_buf.skip_camera_action = data.skip_camera_action
    # 共有メモリにアクセスすためのビューを削除
    del shm_buf

# 共有メモリを閉じて削除
shm.close()
shm.unlink()
