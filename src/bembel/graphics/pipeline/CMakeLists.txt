target_sources(Bembel
    PRIVATE
        camera.ixx
        camera.cpp
        light-source.ixx
        light-source.cpp
        pipeline.ixx
        rendering-pipeline.ixx
        rendering-pipeline.cpp
        geometry-rendering-stage.ixx
        geometry-rendering-stage.cpp
        deferred-lighting-stage.ixx
        deferred-lighting-stage.cpp
        environment-map-reflection-stage.ixx
        environment-map-reflection-stage.cpp
        CMakeLists.txt
)

