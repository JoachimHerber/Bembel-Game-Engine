set( RelativeDir "src/BembelDisplay" )
set( SourceGroup "src\\BembelDisplay" )

set( DirFiles
	_SourceFiles.cmake
	
	DisplaySystem.h
	DisplaySystem.cpp
	
	Window.h
	Window.cpp
	DisplayMode.h
	DisplayMode.cpp
	
	Events.h
	Events.cpp
	EventCallbacks.h
	EventCallbacks.cpp
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



