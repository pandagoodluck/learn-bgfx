cmake_minimum_required(VERSION 3.6.0)

project(LEARN-BGFX)

set(CMAKE_CXX_STANDARD 11)
set(THIRD_PARTY ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# MACOS 宏定义改为从build-macos.sh脚本传入
# if (APPLE AND NOT IOS)
#     set(MACOS TRUE)
# endif()

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8 /std:c++11 /Zc:__cplusplus")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MD /Z7 /GL")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /Zi /GL")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Z7")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -ffast-math")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fomit-frame-pointer -fstrict-aliasing")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -s")
endif()

add_subdirectory(${THIRD_PARTY}/bgfx_group)
add_subdirectory(${THIRD_PARTY}/glfw)

include_directories(${ALL_HEADER_DIRECTORIES})
include_directories(${THIRD_PARTY}/bgfx_group/bgfx/include)
include_directories(${THIRD_PARTY}/bgfx_group/bimg/include)
include_directories(${THIRD_PARTY}/bgfx_group/bx/include)
include_directories(${THIRD_PARTY}/glfw/include)

set(MAIN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/example_01_helloworld.cpp)

add_executable(main ${MAIN_SRC})
target_link_libraries(main ${PROJECT_NAME} bgfx bx bimg glfw)

