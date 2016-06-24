cmake_minimum_required( VERSION 2.8 )
list( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )
include( cmake_utils )

project( BembelGameEngine )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

bembel_find_external_dependencies()

set( CMAKE_CONFIGURAION_TYPES "Debug;Release" )

include( "src/BembelBase/_SourceFiles.cmake" )
include( "src/BembelDataStructures/_SourceFiles.cmake" )
include( "src/BembelKernel/_SourceFiles.cmake" )
include( "src/BembelGraphics/_SourceFiles.cmake" )

include_directories( "${CMAKE_SOURCE_DIR}/src" )
include_directories( "${CMAKE_SOURCE_DIR}/dependences" )
include_directories( "${CMAKE_SOURCE_DIR}/dependences/glm" )
include_directories( "${EXTERNAL_INCLUDE_DIRS}" )

add_library( Bembel SHARED ${ProjectSources} )
target_link_libraries( Bembel ${EXTERNAL_LIBRARIES} )
set_target_properties( Bembel PROPERTIES OUTPUT_NAME "Bembel" )

set( BEMBEL_DISABLE_TEMPLATE_NEEDS_DLL_INTERFACE_WARNING ON CACHE BOOL "disables the 'Template needs DLL-Interface'-Warning." )

if( WIN32 )
	if( BEMBEL_DISABLE_TEMPLATE_NEEDS_DLL_INTERFACE_WARNING )
		add_definitions( "/wd4251" )
	endif( BEMBEL_DISABLE_TEMPLATE_NEEDS_DLL_INTERFACE_WARNING )
	
	get_target_property( _BUILD_TYPE Bembel TYPE )
	if( "${_BUILD_TYPE}" STREQUAL "SHARED_LIBRARY" OR "${_BUILD_TYPE}" STREQUAL "MODULE_LIBRARY" )
		set_target_properties( Bembel PROPERTIES COMPILE_FLAGS "-DTINYXML2_EXPORT -DBEMBEL_EXPORT" )
	else()
		set_target_properties( Bembel PROPERTIES COMPILE_FLAGS -DBEMBEL_STATIC )
	endif()
endif( WIN32 )

bembel_conditional_add_sub_project(BaseApp "Examples/BaseApp" "build base application")
bembel_conditional_add_sub_project(RenderingTest "Examples/RenderingTest" "build rendering test")