target_sources(Bembel
    PRIVATE
        rendering.ixx

        frame-buffer-object.ixx
        frame-buffer-object.cpp
        shader.ixx
        shader.cpp
        texture.ixx
        texture.cpp
        texture-atlas.ixx
        texture-atlas.cpp

        font.ixx
        font.cpp
        text.ixx
        text.cpp
        text-layout.ixx
        text-layout.cpp

        CMakeLists.txt
)
