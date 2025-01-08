cmake_minimum_required(VERSION 3.10)

# 查找并添加源文件
file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE SOURCE_FILES1 "*.hpp")


# 将源文件按目录结构组织
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES} ${SOURCE_FILES1})

set(RmlUi_Includes "E:/MTEngine/Editor/3dparty/RmlUi/include")
set(RmlUi_Libs "E:/MTEngine/Editor/3dparty/RmlUi/lib")

set(PartyIncludes "D:/3dparty/glfw3/include")
set(PartyLibs "D:/3dparty/glfw3/lib" "D:/3dparty/freetype/install/lib")


set(SNIRender_DIR "D:/Demo/cmakedemo/build/install/lib/cmake/SNIRender/")
find_package(SNIRender REQUIRED)

link_directories( ${RmlUi_Libs} ${PartyLibs})

# 生成可执行文件
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SOURCE_FILES1})


# 寻找并链接第三方库
#set(RmlUi_DIR "E:/MTEngine/Editor/3dparty/RmlUi/lib/cmake/RmlUi")
#find_package(RmlUi REQUIRED)
message(1111111111 ${RmlUi_INCLUDE_DIRS})
message(222222222 ${RmlUi_LIBRARY_DIRS})

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)


# 添加头文件搜索路径
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${RmlUi_Includes}  ${PartyIncludes})
target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 freetype glfw3 rmlui rmlui_debugger dwmapi shlwapi
    Qt5::Core Qt5::Gui Qt5::Widgets)

# 在 Debug 模式下添加后缀 "d"
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

# 安装可执行文件
install(TARGETS ${PROJECT_NAME}
    DESTINATION build/install
)

### precompile
# set global vari used by precompile
set(PICCOLO_EDITOR_HEADS "${HEADER_FILES}" PARENT_SCOPE)








