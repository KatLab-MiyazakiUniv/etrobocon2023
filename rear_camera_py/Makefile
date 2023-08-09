help:
	@echo "画像を取得する"
	@echo " $$ make image"
	@echo "画像を複数枚取得する"
	@echo " $$ make images"
	@echo "取得した画像を一括削除"
	@echo " $$ make rm"
	@echo "フォーマットする"
	@echo " $$ make format"
	@echo "スタイルチェックを行う"
	@echo " $$ make check_style"

image:
	python3 src/camera_interface.py

images:
	python3 src/camera_interface.py -s

rm:
	rm -rf image_data/
	
format:
	autopep8 --in-place --recursive src/

check_style:
	pycodestyle src/
	pydocstyle src/
