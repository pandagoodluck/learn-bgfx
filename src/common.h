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

/**
 * 通用结构体定义
 *
 */
enum class RenderView : uint16_t {
    MAIN = 0,
    NO_CLEAR = 1,
    SWAP = 2
};
struct PosColorVertex {
    // 顶点坐标
    float m_x;
    float m_y;
    float m_z;
    // 顶点颜色
    uint32_t m_abgr;
    // 顶点描述对象
    static bgfx::VertexLayout ms_decl;

    static void init() {
        ms_decl
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            // When normalized is set to true, AttribType::Uint8 value in range 0-255 will be in range 0.0-1.0 in vertex shader.
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    };
};
bgfx::VertexLayout PosColorVertex::ms_decl;

#endif