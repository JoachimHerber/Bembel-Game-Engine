include_directories( "${EXTERNAL_INCLUDE_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/RenderingTest.h 
	src/RenderingTest.cpp 
	src/SimpleGeometryRenderer.h 
	src/SimpleGeometryRenderer.cpp 
	src/SimpleGeometryComponent.h 
	src/SimpleGeometryComponent.cpp 
	src/CameraControle.h 
	src/CameraControle.cpp)
source_group( "src" FILES ${ProjectSources} )

add_executable( RenderingTest ${ProjectSources} )
target_link_libraries( RenderingTest Bembel ${EXTERNAL_LIBRARIES})
set_target_properties( RenderingTest PROPERTIES FOLDER "Examples" )
bembel_configure_application( RenderingTest "Examples/RenderingTest")

