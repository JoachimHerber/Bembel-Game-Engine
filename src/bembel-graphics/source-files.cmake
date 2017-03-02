set( RelativeDir "src/bembel-graphics" )
set( SourceGroup "src\\bembel-graphics" )

set( DirFiles
	source-files.cmake
	
	graphic-system.h
	graphic-system.cpp
	viewport.h
	viewport.cpp
	texture-view.h
	texture-view.cpp
	
	rendering-pipeline/camera.h
	rendering-pipeline/camera.cpp
	rendering-pipeline/rendering-pipeline.h
	rendering-pipeline/rendering-pipeline.cpp
	rendering-pipeline/rendering-stage.h
	rendering-pipeline/geometry-rendering-stage.h
	rendering-pipeline/geometry-rendering-stage.cpp
	rendering-pipeline/deferred-lighting-stage.h
	rendering-pipeline/deferred-lighting-stage.cpp
	rendering-pipeline/light-source-component.h
	rendering-pipeline/light-source-component.cpp
	rendering-pipeline/environment-map-reflection-stage.h
	rendering-pipeline/environment-map-reflection-stage.cpp
	
	geometry/material.h
	geometry/material.cpp
	geometry/geometry-mesh.h
	geometry/geometry-mesh.cpp
	geometry/geometry-model.h
	geometry/geometry-model.cpp
	geometry/geometry-renderer.h
	geometry/geometry-renderer.cpp
	geometry/geometry-render-queue.h
	geometry/geometry-render-queue.cpp
	geometry/geometry-component.h
	geometry/geometry-component.cpp
)

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



