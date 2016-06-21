set( RelativeDir "src/BembelDataStructures" )
set( SourceGroup "src\\BembelDataStructures" )

set( DirFiles
	_SourceFiles.cmake
	
	EntityManager.h
	EntityManager.cpp
	ComponentContainer.hpp
	ComponentContainer.inl
	Entity.h
	Entity.cpp
	PositionComponent.h
	PositionComponent.cpp
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



