set( RelativeDir "src/BembelKernel" )
set( SourceGroup "src\\BembelKernel" )

set( DirFiles
	_SourceFiles.cmake
	
	Kernel.h
	Kernel.cpp
	Application.h
	Application.cpp
	
	Events/EventCallbacks.h
	Events/EventCallbacks.cpp
	Events/Events.h
	Events/Events.cpp
	
	Engine/Engine.h
	Engine/Engine.cpp
	Engine/System.h
	Engine/System.cpp
	
	Display/DisplayManager.h
	Display/DisplayManager.cpp
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



