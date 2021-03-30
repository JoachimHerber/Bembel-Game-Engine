include_directories( "${EXTERNAL_INCLUDE_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/gui-example.h 
	src/gui-example.cpp
)
source_group( "src" FILES ${ProjectSources} )

add_executable( GUIExample ${ProjectSources} )
target_link_libraries( GUIExample Bembel ${EXTERNAL_LIBRARIES})
set_target_properties( GUIExample PROPERTIES FOLDER "examples" )
bembel_configure_application( GUIExample "examples")

