target_sources(Bembel
    PUBLIC
        base.hpp
	PRIVATE
		CMakeLists.txt
)
add_subdirectory(io)
add_subdirectory(logging)
add_subdirectory(utils)

