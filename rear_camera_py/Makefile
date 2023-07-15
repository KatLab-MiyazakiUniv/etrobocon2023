help:
	@echo "カメラシステムを実行する"
	@echo " $$ make run"

run:
	python3 src/__main__.py


format:
	autopep8 --in-place --recursive src/ tests/

check_style:
	pycodestyle src/ tests/
	pydocstyle src/ tests/