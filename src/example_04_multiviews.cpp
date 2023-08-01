
#include <iostream>

#include "utils_bgfx.h"
#include "utils_glfw.h"


static PosColorVertex triangleVertices[] =
    {
        {0.0f, 0.5f, 0.0f, 0xffff0000},
        {0.25f, 0.0f, 0.0f, 0xff0000ff},
        {-0.25f, 0.0f, 0.0f, 0xff00ff00}};

// 顶点绘制顺序
static const uint16_t triangleTriList[] =
    {
        2, 1, 0
    };

bgfx::VertexBufferHandle m_vbh;
bgfx::IndexBufferHandle m_ibh;
bgfx::ProgramHandle m_program;

int main(int argc, char **argv) {
    /***************************** GLFW *********************************/
    // 初始化GLFW window
    GLFWwindow *window;
    int ret = glfw_init(&window);
    if (ret) {
        return 1;
    }

    /***************************** BGFX Init *********************************/
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
    init.type = bgfx::RendererType::Metal;
    if (!bgfx::init(init)) {
        printf("bgfx init error\n");
        return 1;
    }
    printf("bgfx init suc\n");

    /***************************** BGFX Draw Prepare *********************************/
    // 创建VertexBuffer和IndexBuffer
    PosColorVertex::init();
    m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(triangleVertices, sizeof(triangleVertices)),PosColorVertex::ms_decl);
    m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(triangleTriList, sizeof(triangleTriList)));

    // 加载shader，创建program
    bgfx::ShaderHandle vsh = loadShader("../../../shaders/output/metal/vs_triangle.bin");
    bgfx::ShaderHandle fsh = loadShader("../../../shaders/output/metal/fs_triangle.bin");
    bgfx::ProgramHandle m_program = bgfx::createProgram(vsh, fsh, true);
    if (bgfx::isValid(m_program)) {
        printf("bgfx::createProgram suc\n");
    } else {
        printf("bgfx::createProgram fail\n");
    }

    /***************************** BGFX Draw *********************************/
    // 设置清屏色
    bgfx::setViewClear(static_cast<uint16_t>(RenderView::MAIN),     BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00f000ff, 1.0f, 0);
    bgfx::setViewClear(static_cast<uint16_t>(RenderView::VIEW_1),   BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x0000f0ff, 1.0f, 0);
    bgfx::setViewClear(static_cast<uint16_t>(RenderView::VIEW_2),   BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

    // 对bgfx设置渲染宽高
    float quarter_width = 0.25 * width;
    bgfx::setViewRect(static_cast<uint16_t>(RenderView::MAIN),  0, 0, bgfx::BackbufferRatio::Quarter);
    bgfx::setViewRect(static_cast<uint16_t>(RenderView::VIEW_1), quarter_width - 20, 50, bgfx::BackbufferRatio::Quarter);
    bgfx::setViewRect(static_cast<uint16_t>(RenderView::VIEW_2), quarter_width * 2 - 40, 100, bgfx::BackbufferRatio::Quarter);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // glfw window 尺寸调整；对应bgfx进行重置宽高
        int oldWidth = width, oldHeight = height;
        glfwGetWindowSize(window, &width, &height);
        if (width != oldWidth || height != oldHeight) {
            printf("window w : %i, h : %i\n",width, height);
            bgfx::setViewRect(static_cast<uint16_t>(RenderView::MAIN), 0, 0, bgfx::BackbufferRatio::Quarter);
            bgfx::setViewRect(static_cast<uint16_t>(RenderView::VIEW_1), quarter_width - 20 , 50, bgfx::BackbufferRatio::Quarter);
            bgfx::setViewRect(static_cast<uint16_t>(RenderView::VIEW_2), quarter_width * 2 - 40, 100, bgfx::BackbufferRatio::Quarter);
        }

        // 经过测试，默认最外层绘制view_id 最大的，可能按照view id的大小顺序来绘制。
        
        /**********VIEW_1*************/
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);
        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(static_cast<uint16_t>(RenderView::VIEW_1), m_program);
        bgfx::touch(static_cast<uint16_t>(RenderView::VIEW_1));

        /**********MAIN*************/
        bgfx::touch(static_cast<uint16_t>(RenderView::MAIN));
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);
        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(static_cast<uint16_t>(RenderView::MAIN), m_program);
        bgfx::touch(static_cast<uint16_t>(RenderView::MAIN));


        /**********VIEW_2*************/
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);
        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(static_cast<uint16_t>(RenderView::VIEW_2), m_program);
        bgfx::touch(static_cast<uint16_t>(RenderView::VIEW_2));

        /**********背景VIEW*************/
        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
        bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
        bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
        bgfx::setDebug(BGFX_DEBUG_STATS);
        // 单线程中：提交指令，执行帧渲染，跳转下一帧；多线程中切换内部buffer
        bgfx::frame();
    }

    // 销毁bgfx资源
    bgfx::destroy(m_ibh);
    bgfx::destroy(m_vbh);
    bgfx::destroy(m_program);

    // 销毁bgfx
    bgfx::shutdown();

    glfwTerminate();
    return 0;
}
