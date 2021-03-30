include_directories( "${EXTERNAL_INCLUDE_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/rendering-example.hpp
	src/rendering-example.cpp 
	src/camera-controle.hpp
	src/camera-controle.cpp 
)

foreach( File ${ProjectSources} )
	string( REGEX REPLACE "(.*)/(.*)\\.(.*)" "\\1" subdir ${File} )
	string(REPLACE "/" "\\" subdir  ${subdir})
	source_group( ${subdir} FILES ${File} )
endforeach()

add_executable( RenderingExample ${ProjectSources} )
target_link_libraries( RenderingExample Bembel ${EXTERNAL_LIBRARIES})
set_target_properties( RenderingExample PROPERTIES FOLDER "examples" )
bembel_configure_application( RenderingExample "examples")

