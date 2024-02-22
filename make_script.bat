SET sudoku=./Sudokus/master_difficulty.txt
if %1==build (
    clang ./src/main.cpp --output="./build/main.exe"
) else if %1==run (
    .\build\main.exe %sudoku%
) else (
    echo Invalid command. Use 'build' or 'run'.
)