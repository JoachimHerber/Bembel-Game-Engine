set( RelativeDir "src/bembel-interaction" )
set( SourceGroup "src\\bembel-interaction" )

set( DirFiles
	source-files.cmake
	
	interaction-system.h
	interaction-system.cpp
	
	signal.hpp
	signal.inl
	slot.hpp
	slot.inl
	
	input/button.h
	input/button.cpp
	input/input-device.h
	input/input-device.cpp
	input/keyboard.h
	input/keyboard.cpp
	input/mouse.h
	input/mouse.cpp
)

foreach( File ${DirFiles} )
	list( APPEND ProjectSources "${RelativeDir}/${File}" )
	
	string( REGEX REPLACE "(.*)/(.*)\\.(.*)" "\\1" subdir ${File} )
	if ("${subdir}" STREQUAL "${File}")
		source_group( ${SourceGroup} FILES "${RelativeDir}/${File}" )
	else()
		string(REPLACE "/" "\\" subdir  ${subdir})
		source_group( ${SourceGroup}\\${subdir} FILES "${RelativeDir}/${File}" )
	endif()
endforeach()



