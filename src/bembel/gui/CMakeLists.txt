target_sources(Bembel
    	PUBLIC
       		gui.hpp
       		gui-system.hpp
	        graphical-user-interface.hpp
	PRIVATE
        	gui-system.cpp
		graphical-user-interface.cpp
		CMakeLists.txt
)
add_subdirectory(rendering)
add_subdirectory(interaction)
add_subdirectory(widgets)