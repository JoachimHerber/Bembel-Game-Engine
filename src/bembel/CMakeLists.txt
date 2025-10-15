target_sources(Bembel
	PRIVATE
        bembel.ixx

		CMakeLists.txt
)
add_subdirectory(base)
add_subdirectory(kernel)
add_subdirectory(graphics)
add_subdirectory(gui)
add_subdirectory(physics)
add_subdirectory(particles)
