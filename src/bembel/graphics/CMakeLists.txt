target_sources(Bembel
    PUBLIC
        graphics.hpp
	    graphic-system.hpp
	    texture-view.hpp
	PRIVATE
	    graphic-system.cpp
	    texture-view.cpp
		CMakeLists.txt
)
add_subdirectory(geometry)
add_subdirectory(rendering-pipeline)

