#ifndef BGFX_UTILS_H
#define BGFX_UTILS_H

#include <fstream>

#include "common.h"

static bgfx::ShaderHandle loadShader(const char* _name) {
    char* data = new char[2048];
    std::ifstream file;
    size_t fileSize;
    file.open(_name);
    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        file.read(data, fileSize);
        file.close();
    }
    const bgfx::Memory* mem = bgfx::copy(data, fileSize + 1);
    mem->data[mem->size - 1] = '\0';
    bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, _name);
    return handle;
}


#endif