
#include <iostream>
#include "utils_glfw.h"
#include "utils_bgfx.h"

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


// vertex stream expects _abgr
// vertex stream in they way GPU expects.
// https://github.com/bkaradzic/bgfx/issues/1090

// bgfx中，坐标顶点：
// FBO的纹理坐标顶点：win & mac & iOS & android ：左下角
// 上屏绘制坐标顶点：win & mac & iOS ：左下角；  android：左上角
static PosColorVertex triangleVertices[] =
{
        {0.0f,   0.5f,   0.0f,   0xffff0000},
        {0.25f,  0.0f,   0.0f,   0xff0000ff},
        {-0.25f, 0.0f,   0.0f,   0xff00ff00}
};


//顶点绘制顺序
static const uint16_t triangleTriList[] =
{
    1, 2, 0 
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
        printf("bgfx init error\n") ;
        return 1;
    }
    printf("bgfx init suc\n");

    /***************************** BGFX Draw Prepare *********************************/
    // 创建VertexBuffer和IndexBuffer
    PosColorVertex::init();
    m_vbh = bgfx::createVertexBuffer(
        // Make reference to data to pass to bgfx. Unlike bgfx::alloc, this call doesn’t allocate memory for data. 
        // It just copies the _data pointer. You can pass ReleaseFn function pointer to release this memory after 
        // it’s consumed, otherwise you must make sure _data is available for at least 2 bgfx::frame calls. 
        // ReleaseFn function must be able to be called from any thread.
        bgfx::makeRef(triangleVertices, sizeof(triangleVertices)),
        PosColorVertex::ms_decl);

    m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(triangleTriList, sizeof(triangleTriList)));

    // 加载shader，创建program
    bgfx::ShaderHandle vsh = loadShader("../../../shaders/output/metal/vs_triangle.bin");
    bgfx::ShaderHandle fsh = loadShader("../../../shaders/output/metal/fs_triangle.bin");
    bgfx::ProgramHandle m_program = bgfx::createProgram(vsh, fsh, true);
    if (bgfx::isValid(m_program)){
        printf("bgfx::createProgram suc\n");
    }
    else{
        printf("bgfx::createProgram fail\n");
    }


    /***************************** BGFX Draw *********************************/
    // 设置清屏色
    // ViewId _id, uint16_t _flags, uint32_t _rgba, float _depth, uint8_t _stencil
    // RGBA on little endian when passed as uint32_t
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

        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(m_ibh);

        // Set render states.
        bgfx::setState(
            BGFX_STATE_WRITE_R | 
            BGFX_STATE_WRITE_G | 
            BGFX_STATE_WRITE_B | 
            BGFX_STATE_WRITE_A);

        // 提交渲染数据
        bgfx::submit(static_cast<uint16_t>(RenderView::MAIN), m_program);

        // 单线程中：提交指令，执行帧渲染，跳转下一帧；多线程中切换内部buffer
        bgfx::frame();
    } 
    
    //销毁bgfx资源
    bgfx::destroy(m_ibh);
    bgfx::destroy(m_vbh);
    bgfx::destroy(m_program);

    //销毁bgfx
    bgfx::shutdown();

    glfwTerminate();
    return 0;
}
