target_sources(Bembel
	PUBLIC
		physics.natvis
		
	PRIVATE
        physics.ixx
		
        physics-system.ixx
        physics-system.cpp
        physics-world.ixx
        physics-world.cpp
        collision-shape.ixx
        collision-shape.cpp
        rigid-body.ixx
        rigid-body.cpp
        units.ixx
        debug-renderer.ixx
        debug-renderer.cpp

        CMakeLists.txt
)