set( RelativeDir "src/bembel-base" )
set( SourceGroup "src\\bembel-base" )

set( DirFiles
	source-files.cmake

	xml.h
	xml.cpp
	image.h
	image.cpp
	factory.hpp
	factory.inl
	conversion.h
	conversion.cpp
	
	logging/logger.h
	logging/logger.cpp
	logging/log-message.h
	logging/log-message.cpp
	logging/log-sink.h
	logging/log-sink.cpp
	logging/console-log-sink.h
	logging/console-log-sink.cpp
)

list( APPEND ProjectSources "dependencies/lodepng/lodepng.h" )
list( APPEND ProjectSources "dependencies/lodepng/lodepng.cpp" )
list( APPEND ProjectSources "dependencies/tinyxml2/tinyxml2.h" )
list( APPEND ProjectSources "dependencies/tinyxml2/tinyxml2.cpp" )
source_group( ${SourceGroup}\\dependencies FILES "dependencies/lodepng/lodepng.h" )
source_group( ${SourceGroup}\\dependencies FILES "dependencies/lodepng/lodepng.cpp" )
source_group( ${SourceGroup}\\dependencies FILES "dependencies/tinyxml2/tinyxml2.h" )
source_group( ${SourceGroup}\\dependencies FILES "dependencies/tinyxml2/tinyxml2.cpp" )
	
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



