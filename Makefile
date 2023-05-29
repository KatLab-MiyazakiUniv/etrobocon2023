format:
    git status -s | awk '{print $2}' | xargs clang-format -i -style=file

# make format_file FILE_NAME="sample.cpp"
format_file:
    clang-format -i -style=file $(FILE_NAME)
