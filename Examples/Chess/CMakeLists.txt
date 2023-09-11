include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/application.ixx
	src/application.cpp 
	src/camera-controle.ixx
	src/camera-controle.cpp
	src/selection.ixx
	src/selection.cpp
	src/chess-board.ixx
	src/chess-board.cpp  
	src/game-logic.ixx
	src/game-logic.cpp
	src/move-animations.ixx
	src/move-animations.cpp

	#src/chess-game.h 
	#src/chess-game.cpp  
	#src/player.h 
	#src/player.cpp 
	#src/chess-piece.h 
	#src/chess-piece.cpp
	#src/chess-piece-type.h
	#src/chess-piece-type.cpp
	#src/move-set.h
	#src/move-set.cpp
	#src/standard-move.h
	#src/standard-move.cpp
	#src/initial-pawn-move.h
	#src/initial-pawn-move.cpp
	#src/selection-pointer.h
	#src/selection-pointer.cpp
	#src/game-state.h
	#src/game-state.cpp
	#src/select-chess-piece.h
	#src/select-chess-piece.cpp
	#src/select-move.h
	#src/select-move.cpp
	#src/perform-move.h
	#src/perform-move.cpp
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

