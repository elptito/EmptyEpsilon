#!/bin/bash

# this script builds EE for windows using the new setup
# see http://bridgesim.net/discussion/452/building-ee-for-windows-just-became-a-bit-easier
# used in Windows Linux Subsystem Ubuntu 18 LTS
# ------
# one time setup:
# add cmake apt repo (https://apt.kitware.com/):
# wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add -
# sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'
# sudo apt-get install kitware-archive-keyring
# sudo apt-key --keyring /etc/apt/trusted.gpg del C1F34CDD40CD72DA
# then update and install libraries:
# sudo apt update && sudo apt upgrade
# sudo apt install mingw-w64 cmake make p7zip-full unzip zip
# ------
# assuming $PROJECT_DIR env variable is a path to the project root, that has siblings `drmingw-0.8.2-win32`, and `SeriousProton` src project
#  
cd "$PROJECT_DIR"
rm -rf "_build_win32"
mkdir "_build_win32"
cd "_build_win32"
sudo cmake .. -DCMAKE_MAKE_PROGRAM=$(which make) -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw.toolchain -DSERIOUS_PROTON_DIR=../../SeriousProton -DDRMINGW_ROOT="$PROJECT_DIR/../drmingw-0.8.2-win32" -DENABLE_CRASH_LOGGER=1
sudo make -j $(grep -c ^processor /proc/cpuinfo) package
