#!/bin/sh
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ../
cmake --build . --target PeopleDetector -- -j4
cd ..
#./build/PeopleDetector town.mp4  
