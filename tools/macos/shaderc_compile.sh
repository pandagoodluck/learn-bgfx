#!/bin/bash

# 使用方法
# sh shaderc_compile.sh example_02_trangle

ROOT_DIR=$(cd "$(dirname "$0")/../..";pwd -P)
BGFX_SRC_DIR="$ROOT_DIR/third_party/bgfx_group/bgfx/src"
SHADERS_DIR="$ROOT_DIR/shaders"
OUTPUT_DIR="$ROOT_DIR/shaders/output"
OUTPUT_METAL_DIR="$OUTPUT_DIR/metal"
OUTPUT_GLSL_DIR="$OUTPUT_DIR/glsl"

rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR
mkdir -p $OUTPUT_METAL_DIR
mkdir -p $OUTPUT_GLSL_DIR


if [[ "$1" = "" ]]; then
    echo "please input shader directory"
    exit 1
fi

for item in $(find $SHADERS_DIR -mindepth 2 -maxdepth 2 -type f)
do
    if [[ $item = *"$1"* ]]; then
        path_array=($(echo $item | tr '/' ' '))
        filename=${path_array[ $((${#path_array[@]}-1)) ]}
        name=$(echo $filename | awk '{split($0,arr,"."); print arr[1]}')
        if [[ $name = "fs"* ]]; then
            echo "start compiling fragment shader: $filename"
            ./shaderc -i $BGFX_SRC_DIR -f $item -o "$OUTPUT_METAL_DIR/$name.bin" --type f --platform osx -p "metal"    
            ./shaderc -i $BGFX_SRC_DIR -f $item -o "$OUTPUT_GLSL_DIR/$name.bin" --type f --platform osx -p "120"                
            echo "compile done"
        elif [[ $name = "vs"* ]]; then
            echo "start compiling vertex shader: $filename"
            ./shaderc -i $BGFX_SRC_DIR -f $item -o "$OUTPUT_METAL_DIR/$name.bin" --type v --platform osx -p "metal"   
            ./shaderc -i $BGFX_SRC_DIR -f $item -o "$OUTPUT_GLSL_DIR/$name.bin" --type v --platform osx -p "120"                                 
            echo "compile done"
        fi  
    fi
done



# --platform <platform>     Target platform.
#     android
#     asm.js
#     ios
#     linux
#     orbis
#     osx
#     windows
# -p, --profile <profile>   Shader model. Defaults to GLSL.

#     100_es               OpenGL ES Shading Language / WebGL (ESSL)
#     300_es
#     310_es
#     320_es

#     s_3_0                High-Level Shading Language (HLSL)
#     s_4_0
#     s_5_0

#     metal                Metal Shading Language (MSL)

#     pssl                 PlayStation Shader Language (PSSL)

#     spirv                Standard Portable Intermediate Representation - V (SPIR-V)
#     spirv10-10
#     spirv13-11
#     spirv14-11
#     spirv15-12
#     spirv16-13

#     120                  OpenGL Shading Language (GLSL)
#     130
#     140
#     150
#     330
#     400
#     410
#     420
#     430
#     440
