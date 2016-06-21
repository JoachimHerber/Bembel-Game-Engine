set( RelativeDir "src/BembelEngine" )
set( SourceGroup "src\\BembelEngine" )

set( DirFiles
	_SourceFiles.cmake
	
	System.h
	System.cpp
	Engine.h
	Engine.cpp
	Application.h
	Application.cpp
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



