target_sources(Bembel
    PUBLIC
		font.hpp
        frame-buffer-object.hpp
        shader.hpp
		texture.hpp
		texture-atlas.hpp
		text-layout.hpp
	PRIVATE
		font.cpp
        frame-buffer-object.cpp
		shader.cpp
		texture.cpp
		texture-atlas.cpp
		text-layout.cpp
		CMakeLists.txt
)
