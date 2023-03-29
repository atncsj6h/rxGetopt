#! /usr/bin/env bash

find . -type f -name .DS_Store              -exec rm {} \;
find . -type f -name a.out                  -exec rm {} \;

find . -type f -name CMakeCache.txt         -exec rm {} \;
find . -type d -name CMakeFiles             -exec rm -rf {} \;
find . -type f -name Makefile               -exec rm {} \;

find . -type f -name cmake_install.cmake    -exec rm {} \;

find . -type f -name "*.o"                  -exec rm {} \;
find . -type f -name "*.a"                  -exec rm {} \;
find . -type f -name "*.so"                 -exec rm {} \;
find . -type f -name "*.dylib"              -exec rm {} \;

find . -type f -name "vars_*.txt"           -exec rm {} \;
