#!/bin/bash
rm -rf build-win && cmake -B build-win -DCMAKE_PREFIX_PATH="C:\msys64\mingw64\qt5-static\lib\cmake" -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build build-win -- -j6
strip build-win/audioviz.exe
