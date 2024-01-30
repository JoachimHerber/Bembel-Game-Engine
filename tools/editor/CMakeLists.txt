
#find_package(Freetype REQUIRED)

#list( APPEND EXTERNAL_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIRS}" )
#list( APPEND EXTERNAL_LIBRARIES "${FREETYPE_LIBRARIES}" )

include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/application.ixx
	src/application.cpp
	src/textures.ixx
	src/textures.cpp
	src/materials.ixx
	src/materials.cpp
	src/meshes.ixx
	src/meshes.cpp
	src/models.ixx
	src/models.cpp
	src/scenes.ixx
	src/scenes.cpp
	
	config.xml
)

foreach( File ${ProjectSources} )
	string( REGEX REPLACE "(.*)/(.*)\\.(.*)" "\\1" subdir ${File} )
	string(REPLACE "/" "\\" subdir  ${subdir})
	source_group( ${subdir} FILES ${File} )
endforeach()

source_group( "resources" FILES config.xml )

add_executable( Editor ${ProjectSources} )
target_link_libraries( Editor Bembel ${EXTERNAL_LIBRARIES})
target_link_libraries( Editor assimp )
set_target_properties( Editor PROPERTIES FOLDER "tools" )
bembel_configure_application( Editor "tools")

