
find_package(freetype2 REQUIRED)

list( APPEND EXTERNAL_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIRS}" )
list( APPEND EXTERNAL_LIBRARIES "${FREETYPE_LIBRARIES}" )

include_directories( "${EXTERNAL_INCLUDE_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/application.hpp 
	src/application.cpp 
	src/font-converter.hpp
	src/font-converter.cpp 
	src/font-family.hpp
	src/font-family.cpp 
	src/font-view.hpp
	src/font-view.cpp 
	src/glyph.hpp
	src/glyph.cpp 
	src/glyph-texture-atlas.hpp
	src/glyph-texture-atlas.cpp 
	src/preview-renderer.hpp
	src/preview-renderer.cpp 
	src/texture-atlas-node.hpp
	src/texture-atlas-node.cpp 
)

foreach( File ${ProjectSources} )
	string( REGEX REPLACE "(.*)/(.*)\\.(.*)" "\\1" subdir ${File} )
	string(REPLACE "/" "\\" subdir  ${subdir})
	source_group( ${subdir} FILES ${File} )
endforeach()

add_executable( FontConverter ${ProjectSources} )
target_link_libraries( FontConverter Bembel ${EXTERNAL_LIBRARIES})
set_target_properties( FontConverter PROPERTIES FOLDER "tools" )
bembel_configure_application( FontConverter "tools")

