# $Id:$

set( RelativeDir "Source/Bembel/Base/Logging/" )
set( SourceGroup "Source\\Bembel\\Base\\Logging" )

set( DirFiles
	Logger.h
	Logger.cpp
	LogMessage.h
	LogMessage.cpp
	LogSink.h
	LogSink.cpp
	
	ConsoleLogSink.h
	ConsoleLogSink.cpp
	
	_SourceFiles.cmake
)

set( LocalFiles  )
foreach( File ${DirFiles} )
	list( APPEND LocalFiles     "${RelativeDir}/${File}" )
	list( APPEND ProjectSources "${RelativeDir}/${File}" )
endforeach()
source_group( ${SourceGroup} FILES ${LocalFiles} )

