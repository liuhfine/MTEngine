cmake_minimum_required(VERSION 3.10)

add_subdirectory(source)

add_subdirectory(meta_parser)

set(CODEGEN_TARGET "PiccoloPreCompile")
include(precompile/precompile.cmake)
set_target_properties("${CODEGEN_TARGET}" PROPERTIES FOLDER "Engine" )

add_dependencies(MTEditor "${CODEGEN_TARGET}")
add_dependencies("${CODEGEN_TARGET}" "PiccoloParser")




