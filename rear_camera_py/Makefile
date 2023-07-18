help:
	@echo "画像を取得する"
	@echo " $$ make camera"
	@echo "フォーマットする"
	@echo " $$ make format"
	@echo "スタイルチェックを行う"
	@echo " $$ make check_style"

camera:
	python3 src/camera_interface.py

format:
	autopep8 --in-place --recursive src/

check_style:
	pycodestyle src/
	pydocstyle src/
