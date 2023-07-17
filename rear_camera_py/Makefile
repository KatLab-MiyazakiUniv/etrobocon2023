help:
	@echo "フォーマットする"
	@echo " $$ make format"
	@echo "スタイルチェックを行う"
	@echo " $$ make check_style"

format:
	autopep8 --in-place --recursive src/

check_style:
	pycodestyle src/
	pydocstyle src/
