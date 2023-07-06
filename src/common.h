#ifndef COMMON_H
#define COMMON_H

/**
 * bgfx include
 *
 */
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>


/**
 * glfw include, glfw3native.h必须放在宏定义之后
 *
 */
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>



enum class RenderView : uint16_t {
    MAIN = 0,
    NO_CLEAR = 1,
    SWAP = 2
};

#endif