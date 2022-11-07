target_sources(Bembel
	PRIVATE
	    material.ixx
	    material.cpp
	    mesh.ixx
	    mesh.cpp
	    model.ixx
	    model.cpp
		geometry.ixx
        geometry-component.ixx
	    geometry-component.cpp
	    renderer.ixx
	    renderer.cpp
	    render-queue.ixx
	    render-queue.cpp

		CMakeLists.txt
)

