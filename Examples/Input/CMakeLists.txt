include_directories( "${EXTERNAL_INCLUDE_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/InputExample.h 
	src/InputExample.cpp
)
source_group( "src" FILES ${ProjectSources} )

add_executable( InputExample ${ProjectSources} )
target_link_libraries( InputExample Bembel ${EXTERNAL_LIBRARIES})
set_target_properties( InputExample PROPERTIES FOLDER "Examples" )
bembel_configure_application( InputExample "Examples/Chess")

