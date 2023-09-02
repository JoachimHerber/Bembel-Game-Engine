
#find_package(Freetype REQUIRED)

#list( APPEND EXTERNAL_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIRS}" )
#list( APPEND EXTERNAL_LIBRARIES "${FREETYPE_LIBRARIES}" )

include_directories( "${EXTERNAL_INCLUDE_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/application.ixx
	src/application.cpp
)

foreach( File ${ProjectSources} )
	string( REGEX REPLACE "(.*)/(.*)\\.(.*)" "\\1" subdir ${File} )
	string(REPLACE "/" "\\" subdir  ${subdir})
	source_group( ${subdir} FILES ${File} )
endforeach()

add_executable( Editor ${ProjectSources} )
target_link_libraries( Editor Bembel ${EXTERNAL_LIBRARIES})
set_target_properties( Editor PROPERTIES FOLDER "tools" )
bembel_configure_application( Editor "tools")

