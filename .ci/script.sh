#!/bin/sh
set -e

# Install source deps

# Build, blocktest
cd $TRAVIS_BUILD_DIR
mkdir build && cd build
cmake ..
cmake --build . --config ${TRAVIS_BUILD_TYPE}
cmake --build . --config ${TRAVIS_BUILD_TYPE} --target install
