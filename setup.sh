#!/bin/bash

# Update the package list
sudo apt update

# Install clang
sudo apt -y install clang

# Install cmake
sudo apt -y install cmake

# Install build-essential and libstdc++
sudo apt -y install build-essential libstdc++6

# 安装 clang-format
sudo apt-get -y install clang-format

# 安装 clang-tidy
sudo apt-get -y install clang-tidy

# 安装 cpplint
sudo apt-get -y install cpplint
