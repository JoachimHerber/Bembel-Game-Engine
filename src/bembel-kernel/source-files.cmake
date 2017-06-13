set( RelativeDir "src/bembel-kernel" )
set( SourceGroup "src\\bembel-kernel" )

set( DirFiles
	source-files.cmake
	
	kernel.h
	kernel.cpp
	system.h
	system.cpp
	application.h
	application.cpp
	
	events/event-channel.hpp
	events/event-channel.inl
	events/event-manager.h
	events/event-manager.inl
	events/event-manager.cpp
	events/event-callbacks.h
	events/event-callbacks.cpp
	events/display-events.h
	events/input-events.h
	
	display/display-manager.h
	display/display-manager.cpp
	display/display-mode.h
	display/display-mode.cpp
	display/window.h
	display/window.cpp
	
	assets/asset-manager.h
	assets/asset-manager.cpp
	assets/asset-manager.inl
	assets/asset-container.h
	assets/asset-container.inl
	assets/asset-handle.h
	assets/asset-loader.h
	assets/asset-loader.cpp
	assets/asset-locator.h
	assets/asset-locator.cpp
	assets/serial-asset-loader.hpp
	assets/serial-asset-loader.inl
	
	rendering/shader.h
	rendering/shader.cpp
	rendering/texture.h
	rendering/texture.cpp
	rendering/frame-buffer-object.h
	rendering/frame-buffer-object.cpp
	
	scene/scene.h
	scene/scene.cpp
	scene/entity.h
	scene/entity.cpp
	scene/component-container.hpp
	scene/component-container.inl
	scene/position-component.h
	scene/position-component.cpp
	scene/rotation-component.h
	scene/rotation-component.cpp
	
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



