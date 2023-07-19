#include <iostream>
#include "utils_glfw.h"

// 参考 jpcy https://github.com/jpcy/bgfx-minimal-example.git
int main(int argc, char **argv) {
    // 初始化GLFW window
    GLFWwindow *window;
    int ret = glfw_init(&window);
    if(ret){
        return 1;
    }

    // 提前调用renderFrame，控制在当前线程初始化bgfx
    bgfx::renderFrame();
   
    // 初始化bgfx
    bgfx::Init init;
#if BX_PLATFORM_OSX
    init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
    init.platformData.nwh = glfwGetWin32Window(window);
#endif
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    init.resolution.width = (uint32_t)width;
    init.resolution.height = (uint32_t)height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    if (!bgfx::init(init)){
        return 1;
    }

    // 设置清屏色
    // ViewId _id, uint16_t _flags, uint32_t _rgba, float _depth, uint8_t _stencil
    bgfx::setViewClear(
        static_cast<uint16_t>(RenderView::MAIN), 
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
        0x303030ff, 
        1.0f, 
        0);

    // 对bgfx设置渲染宽高
    // ViewId _id, uint16_t _x, uint16_t _y, BackbufferRatio::Enum _ratio
    bgfx::setViewRect(
        static_cast<uint16_t>(RenderView::MAIN), 
        0, 
        0, 
        bgfx::BackbufferRatio::Equal);


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // glfw window 尺寸调整；对应bgfx进行重置宽高
        int oldWidth = width, oldHeight = height;
        glfwGetWindowSize(window, &width, &height);
        if (width != oldWidth || height != oldHeight) {
            bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
            bgfx::setViewRect(
                static_cast<uint16_t>(RenderView::MAIN), 
                0, 
                0, 
                bgfx::BackbufferRatio::Equal);
        }
        
        // 提交空指令到指定view，不执行几何渲染，确认没有其他draw call提交到该view
        bgfx::touch(static_cast<uint16_t>(RenderView::MAIN));
        
        // 输出debug信息
        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
        bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
        bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");

        // 设置debug flag为展示debug text，其他flag可以展示性能数据等
        bgfx::setDebug(BGFX_DEBUG_TEXT);
        
        // 单线程中：提交指令，执行帧渲染，跳转下一帧；多线程中切换内部buffer
        bgfx::frame();
    }
    bgfx::shutdown();
    glfwTerminate();
    return 0;
}
