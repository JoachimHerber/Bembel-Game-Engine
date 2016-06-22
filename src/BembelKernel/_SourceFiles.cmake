set( RelativeDir "src/BembelKernel" )
set( SourceGroup "src\\BembelKernel" )

set( DirFiles
	_SourceFiles.cmake
	
	Application.h
	Application.cpp
	
	Engine/Engine.h
	Engine/Engine.cpp
	Engine/System.h
	Engine/System.cpp
	
	Display/DisplaySystem.h
	Display/DisplaySystem.cpp
	Display/EventCallbacks.h
	Display/EventCallbacks.cpp
	Display/Events.h
	Display/Events.cpp
	Display/Window.h
	Display/Window.cpp
	Display/DisplayMode.h
	Display/DisplayMode.cpp
)

foreach( File ${DirFiles} )
	list( APPEND ProjectSources "${RelativeDir}/${File}" )
	
	string( REGEX REPLACE "(.*)/(.*)\\.(.*)" "\\1" subdir ${File} )
	if ("${subdir}" STREQUAL "${File}")
		source_group( ${SourceGroup} FILES "${RelativeDir}/${File}" )
	else()
		source_group( ${SourceGroup}\\${subdir} FILES "${RelativeDir}/${File}" )
	endif()
endforeach()



