#!/bin/bash

# this script builds EE for windows using the new setup
# see http://bridgesim.net/discussion/452/building-ee-for-windows-just-became-a-bit-easier
# used in Windows Linux Subsystem Ubuntu 18 LTS
# ------
# one time setup:
# sudo apt update && sudo apt upgrade
# sudo apt install mingw-w64 cmake
# ------
# assuming ~/workspace contains `drmingw-0.8.2-win32`, and `EmptyEpsilon` and `SeriousProton` src projects
cd ~/workspace/EmptyEpsilon
rm -rf "_build_win32"
mkdir "_build_win32"
cd "_build_win32"
sudo cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw.toolchain -DSERIOUS_PROTON_DIR=../../SeriousProton -DDRMINGW_ROOT="$(pwd)/../../drmingw-0.8.2-win32" -DENABLE_CRASH_LOGGER=1
sudo make -j $(grep -c ^processor /proc/cpuinfo) package