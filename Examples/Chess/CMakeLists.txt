include_directories( "${EXTERNAL_INCLUDE_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/chess-application.h 
	src/chess-application.cpp 
	src/camera-controle.h
	src/camera-controle.cpp
	src/selection-component.h
	src/selection-component.cpp
	src/selection-rendering-stage.h
	src/selection-rendering-stage.cpp
	
	src/chess/chess-game.h 
	src/chess/chess-game.cpp  
	src/chess/player.h 
	src/chess/player.cpp 
	src/chess/chess-piece.h 
	src/chess/chess-piece.cpp
	src/chess/chess-piece-type.h
	src/chess/chess-piece-type.cpp
	src/chess/moves/move-set.h
	src/chess/moves/move-set.cpp
	src/chess/moves/standard-move.h
	src/chess/moves/standard-move.cpp
	src/chess/moves/initial-pawn-move.h
	src/chess/moves/initial-pawn-move.cpp
	src/chess/game-logic/selection-pointer.h
	src/chess/game-logic/selection-pointer.cpp
	src/chess/game-logic/game-state.h
	src/chess/game-logic/game-state.cpp
	src/chess/game-logic/select-chess-piece.h
	src/chess/game-logic/select-chess-piece.cpp
	src/chess/game-logic/select-move.h
	src/chess/game-logic/select-move.cpp
	src/chess/game-logic/perform-move.h
	src/chess/game-logic/perform-move.cpp
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

