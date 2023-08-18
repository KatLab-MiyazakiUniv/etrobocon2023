#!/bin/bash

# 引数から結合する番号を取得
number="$1"

# 引数がない場合はエラーを表示して終了
if [ -z "$number" ]; then
    echo "エラー: 結合する番号を引数として指定してください。"
    exit 1
fi

# 引数が0から5の範囲外の場合もエラーを表示して終了
if ! [[ "$number" =~ ^[0-5]$ ]]; then
    echo "エラー: 0から5までの番号を引数として指定してください。"
    exit 1
fi

# データファイルを生成するディレクトリパス
directory="./datafiles"

# データファイルのパーツがあるディレクトリパス
parts_directory="./datafiles/datafile_parts"

# 出力ファイル名
output_file="$directory/DoubleLoopLeft.csv"

# 出力ファイルが既に存在する場合は削除する
if [ -f "$output_file" ]; then
    rm "$output_file"
    echo "DoubleLoopLeft.csvが削除されました。"
fi

# ファイルが存在しない場合はエラーを表示して終了
function check_file_exists() {
    if [ ! -f "$1" ]; then
        echo "エラー: $1 が見つかりません。"
        exit 1
    fi
}

# 3つのファイルが存在するかチェック
check_file_exists "$parts_directory/DoubleLoopLeftFirst.csv"
check_file_exists "$parts_directory/DoubleLoopLeftMiddle_$number.csv"
check_file_exists "$parts_directory/DoubleLoopLeftEnd.csv"

# ファイルを結合する関数
function merge_files() {
    cat "$1" >> "$output_file"
    echo >> "$output_file"
}

# ファイルを結合する
merge_files "$parts_directory/DoubleLoopLeftFirst.csv"
merge_files "$parts_directory/DoubleLoopLeftMiddle_$number.csv"
merge_files "$parts_directory/DoubleLoopLeftEnd.csv"

echo "CSVファイルが結合されました。"
