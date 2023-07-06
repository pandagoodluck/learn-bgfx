# bgfx.cmake - bgfx building in cmake
# Written in 2017 by Joshua Brookover <joshua.al.brookover@gmail.com>

# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty.

# You should have received a copy of the CC0 Public Domain Dedication along with
# this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

if(TARGET tinyexr)
    return()
endif()

file(
    GLOB
    TINYEXR_SOURCES
    ${BIMG_DIR}/3rdparty/tinyexr/deps/miniz/*.c
    ${BIMG_DIR}/3rdparty/tinyexr/deps/miniz/*.h
    ${BIMG_DIR}/3rdparty/tinyexr/*.h
)

add_library(tinyexr STATIC ${TINYEXR_SOURCES})
target_include_directories(tinyexr PUBLIC
    $<BUILD_INTERFACE:${BIMG_DIR}/3rdparty>
    $<BUILD_INTERFACE:${BIMG_DIR}/3rdparty/tinyexr>
    $<BUILD_INTERFACE:${BIMG_DIR}/3rdparty/tinyexr/deps/miniz>)
set_target_properties(tinyexr PROPERTIES FOLDER "bgfx/3rdparty")
target_link_libraries(tinyexr PUBLIC bx)

if(BGFX_INSTALL)
    include(GNUInstallDirs)
    install(
        TARGETS tinyexr
        EXPORT bgfx-config
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()