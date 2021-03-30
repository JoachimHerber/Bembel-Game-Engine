target_sources(Bembel
    PUBLIC
        geometry.hpp
	    material.hpp
	    mesh.hpp
	    model.hpp
	    renderer.hpp
	    render-queue.hpp
	PRIVATE
	    geometry.cpp
	    material.cpp
	    mesh.cpp
	    model.cpp
	    renderer.cpp
	    render-queue.cpp
		CMakeLists.txt
)

