@echo off
SET sudoku=./Sudokus/empty_sudoku.txt
if %1==build (
    clang ./src/main.cpp --output="./build/main.exe"
) else if %1==run (
    .\build\main.exe %sudoku%
) else (
    echo Invalid command. Use 'build' or 'run'.
)