help:
	@echo "テストを実行する"
	@echo " $$ make test"
	@echo "キャリブレーションを行う"
	@echo " $$ make calibrate"
	@echo "デバッグを行う"
	@echo " $$ make debug"
	@echo "リアカメラで撮影する"
	@echo " $$ make image"
	@echo "リアカメラで撮影する(画像のファイル名を指定する)"
	@echo " $$ make image SAVE_NAME=FigA_1.png"
	@echo "リアカメラで1sごとに撮影する(Ctrl+Cで終了)"
	@echo " $$ make images"
	@echo "取得した画像を一括削除"
	@echo " $$ make rm"
	@echo "フォーマットする"
	@echo " $$ make format"
	@echo "フォーマットチェックをする"
	@echo " $$ make format-check"
	@echo "スタイルチェックを行う"
	@echo " $$ make check_style"
	@echo "デバッグフォルダを削除する"
	@echo " $$ make clean"

test:
	python -m unittest

calibrate:
	python -m src --calibrate

debug:
	python -m src --debug

ifeq ($(SAVE_NAME),)
    SAVE_NAME = FigA_1.png
endif
image:
	python3 src/camera_interface.py  --save_path $(SAVE_NAME)

images:
	python3 src/camera_interface.py -s

rm:
	rm -rf image_data/

format:
	autopep8 --in-place --recursive src/
	python -m autopep8 -i -r src/ tests/

format-check:
	python -m pycodestyle src/ tests/
	python -m pydocstyle src/ tests/

check_style:
	pycodestyle src/
	pydocstyle src/

clean:
	rm -rf ./debug
	
