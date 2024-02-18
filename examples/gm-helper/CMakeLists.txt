include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/application.ixx
	src/application.cpp
	src/components.ixx
	src/selection.ixx
	src/textures.ixx
	src/rendering-stages.ixx
	src/rendering-stages.cpp
	src/ui.ixx
	config.xml
)
foreach( File ${ProjectSources} )
	string( REGEX REPLACE "(.*)/(.*)\\.(.*)" "\\1" subdir ${File} )
	string(REPLACE "/" "\\" subdir  ${subdir})
	source_group( ${subdir} FILES ${File} )
endforeach()
source_group( "resources" FILES config.xml )

add_executable( GMHelper ${ProjectSources} )
target_link_libraries( GMHelper Bembel ${EXTERNAL_LIBRARIES})
set_target_properties( GMHelper PROPERTIES FOLDER "examples" )
bembel_configure_application( GMHelper "examples")

