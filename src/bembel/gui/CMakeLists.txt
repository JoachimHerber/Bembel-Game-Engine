target_sources(Bembel
	PUBLIC
		gui.natvis
		
    PRIVATE
        gui.ixx

        gui-system.ixx
        gui-system.cpp

        CMakeLists.txt
)
add_subdirectory(core)
add_subdirectory(widgets)
add_subdirectory(imgui)