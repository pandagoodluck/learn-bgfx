#!/bin/bash

MACOS_PLATFORM="OSX"
PROJECT_ROOT_PATH="$(cd "$(dirname "$0")"; pwd -P)"

function set_build_dir() {
    BUILD_DIR=$PROJECT_ROOT_PATH/build/${MACOS_PLATFORM}
    rm -rf $BUILD_DIR
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
}

function main() {
    
    set_build_dir

    cmake ${PROJECT_ROOT_PATH} \
        -G Xcode \
        -DMACOS:BOOL="ON" \
        -DCMAKE_OSX_DEPLOYMENT_TARGET="10.13" 
}

main