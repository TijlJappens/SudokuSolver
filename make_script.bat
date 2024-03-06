@echo off
SET sudoku=./Sudokus/empty_sudoku.txt
if %1==build (
    REM clang ./src/main.cpp --output="./build/main.exe"
    g++ ./src/main.cpp -o ./build/main.exe -O3
) else if %1==run (
    powershell -Command "& {$stopwatch = [System.Diagnostics.Stopwatch]::StartNew(); .\build\main.exe %sudoku%; $stopwatch.Stop(); $executionTime = $stopwatch.Elapsed; Write-Host "Script execution time: $executionTime"}
) else (
    echo Invalid command. Use 'build' or 'run'.
)