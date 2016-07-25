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

	OpenGL/Texture.h
	OpenGL/Texture.cpp
	OpenGL/ShaderProgram.h
	OpenGL/ShaderProgram.cpp
	OpenGL/FrameBufferObject.h
	OpenGL/FrameBufferObject.cpp
	
	RenderingPipeline/Camera.h
	RenderingPipeline/Camera.cpp
	RenderingPipeline/Renderer.h
	RenderingPipeline/RenderingPipeline.h
	RenderingPipeline/RenderingPipeline.cpp
	RenderingPipeline/RenderingStage.h
	RenderingPipeline/DeferredGeometryStage.h
	RenderingPipeline/DeferredGeometryStage.cpp
	RenderingPipeline/DeferredLightingStage.h
	RenderingPipeline/DeferredLightingStage.cpp
	RenderingPipeline/LightSourceProperties.h
	RenderingPipeline/LightSourceProperties.cpp
	
	Geometry/GeometryModel.h
	Geometry/GeometryModel.cpp
	Geometry/Material.h
	Geometry/Material.cpp
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



