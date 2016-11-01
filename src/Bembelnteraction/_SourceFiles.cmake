set( RelativeDir "src/Bembelnteraction" )
set( SourceGroup "src\\Bembelnteraction" )

set( DirFiles
	_SourceFiles.cmake
	
	InteractionSystem.h
	InteractionSystem.cpp
	
	Signal.hpp
	Signal.inl
	Slot.hpp
	Slot.inl
	
	Input/Button.h
	Input/Button.cpp
	Input/InputDevice.h
	Input/InputDevice.cpp
	Input/Keyboard.h
	Input/Keyboard.cpp
	Input/Mouse.h
	Input/Mouse.cpp
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



