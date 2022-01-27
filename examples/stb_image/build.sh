#!/bin/bash
shopt -s nullglob
SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )" # https://stackoverflow.com/a/246128
python3 "$SCRIPT_DIR/../../download-include-files.py" *.c *.cpp *.h *.hpp
c++ -std=c++17 -O3 -I third_party/include *.c *.cpp
./a.out
