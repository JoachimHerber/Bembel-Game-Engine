include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/input-example.ixx
	src/input-example.cpp
)
source_group( "src" FILES ${ProjectSources} )

add_executable( InputExample ${ProjectSources} )
target_link_libraries( InputExample Bembel ${EXTERNAL_LIBRARIES})
set_target_properties( InputExample PROPERTIES FOLDER "examples" )
bembel_configure_application( InputExample "examples")

