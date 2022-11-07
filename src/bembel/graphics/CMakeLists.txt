target_sources(Bembel
	PRIVATE
        graphics.ixx
	    graphic-system.ixx
	    graphic-system.cpp

		CMakeLists.txt
)
add_subdirectory(geometry)
add_subdirectory(pipeline)

