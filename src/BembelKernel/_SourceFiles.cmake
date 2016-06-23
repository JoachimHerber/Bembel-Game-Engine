set( RelativeDir "src/BembelKernel" )
set( SourceGroup "src\\BembelKernel" )

set( DirFiles
	_SourceFiles.cmake
	
	Kernel.h
	Kernel.cpp
	System.h
	System.cpp
	Application.h
	Application.cpp
	
	Events/EventChannel.hpp
	Events/EventChannel.inl
	Events/EventManager.h
	Events/EventManager.inl
	Events/EventManager.cpp
	Events/EventCallbacks.h
	Events/EventCallbacks.cpp
	Events/DisplayEvents.h
	Events/DisplayEvents.cpp
	Events/InputEvents.h
	Events/InputEvents.cpp
	
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



