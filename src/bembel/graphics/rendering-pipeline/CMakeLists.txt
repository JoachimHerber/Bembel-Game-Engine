target_sources(Bembel
    PUBLIC
        camera.hpp
	    deferred-lighting-stage.hpp
	    environment-map-reflection-stage.hpp
	    geometry-rendering-stage.hpp
		light-source.hpp
		rendering-pipeline.hpp
		rendering-stage.hpp
	PRIVATE
	    camera.cpp
	    deferred-lighting-stage.cpp
	    environment-map-reflection-stage.cpp
	    geometry-rendering-stage.cpp
		light-source.cpp
		rendering-pipeline.cpp
		rendering-stage.cpp
		CMakeLists.txt
)

