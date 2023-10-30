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
	@echo "フォーマットする"
	@echo " $$ make format"
	@echo "フォーマットチェックをする"
	@echo " $$ make format-check"
	@echo "スタイルチェックを行う"
	@echo " $$ make check_style"
	@echo "ソースコードチェックとテストを実行する"
	@echo " $$ make all-check"
	@echo "デバッグを行う"
	@echo " $$ make debug"
	@echo "デバッグフォルダを削除する"
	@echo " $$ make clean"
	@echo "ライブラリをinstallする"
	@echo " $$ make create-env-rear"

# キャリブレーション 
calibrate:
	python -m src --calibrate

## 画像取得関連 ##
ifeq ($(SAVE_NAME),)
    SAVE_NAME = FigA_1.png # デフォルト値
endif
image:
	python3 src/camera_interface.py  --save_path $(SAVE_NAME)

images:
	python3 src/camera_interface.py -s

rm-img:
	rm -rf image_data/

## 開発関連 ##
test:
	python -m unittest

format:
	autopep8 --in-place --recursive src/
	python -m autopep8 -i -r src/ tests/

format-check:
	python -m pycodestyle src/ tests/
	python -m pydocstyle src/ tests/

check_style:
	pycodestyle src/
	pydocstyle src/

all-check:
	@${make} format
	@${make} test
	@${make} format-check

debug:
	python -m src --debug

clean:
	rm -rf ./debug
	
create-env-rear:	
	pip install -r ../requirements.txt

## 作業用 ##
get_area:
	python src/get_area_info.py -develop

get_area2:
	python src/get_area_info2.py -develop

coodinate:
	python work/search_coodinate.py

zikken:
	python src/zikken.py

make rm:
	rm work_image_data/*.png