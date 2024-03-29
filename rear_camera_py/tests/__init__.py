
"""パッケージの初期化を行うモジュール.

NOTE: このファイルが無いと、'python -m unittest'コマンドでPicameraのダミーモジュールを読み込むことができない.
@author: aridome222
"""
from pathlib import Path
import sys

sys.path.append(str(Path(__file__).parent / "dummy"))
sys.path.append(str(Path(__file__).parent.parent / "src"))
