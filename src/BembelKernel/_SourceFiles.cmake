set( RelativeDir "src/BembelKernel" )
set( SourceGroup "src\\BembelKernel" )

set( DirFiles
	_SourceFiles.cmake
	
	Kernel.h
	Kernel.cpp
	System.h
	System.cpp
	Application.h
	Application.cpp
	
	Events/EventChannel.hpp
	Events/EventChannel.inl
	Events/EventManager.h
	Events/EventManager.inl
	Events/EventManager.cpp
	Events/EventCallbacks.h
	Events/EventCallbacks.cpp
	Events/DisplayEvents.h
	Events/DisplayEvents.cpp
	Events/InputEvents.h
	Events/InputEvents.cpp
	
	Display/DisplayManager.h
	Display/DisplayManager.cpp
	Display/Window.h
	Display/Window.cpp
	Display/DisplayMode.h
	Display/DisplayMode.cpp
	
	Input/Signal.hpp
	Input/Signal.inl
	Input/Slot.hpp
	Input/Slot.inl
	Input/Button.h
	Input/Button.cpp
	
	Assets/AssetDescription.h
	Assets/AssetDescription.cpp
	Assets/AssetContainer.h
	Assets/AssetContainer.inl
	Assets/AssetHandle.h
	Assets/AssetLoader.h
	Assets/AssetLoader.cpp
	Assets/SerialAssetLoader.hpp
	Assets/SerialAssetLoader.inl
	Assets/AssetManager.h
	Assets/AssetManager.cpp
	Assets/AssetManager.inl
	
	Scene/Scene.h
	Scene/Scene.cpp
	Scene/Entity.h
	Scene/Entity.cpp
	Scene/ComponentContainer.hpp
	Scene/ComponentContainer.inl
	Scene/PositionComponent.h
	Scene/PositionComponent.cpp
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



