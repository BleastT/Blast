@echo off

set file_name=blast

if not exist build (mkdir build)
cd build

cmake .. -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=clang++
mingw32-make

if exist "%file_name%.exe" (start %file_name% & goto END)

:END