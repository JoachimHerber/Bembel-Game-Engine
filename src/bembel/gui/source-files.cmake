set( RelativeDir "bembel/gui" )

set( DirFiles
	source-files.cmake
	
	gui-system.h
	gui-system.cpp
	graphical-user-interface.h
	graphical-user-interface.cpp

	rendering/renderer.h
	rendering/renderer.cpp
	rendering/skin.h
	rendering/skin.cpp
	rendering/render-batch.h
	rendering/render-batch.cpp
	rendering/widget-view.h
	rendering/widget-view.cpp
	
	interaction/input-handler.h
	interaction/input-handler.cpp
	interaction/observable-value.hpp
	interaction/observable-value.inl
	interaction/interaction-handle.h
	interaction/interaction-handle.cpp
	
	widgets/widget.h
	widgets/widget.cpp
	widgets/group-widget.h
	widgets/group-widget.cpp
	widgets/button/widget.h
	widgets/button/widget.cpp
	widgets/window/widget.h
	widgets/window/widget.cpp
)

foreach( File ${DirFiles} )
	list( APPEND ProjectSources "${RelativeDir}/${File}" )
	
	string( REGEX REPLACE "(.*)/(.*)\\.(.*)" "\\1" subdir ${File} )
	if ("${subdir}" STREQUAL "${File}")
		source_group( "bembel\\gui" FILES "${RelativeDir}/${File}" )
	else()
		source_group( "bembel\\gui\\${subdir}" FILES "${RelativeDir}/${File}" )
	endif()
endforeach()



