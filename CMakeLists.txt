cmake_minimum_required(VERSION 3.10)

# 查找Git
find_package(Git)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "Git version: ${GIT_VERSION}")
else()
    set(GIT_VERSION "unknown")
endif()

project(MTEditor
    VERSION ${GIT_VERSION}
    DESCRIPTION "C++ user interface library for rendering"
    LANGUAGES "C" "CXX"
)

# 设置 C++ 版本
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(ENGINE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(THIRD_PARTY_DIR "${ENGINE_ROOT_DIR}/3rdparty")

option(SAMPLES "Build samples of the library." OFF)

add_subdirectory(Editor)
add_subdirectory(Runtime)
