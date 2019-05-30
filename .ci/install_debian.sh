#!/bin/sh
set -e

apt-get update

# noninteractive tzdata ( https://stackoverflow.com/questions/44331836/apt-get-install-tzdata-noninteractive )
export DEBIAN_FRONTEND=noninteractive

# CI specific packages
apt-get install -y gcc git clang valgrind ccache

# Core dependencies
apt-get install -y cmake libace-dev libboost-all-dev qtbase5-dev qtdeclarative5-dev qtmultimedia5-dev libqt5opengl5-dev libqcustomplot-dev libeigen3-dev
