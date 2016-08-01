macro( bembel_find_external_dependencies  )

	find_package(OpenGL REQUIRED)
	find_package(GLFW REQUIRED)
	find_package(glbinding REQUIRED)
	find_package(Assimp REQUIRED)

	list( APPEND EXTERNAL_INCLUDE_DIRS "${GLFW_INCLUDE_DIR}" )
	list( APPEND EXTERNAL_INCLUDE_DIRS "${GLBINDING_INCLUDE_DIR}" )

	list( APPEND EXTERNAL_LIBRARIES "${OPENGL_LIBRARIES}" )
	list( APPEND EXTERNAL_LIBRARIES "${GLFW_LIBRARY}" )
	list( APPEND EXTERNAL_LIBRARIES "${GLBINDING_LIBRARIES}" )
	
	
	get_filename_component(GLBINDING_BINARY_DIR  "${GLBINDING_BINARY_DEBUG}" PATH )
	
	list( APPEND  DLL_DIRS "${GLBINDING_BINARY_DIR}" )

endmacro( bembel_find_external_dependencies )

macro( bembel_conditional_add_sub_project _PACKAGE_NAME _DIRECTORY _DISCRIPTION )
	string( TOUPPER ${_PACKAGE_NAME} _PACKAGE_NAME_UPPER )
	set( BEMBEL_BUILD_${_PACKAGE_NAME_UPPER} ON CACHE BOOL ${_DISCRIPTION} )
	
	if( BEMBEL_BUILD_${_PACKAGE_NAME_UPPER} )
		add_subdirectory( ${_DIRECTORY} )
	endif()
endmacro()

macro( bembel_configure_application _PACKAGE_NAME _WORK_FOLDER )
	list( APPEND  DLL_DIRS "${CMAKE_BINARY_DIR}/bin/Debug" )
	list( APPEND  DLL_DIRS "${CMAKE_BINARY_DIR}/bin/Release" )
	
	string( TOUPPER ${_PACKAGE_NAME} _PACKAGE_NAME_UPPER )

	set( ${_PACKAGE_NAME_UPPER}_OUTPUT_NAME ${_PACKAGE_NAME} CACHE INTERNAL "" FORCE )
	
	set_target_properties( ${_PACKAGE_NAME} PROPERTIES OUTPUT_NAME_DEBUG	"${${_PACKAGE_NAME_UPPER}_OUTPUT_NAME}D" )
	set_target_properties( ${_PACKAGE_NAME} PROPERTIES OUTPUT_NAME_RELEASE	"${${_PACKAGE_NAME_UPPER}_OUTPUT_NAME}" )
	set_target_properties( ${_PACKAGE_NAME} PROPERTIES OUTPUT_NAME			"${${_PACKAGE_NAME_UPPER}_OUTPUT_NAME}" )
	set_target_properties( ${_PACKAGE_NAME} PROPERTIES COMPILE_FLAGS -DTINYXML2_IMPORT )
	
	if( MSVC )
		#if we're usign MSVC, we set up a *.vcproj.user file
		if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
			set( _CONFIG_NAME "x64" )
		else()
			set( _CONFIG_NAME "Win32" )
		endif()
		
		set( VCPROJUSER_PROTO_FILE_NAME "VS.vcxproj.user_proto" )
		find_file( VCPROJUSER_PROTO_FILE "${VCPROJUSER_PROTO_FILE_NAME}" ${CMAKE_MODULE_PATH} )
		set( VCPROJUSER_PROTO_FILE ${VCPROJUSER_PROTO_FILE} CACHE INTERNAL "" )
		if( VCPROJUSER_PROTO_FILE )
			set( _WORK_DIR "${CMAKE_SOURCE_DIR}/${_WORK_FOLDER}" )
			
			set( _ENVIRONMENT "" )
			if( DLL_DIRS )
				set( _ENVIRONMENT "PATH=${DLL_DIRS};%PATH%&#x0A;" )
			endif()
				
			configure_file(
				${VCPROJUSER_PROTO_FILE}
				${CMAKE_CURRENT_BINARY_DIR}/${_PACKAGE_NAME}.vcxproj.user
				@ONLY
			)
			set( ${_PACKAGE_NAME_UPPER}_TARGET_MSVC_PROJECT "${CMAKE_CURRENT_BINARY_DIR}/${_PACKAGE_NAME}.vcxproj" CACHE INTERNAL "" FORCE )
		endif( VCPROJUSER_PROTO_FILE )
	endif( MSVC )	
	
	
endmacro()