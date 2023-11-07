make = make --no-print-directory

help:
	@echo "キャリブレーションを行う"
	@echo " $$ make calibrate"

	@echo "リアカメラで画像を取得する"
	@echo " $$ make image"
	@echo "リアカメラで画像を取得する(画像のファイル名を指定する)"
	@echo " $$ make image SAVE_NAME=(画像のファイル名)"
	@echo "リアカメラで1sごとに画像を取得する(Ctrl+Cで終了)"
	@echo " $$ make images"
	@echo "取得した画像を一括削除"
	@echo " $$ make rm-img"

	@echo "テストを実行する"
	@echo " $$ make test"
	@echo "フォーマットを実行する"
	@echo " $$ make format"
	@echo "スタイルチェックを実行する"
	@echo " $$ make check_style"
	@echo "フォーマットとスタイルチェックを実行する"
	@echo " $$ make format-check"
	@echo "ソースコードチェックとテストを実行する"
	@echo " $$ make all-check"
	@echo "デバッグを実行する"
	@echo " $$ make debug"
	@echo "デバッグフォルダを削除する"
	@echo " $$ make clean"
	@echo "ライブラリをinstallする"
	@echo " $$ make create-env-rear"

# キャリブレーション 
calibrate:
	@${make} kill
	python -m src --calibrate

## 画像取得関連 ##
ifeq ($(SAVE_NAME),)
    SAVE_NAME = FigA_1.png # デフォルト値
endif
snap_image:
	@${make} kill
	sudo python3 src/capture_snap_image.py  --save_path $(SAVE_NAME)
	@echo "$(SAVE_NAME) 画像取得！！！！！！！"

image:
	@${make} kill
	sudo python3 src/camera_interface.py
	@echo "画像取得！！！！！！！"


images:
	sudo python3 src/camera_interface.py -s

kill:
	@ps aux | grep "python -m src --server --server-port 10338" | grep -v "grep" | awk '{print $$2}' | sudo xargs -r kill -9


rm-img:
	sudo rm -rf image_data/

## 開発関連 ##
test:
	python -m unittest

format:
	autopep8 --in-place --recursive src/
	python -m autopep8 -i -r src/ tests/

check_style:
	python -m pycodestyle src/ tests/
	python -m pydocstyle src/ tests/

format-check:
	@${make} format
	@${make} check_style

all-check:
	@${make} format
	@${make} test
	@${make} check_style

debug:
	python -m src --debug

clean:
	rm -rf ./debug
	
create-env-rear:	
	pip install -r ../requirements.txt

## get_area_info.py用 ##
get_area:
	python src/get_area_info.py -develop

coodinate:
	python work/search_coodinate.py

make rm:
	rm work_image_data/*.png

test-get:
	python -m unittest tests/test_get_area_info.py
