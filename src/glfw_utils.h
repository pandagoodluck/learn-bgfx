#ifndef GLFW_UTIL_H
#define GLFW_UTIL_H

/**
 * glfw include, glfw3native.h必须放在上述宏定义之后
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

static void glfw_errorCallback(int error, const char *description) {
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static int glfw_init(GLFWwindow **window) {
    // Create a GLFW window without an OpenGL context.
    glfwSetErrorCallback(glfw_errorCallback);
    if (!glfwInit()){
        return 1;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    *window = glfwCreateWindow(1024, 768, "helloworld", nullptr, nullptr);
    if (!window)
        return 1;
    // glfwSetKeyCallback(*window, glfw_keyCallback);

    return 0;
}

#endif