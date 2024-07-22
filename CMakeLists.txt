cmake_minimum_required(VERSION 3.10)
project(MTEditor
	VERSION "6.0"
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








