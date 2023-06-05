MAKEFILE_PATH := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
HOST = $(shell hostname)

# 使い方
help:
	@echo ビルドする
	@echo " $$ make build"
	@echo 走行を開始する\(実機限定\)
	@echo " $$ make start"
	@echo 指定ファイルをフォーマットする
	@echo " $$ make format FILES=test/PidTest.cpp"
	@echo すべての変更ファイルをフォーマットする
	@echo " $$ make format"
	@echo テストを実行する
	@echo " $$ make gtest"

# ビルドする
build:
# 実機で動かす場合(hostnameがkatlabから始まる場合)
ifeq ($(filter katlab%,$(HOST)), $(HOST))
	cd $(MAKEFILE_PATH)../ && make img=etrobocon2023
# それ以外の環境の場合(開発環境など)
else
	cd $(ETROBO_ROOT) && make app=etrobocon2023
endif

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
	git status -s | awk '/\.cpp$$|\.h$$/ {print $$2}' | xargs clang-format -i -style=file
endif

# テストを実行する
# ※ターゲット名がtestだと動かない
gtest:
	set -eu
	./test/gtest/gtest_build.sh