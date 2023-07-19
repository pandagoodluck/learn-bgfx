cmake_minimum_required(VERSION 3.6.0)

project(LEARN-BGFX)

set(CMAKE_CXX_STANDARD 14)
set(THIRD_PARTY ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# MACOS 宏定义改为从build-macos.sh脚本传入
# if (APPLE AND NOT IOS)
# set(MACOS TRUE)
# endif()

#--------------------------------------------------------------------
# BGFX 配置
#--------------------------------------------------------------------
# BX_CONFIG_DEBUG需要在最外层配置，否则影响bx/config.h编译
add_compile_definitions(BX_CONFIG_DEBUG=0)


#--------------------------------------------------------------------
# 基本编译参数配置
#--------------------------------------------------------------------
if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -ffast-math")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fomit-frame-pointer -fstrict-aliasing")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()


#--------------------------------------------------------------------
# 依赖库添加
#--------------------------------------------------------------------
add_subdirectory(${THIRD_PARTY}/bgfx_group)
add_subdirectory(${THIRD_PARTY}/glfw)


#--------------------------------------------------------------------
# 平台相关层编译配置
#--------------------------------------------------------------------
# if (APPLE)
#     file(
#         GLOB_RECURSE
#         ALL_SOURCE_FILES
#         ${ALL_SOURCE_FILES}
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/**.m
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/**.mm
#     )
#     # 需要放在glfw编译之后，glfw设置了c99，有冲突
#     add_compile_options(-x objective-c++)
#     set(THIRDPARTY_LIBRARIES ${THIRDPARTY_LIBRARIES} z)
# endif()


# --------------------------------------------------------------------
# src 文件编译
# --------------------------------------------------------------------
file(
    GLOB
    INSTALL_HEADERS
    ${INSTALL_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
)

include_directories(${INSTALL_HEADERS})
include_directories(${THIRD_PARTY}/bgfx_group/bgfx/include)
include_directories(${THIRD_PARTY}/bgfx_group/bimg/include)
include_directories(${THIRD_PARTY}/bgfx_group/bx/include)
include_directories(${THIRD_PARTY}/glfw/include)


set(MAIN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/example_01_helloworld.cpp)
add_executable(example_01_helloworld ${MAIN_SRC})
target_link_libraries(example_01_helloworld bgfx bx bimg glfw ${THIRDPARTY_LIBRARIES})

set(MAIN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/example_02_triangle.cpp)
add_executable(example_02_triangle ${MAIN_SRC})
target_link_libraries(example_02_triangle bgfx bx bimg glfw ${THIRDPARTY_LIBRARIES})