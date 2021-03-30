set( DEPENDENCIES_SRC_FILES	../dependencies/tinyxml2/tinyxml2.cpp)

add_library( Bembel SHARED "bembel/pch.hpp")
		
target_link_libraries( Bembel glbinding::glbinding )
target_link_libraries( Bembel ${EXTERNAL_LIBRARIES} )
set_target_properties( Bembel PROPERTIES OUTPUT_NAME "Bembel" )

add_subdirectory(bembel)
get_target_property( BEMBEL_SRC_FILES Bembel SOURCES )
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/bembel PREFIX "bembel" FILES ${BEMBEL_SRC_FILES})

target_sources(Bembel
	PUBLIC
		../dependencies/tinyxml2/tinyxml2.h
		../dependencies/lodepng/lodepng.h
	PRIVATE
		../dependencies/tinyxml2/tinyxml2.cpp
		../dependencies/lodepng/lodepng.cpp
)
source_group(dependencies PREFIX "bembel" FILES 
	../dependencies/tinyxml2/tinyxml2.h
	../dependencies/tinyxml2/tinyxml2.cpp
	../dependencies/lodepng/lodepng.h
	../dependencies/lodepng/lodepng.cpp
)

if( WIN32 )
	get_target_property( _BUILD_TYPE Bembel TYPE )
	if( "${_BUILD_TYPE}" STREQUAL "SHARED_LIBRARY" OR "${_BUILD_TYPE}" STREQUAL "MODULE_LIBRARY" )
		set_target_properties( Bembel PROPERTIES COMPILE_FLAGS "-DTINYXML2_EXPORT -DBEMBEL_EXPORT" )
	else()
		set_target_properties( Bembel PROPERTIES COMPILE_FLAGS -DBEMBEL_STATIC )
	endif()
endif( WIN32 )
target_precompile_headers(Bembel PRIVATE "bembel/pch.hpp")

set( BEMBEL_DISABLE_TEMPLATE_NEEDS_DLL_INTERFACE_WARNING OFF CACHE BOOL "disables the 'Template needs DLL-Interface'-Warning." )
if( WIN32 )
	if( BEMBEL_DISABLE_TEMPLATE_NEEDS_DLL_INTERFACE_WARNING )
			add_definitions( "/wd4251" )
	endif( BEMBEL_DISABLE_TEMPLATE_NEEDS_DLL_INTERFACE_WARNING )
endif( WIN32 )

