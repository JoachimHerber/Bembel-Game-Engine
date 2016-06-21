set( RelativeDir "Source/Bembel/Base/Events" )
set( SourceGroup "Source\\Bembel\\Base\\Events" )

set( DirFiles
	EventChannel.hpp
	EventChannel.inl
	EventManager.h
	EventManager.inl
	EventManager.cpp
	
	Delegate.hpp
	Delegate.inl
	
	Signal.hpp
	Signal.inl
	
	_SourceFiles.cmake
)

set( LocalFiles  )
foreach( File ${DirFiles} )
	list( APPEND LocalFiles     "${RelativeDir}/${File}" )
	list( APPEND ProjectSources "${RelativeDir}/${File}" )
endforeach()
source_group( ${SourceGroup} FILES ${LocalFiles} )



