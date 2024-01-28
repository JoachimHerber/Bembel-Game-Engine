add_library( Bembel STATIC)
include_directories( "../dependencies/bullet3/src" )
include_directories( "../dependencies/imgui" )

target_link_libraries( Bembel glfw )
target_link_libraries( Bembel glbinding::glbinding )
target_link_libraries( Bembel BulletDynamics BulletCollision BulletSoftBody LinearMath )
target_link_libraries( Bembel ${EXTERNAL_LIBRARIES} )
set_target_properties( Bembel PROPERTIES OUTPUT_NAME "Bembel" )

add_subdirectory(bembel)
get_target_property( BEMBEL_SRC_FILES Bembel SOURCES )
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/bembel PREFIX "bembel" FILES ${BEMBEL_SRC_FILES})

target_sources(Bembel
    PUBLIC
        ../dependencies/tinyxml2/tinyxml2.h
        ../dependencies/lodepng/lodepng.h
        ../dependencies/glm.natvis
    PRIVATE
        ../dependencies/tinyxml2/tinyxml2.cpp
        ../dependencies/lodepng/lodepng.cpp
        ../dependencies/imgui/imgui.h
        ../dependencies/imgui/imgui.cpp
        ../dependencies/imgui/imgui_demo.cpp
        ../dependencies/imgui/imgui_draw.cpp
        ../dependencies/imgui/imgui_tables.cpp
        ../dependencies/imgui/imgui_widgets.cpp
        ../dependencies/imgui/backends/imgui_impl_opengl3.h
        ../dependencies/imgui/backends/imgui_impl_opengl3.cpp
        ../dependencies/imgui/backends/imgui_impl_glfw.h
        ../dependencies/imgui/backends/imgui_impl_glfw.cpp
)
source_group(dependencies PREFIX "bembel" FILES 
    ../dependencies/tinyxml2/tinyxml2.h
    ../dependencies/tinyxml2/tinyxml2.cpp
    ../dependencies/lodepng/lodepng.h
    ../dependencies/lodepng/lodepng.cpp
    ../dependencies/glm.natvis
)
source_group(dependencies/imgui PREFIX "bembel" FILES 
        ../dependencies/imgui/imgui.h
        ../dependencies/imgui/imgui.cpp
        ../dependencies/imgui/imgui_demo.cpp
        ../dependencies/imgui/imgui_draw.cpp
        ../dependencies/imgui/imgui_tables.cpp
        ../dependencies/imgui/imgui_widgets.cpp
        ../dependencies/imgui/backends/imgui_impl_opengl3.h
        ../dependencies/imgui/backends/imgui_impl_opengl3.cpp
        ../dependencies/imgui/backends/imgui_impl_glfw.h
        ../dependencies/imgui/backends/imgui_impl_glfw.cpp
)
