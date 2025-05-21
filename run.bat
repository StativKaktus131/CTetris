@echo off
cls
gcc src/tetris.c src/tetromino.c -o bin/tetris.exe -Iinclude -Llib -lSDL3
cd bin
tetris.exe
cd ..