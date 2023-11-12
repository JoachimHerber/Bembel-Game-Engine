include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/application.ixx
	src/application.cpp 
	src/camera-controle.ixx
	src/camera-controle.cpp
	src/selection-highlight.ixx
	src/selection-highlight.cpp
	src/selector.ixx
	src/selector.cpp
	src/chess-board.ixx
	src/chess-board.cpp  
	src/chess-moves.ixx
	src/chess-moves.cpp  
	src/game-logic.ixx
	src/game-logic.cpp
	src/move-animations.ixx
	src/move-animations.cpp

)

foreach( File ${ProjectSources} )
	string( REGEX REPLACE "(.*)/(.*)\\.(.*)" "\\1" subdir ${File} )
	string(REPLACE "/" "\\" subdir  ${subdir})
	source_group( ${subdir} FILES ${File} )
endforeach()

add_executable( ChessApp ${ProjectSources} )
target_link_libraries( ChessApp Bembel ${EXTERNAL_LIBRARIES})
set_target_properties( ChessApp PROPERTIES FOLDER "examples" )
bembel_configure_application( ChessApp "examples")

