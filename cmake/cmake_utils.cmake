macro( bembel_find_external_dependencies  )

	find_package(OpenGL REQUIRED)
	find_package(GLFW REQUIRED)
	find_package(glbinding REQUIRED)
	find_package(Bullet REQUIRED)

	list( APPEND EXTERNAL_INCLUDE_DIRS "${GLFW_INCLUDE_DIR}" )
	list( APPEND EXTERNAL_INCLUDE_DIRS "${GLBINDING_INCLUDE_DIR}" )
	list( APPEND EXTERNAL_INCLUDE_DIRS "${BULLET_INCLUDE_DIR}" )

	list( APPEND EXTERNAL_LIBRARIES "${OPENGL_LIBRARIES}" )
	list( APPEND EXTERNAL_LIBRARIES "${GLFW_LIBRARY}" )
	list( APPEND EXTERNAL_LIBRARIES "${GLBINDING_LIBRARIES}" )
	list( APPEND EXTERNAL_LIBRARIES "${BULLET_LIBRARIES}" )
	
	get_filename_component(GLBINDING_BINARY_DIR  "${glbinding_DIR}" PATH )
	
	message(STATUS "glbinding_DIR='${glbinding_DIR}'")

	list( APPEND  DLL_DIRS "${glbinding_DIR}" )
endmacro( bembel_find_external_dependencies )

macro( bembel_conditional_add_sub_project _PACKAGE_NAME _DIRECTORY _DISCRIPTION )
	string( TOUPPER ${_PACKAGE_NAME} _PACKAGE_NAME_UPPER )
	set( BEMBEL_BUILD_${_PACKAGE_NAME_UPPER} ON CACHE BOOL ${_DISCRIPTION} )
	
	if( BEMBEL_BUILD_${_PACKAGE_NAME_UPPER} )
		add_subdirectory( ${_DIRECTORY} )
	endif()
endmacro()

macro( bembel_configure_application _PACKAGE_NAME _WORK_FOLDER )
	list( APPEND DLL_DIRS "${CMAKE_BINARY_DIR}/bin/Debug" )
	list( APPEND DLL_DIRS "${CMAKE_BINARY_DIR}/bin/Release" )
	
	string( TOUPPER ${_PACKAGE_NAME} _PACKAGE_NAME_UPPER )

	set( ${_PACKAGE_NAME_UPPER}_OUTPUT_NAME ${_PACKAGE_NAME} CACHE INTERNAL "" FORCE )
	
	set_target_properties( ${_PACKAGE_NAME} PROPERTIES OUTPUT_NAME_DEBUG	"${${_PACKAGE_NAME_UPPER}_OUTPUT_NAME}D" )
	set_target_properties( ${_PACKAGE_NAME} PROPERTIES OUTPUT_NAME_RELEASE	"${${_PACKAGE_NAME_UPPER}_OUTPUT_NAME}" )
	set_target_properties( ${_PACKAGE_NAME} PROPERTIES OUTPUT_NAME			"${${_PACKAGE_NAME_UPPER}_OUTPUT_NAME}" )
	set_target_properties( ${_PACKAGE_NAME} PROPERTIES COMPILE_FLAGS -DTINYXML2_IMPORT )
	set_property(TARGET ${_PACKAGE_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${_WORK_FOLDER}")
	
	if( MSVC )
		#if we're usign MSVC, we set up a *.vcproj.user file
		if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
			set( _CONFIG_NAME "x64" )
		else()
			set( _CONFIG_NAME "Win32" )
		endif()
		
	endif( MSVC )	
endmacro()

macro(bembel_add_msvc_precompiled_header PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
endmacro(bembel_add_msvc_precompiled_header)