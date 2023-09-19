MAKEFILE_PATH := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
HOST = $(shell hostname)
make = make --no-print-directory

# 使い方
help:
	@echo ビルドする
	@echo " $$ make build"
	@echo ビルドファイルを消してからビルドする
	@echo " $$ make rebuild"
	@echo 走行を開始する\(実機限定\)
	@echo " $$ make start"
	@echo 指定ファイルをフォーマットする
	@echo " $$ make format FILES=<ディレクトリ名>/<ファイル名>.cpp"
	@echo すべての変更ファイルをフォーマットする
	@echo " $$ make format"
	@echo フォーマットチェックをする
	@echo " $$ make format-check"
	@echo テストを実行する
	@echo " $$ make gtest"
	@echo format, rebuild-gtest, format-checkを行う
	@echo " $$ make all-check"

# ビルドする
build:
# 実機で動かす場合(hostnameがkatlabから始まる場合)
ifeq ($(filter katlab%,$(HOST)), $(HOST))
	cd $(MAKEFILE_PATH)../ && make img=etrobocon2023
# それ以外の環境の場合(開発環境など)
else
	cd $(ETROBO_ROOT) && make app=etrobocon2023
endif

rebuild:
	rm -rf build
	@${make} build

# 実機の場合、走行を開始する 
start:
ifeq ($(filter katlab%,$(HOST)), $(HOST))
	cd $(MAKEFILE_PATH)../ && make start
endif

# ファイルにclang-formatを適用する
format:
# 指定ファイルがある場合、そのファイルにclang-formatを適用する
ifdef FILES
	clang-format -i -style=file $(FILES)
# ない場合、変更されたファイルのうち、cpp、hファイルにclang-formatを適用する
else
	git diff origin/main --name-only | awk '/\.cpp$$|\.h$$/ {print $$1}' | xargs clang-format -i -style=file
endif

format-check:
	find ./test ./module -type f -name "*.cpp" -o -name "*.h" | xargs clang-format --dry-run --Werror *.h *.cpp

# テストを実行する
# ※ターゲット名がtestだと動かない
gtest:
	set -eu
	./test/gtest/gtest_build.sh

rebuild-gtest:
	rm -rf build
	@${make} gtest

all-check:
	@${make} format
	@${make} rebuild-gtest
	@${make} format-check