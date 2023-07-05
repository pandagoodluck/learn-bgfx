#!/bin/bash

MACOS_PLATFORM="OSX"
PROJECT_ROOT_PATH="$(cd "$(dirname "$0")"; pwd -P)"

function set_build_dir() {
    BUILD_DIR=$PROJECT_ROOT_PATH/build/${MACOS_PLATFORM}
    rm -rf $BUILD_DIR
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
}

function build(){
    cmake ${PROJECT_ROOT_PATH} \
        -G Xcode \
        -DMACOS:BOOL="ON" \
        -DCMAKE_OSX_DEPLOYMENT_TARGET="10.13" 

    if [ $? -ne 0 ]; then
        echo "cmake ${MACOS_PLATFORM} error\n"
        exit 1
    fi

    xcodebuild -scheme install -arch x86_64 -arch arm64 ONLY_ACTIVE_ARCH=NO -configuration Release build

    if [ $? -ne 0 ]; then
        echo "cmake build ${MACOS_PLATFORM} error\n"
        exit 1
    fi
}

function main() {
    
    set_build_dir

    build

   
}

main