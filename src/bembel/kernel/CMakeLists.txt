target_sources(Bembel
	PUBLIC
		kernel.natvis
		
    PRIVATE
        kernel.ixx

        CMakeLists.txt
)
add_subdirectory(assets)
add_subdirectory(display)
add_subdirectory(input)
add_subdirectory(core)
add_subdirectory(rendering)