#ifndef GLFW_UTIL_H
#define GLFW_UTIL_H

#include "common.h"

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