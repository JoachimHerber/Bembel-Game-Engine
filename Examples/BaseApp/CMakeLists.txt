include_directories( "${EXTERNAL_INCLUDE_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources main.cpp BaseApp.h BaseApp.cpp )
source_group( Source FILES ${ProjectSources} )

add_executable( BaseApp ${ProjectSources} )
target_link_libraries( BaseApp Bembel ${EXTERNAL_LIBRARIES})
set_target_properties( BaseApp PROPERTIES FOLDER "Examples" )
bembel_configure_application( BaseApp "Examples/BaseApp/")

