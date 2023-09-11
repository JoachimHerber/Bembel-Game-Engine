macro( bembel_find_external_dependencies  )

	option(GLFW_INSTALL "" OFF)
	add_subdirectory( "dependencies/glfw")

	option(OPTION_BUILD_TOOLS "" OFF)
	option(OPTION_BUILD_EXAMPLES "" OFF)
	add_subdirectory( "dependencies/glbinding")
	
	option(BUILD_CPU_DEMOS "" OFF)
	option(USE_GLUT "" OFF)
	option(BUILD_BULLET2_DEMOS "" OFF)
	option(BUILD_OPENGL3_DEMOS "" OFF)
	option(BUILD_EXTRAS "" OFF)
	option(BUILD_UNIT_TESTS "" OFF)
	option(BUILD_ENET "" OFF)
	option(BUILD_CLSOCKET "" OFF)
	add_subdirectory( "dependencies/bullet3")
	
	option(BUILD_CLSOCKET "" OFF)
	add_subdirectory( "dependencies/freetype")
	
	set_target_properties( glfw                  PROPERTIES FOLDER "3rdParty/glfw" )
	set_target_properties( update_mappings       PROPERTIES FOLDER "3rdParty/glfw" )
	set_target_properties( glbinding             PROPERTIES FOLDER "3rdParty/glbinding" )
	set_target_properties( glbinding-aux         PROPERTIES FOLDER "3rdParty/glbinding" )
	set_target_properties( KHRplatform-sources   PROPERTIES FOLDER "3rdParty/glbinding" )
	set_target_properties( pack                  PROPERTIES FOLDER "3rdParty/glbinding" )
	set_target_properties( pack-glbinding        PROPERTIES FOLDER "3rdParty/glbinding" )
	set_target_properties( Bullet2FileLoader     PROPERTIES FOLDER "3rdParty/bullet3" )
	set_target_properties( Bullet3Collision      PROPERTIES FOLDER "3rdParty/bullet3" )
	set_target_properties( Bullet3Common         PROPERTIES FOLDER "3rdParty/bullet3" )
	set_target_properties( Bullet3Dynamics       PROPERTIES FOLDER "3rdParty/bullet3" )
	set_target_properties( Bullet3Geometry       PROPERTIES FOLDER "3rdParty/bullet3" )
	set_target_properties( Bullet3OpenCL_clew    PROPERTIES FOLDER "3rdParty/bullet3" )
	set_target_properties( BulletCollision       PROPERTIES FOLDER "3rdParty/bullet3" )
	set_target_properties( BulletDynamics        PROPERTIES FOLDER "3rdParty/bullet3" )
	set_target_properties( BulletInverseDynamics PROPERTIES FOLDER "3rdParty/bullet3" )
	set_target_properties( BulletSoftBody        PROPERTIES FOLDER "3rdParty/bullet3" )
	set_target_properties( LinearMath            PROPERTIES FOLDER "3rdParty/bullet3" )
	set_target_properties( freetype              PROPERTIES FOLDER "3rdParty/freetype" )
	
	find_package(OpenGL REQUIRED)
	list( APPEND EXTERNAL_LIBRARIES "${OPENGL_LIBRARIES}" )
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
