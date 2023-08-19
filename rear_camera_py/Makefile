help:
	@echo "テストを実行する"
	@echo " $$ make test"
	@echo "キャリブレーションを行う"
	@echo " $$ make calibrate"
	@echo "デバッグを行う"
	@echo " $$ make debug"
	@echo "画像を取得する"
	@echo " $$ make image"
	@echo "画像を複数枚取得する"
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

image:
	python3 src/camera_interface.py

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
