target_sources(Bembel
    PRIVATE
        gui.ixx

        gui-system.ixx
        gui-system.cpp

        CMakeLists.txt
)
add_subdirectory(core)
add_subdirectory(widgets)