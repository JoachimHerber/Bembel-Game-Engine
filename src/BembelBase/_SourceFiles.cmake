set( RelativeDir "src/BembelBase" )
set( SourceGroup "src\\BembelBase" )

set( DirFiles
	_SourceFiles.cmake

	XML.h
	XML.cpp
	Image.h
	Image.cpp
	Factory.hpp
	Factory.inl
	
	Logging/Logger.h
	Logging/Logger.cpp
	Logging/LogMessage.h
	Logging/LogMessage.cpp
	Logging/LogSink.h
	Logging/LogSink.cpp
	Logging/ConsoleLogSink.h
	Logging/ConsoleLogSink.cpp
)

list( APPEND ProjectSources "dependences/lodepng/lodepng.h" )
list( APPEND ProjectSources "dependences/lodepng/lodepng.cpp" )
list( APPEND ProjectSources "dependences/tinyxml2/tinyxml2.h" )
list( APPEND ProjectSources "dependences/tinyxml2/tinyxml2.cpp" )
source_group( ${SourceGroup}\\dependences FILES "dependences/lodepng/lodepng.h" )
source_group( ${SourceGroup}\\dependences FILES "dependences/lodepng/lodepng.cpp" )
source_group( ${SourceGroup}\\dependences FILES "dependences/tinyxml2/tinyxml2.h" )
source_group( ${SourceGroup}\\dependences FILES "dependences/tinyxml2/tinyxml2.cpp" )
	
foreach( File ${DirFiles} )
	list( APPEND ProjectSources "${RelativeDir}/${File}" )
	
	string( REGEX REPLACE "(.*)/(.*)\\.(.*)" "\\1" subdir ${File} )
	if ("${subdir}" STREQUAL "${File}")
		source_group( ${SourceGroup} FILES "${RelativeDir}/${File}" )
	else()
		source_group( ${SourceGroup}\\${subdir} FILES "${RelativeDir}/${File}" )
	endif()
endforeach()



