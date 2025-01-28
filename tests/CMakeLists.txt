include_directories( "${CMAKE_SOURCE_DIR}" )

include_directories( "../dependencies/googletest/googletest/include" )
set( ProjectSources 
	main.cpp
	coroutine.cpp
)
source_group( "src" FILES ${ProjectSources} )

add_executable( UnitTests ${ProjectSources} )
target_link_libraries( UnitTests Bembel ${EXTERNAL_LIBRARIES})
target_link_libraries( UnitTests gtest_main)
set_target_properties( UnitTests PROPERTIES FOLDER "tests" )
bembel_configure_application( UnitTests "tests")

