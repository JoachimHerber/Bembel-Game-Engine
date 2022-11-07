cmake_minimum_required( VERSION 3.16 )
list( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )
include( cmake_utils )

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

project( BembelGameEngine )
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

bembel_find_external_dependencies()
include_directories( "${CMAKE_SOURCE_DIR}/src" )
include_directories( "${CMAKE_SOURCE_DIR}/dependencies" )
include_directories( "${CMAKE_SOURCE_DIR}/dependencies/glm" )
include_directories( "${CMAKE_SOURCE_DIR}/dependencies/json/include" )
include_directories( "${EXTERNAL_INCLUDE_DIRS}" )

set( CMAKE_CONFIGURAION_TYPES "Debug;Release" )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

bembel_conditional_add_sub_project(Input "examples/input" "a simple example that show how to handle input in the bembel game engine")
bembel_conditional_add_sub_project(rendering "examples/rendering" "rendering example application")
bembel_conditional_add_sub_project(Chess "examples/chess" "chess example application")
bembel_conditional_add_sub_project(GUI "examples/GUI" "GUI example application")

bembel_conditional_add_sub_project(FontConverter "tools/font-converter" "A tool for converting fonts")

