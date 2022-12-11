include_directories( "${EXTERNAL_INCLUDE_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/physics-example.ixx 
	src/physics-example.cpp
	src/camera-controle.ixx
	src/camera-controle.cpp
	gui.xml
	config.xml
)
source_group( "src" FILES ${ProjectSources} )

add_executable( PhysicsExample ${ProjectSources} )
target_link_libraries( PhysicsExample Bembel ${EXTERNAL_LIBRARIES})
set_target_properties( PhysicsExample PROPERTIES FOLDER "examples" )
bembel_configure_application( PhysicsExample "examples")

