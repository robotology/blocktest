#!/bin/sh
set -e

# Install source deps

# Build  YARP
git clone https://github.com/robotology/yarp
cd yarp
git checkout ${YARP_BRANCH}
mkdir build
cd build
cmake -G"${TRAVIS_CMAKE_GENERATOR}" -DCREATE_GUIS:BOOL=OFF -DCREATE_LIB_MATH:BOOL=ON -DCMAKE_BUILD_TYPE=${TRAVIS_BUILD_TYPE} ..
cmake --build . --config ${TRAVIS_BUILD_TYPE} --target install
cd ../..


# Build, blocktest
cd $TRAVIS_BUILD_DIR
mkdir build && cd build
cmake ..
cmake --build . --config ${TRAVIS_BUILD_TYPE}
cmake --build . --config ${TRAVIS_BUILD_TYPE} --target install
