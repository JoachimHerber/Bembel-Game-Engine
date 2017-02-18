set( RelativeDir "src/BembelGraphics" )
set( SourceGroup "src\\BembelGraphics" )

set( DirFiles
	_SourceFiles.cmake
	
	GraphicSystem.h
	GraphicSystem.cpp
	Viewport.h
	Viewport.cpp
	TextureView.h
	TextureView.cpp
	
	RenderingPipeline/Camera.h
	RenderingPipeline/Camera.cpp
	RenderingPipeline/RenderingPipeline.h
	RenderingPipeline/RenderingPipeline.cpp
	RenderingPipeline/RenderingStage.h
	RenderingPipeline/GeometryRenderingStage.h
	RenderingPipeline/GeometryRenderingStage.cpp
	RenderingPipeline/DeferredLightingStage.h
	RenderingPipeline/DeferredLightingStage.cpp
	RenderingPipeline/LightSourceProperties.h
	RenderingPipeline/LightSourceProperties.cpp
	RenderingPipeline/EnvironmentMapReflectionStage.h
	RenderingPipeline/EnvironmentMapReflectionStage.cpp
	
	Geometry/Material.h
	Geometry/Material.cpp
	Geometry/GeometryMesh.h
	Geometry/GeometryMesh.cpp
	Geometry/GeometryModel.h
	Geometry/GeometryModel.cpp
	Geometry/GeometryRenderer.h
	Geometry/GeometryRenderer.cpp
	Geometry/GeometryRenderQueue.h
	Geometry/GeometryRenderQueue.cpp
	Geometry/GeometryComponent.h
	Geometry/GeometryComponent.cpp
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



