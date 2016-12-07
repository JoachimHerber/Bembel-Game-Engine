include_directories( "${EXTERNAL_INCLUDE_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/ChessApplication.h 
	src/ChessApplication.cpp 
	src/CameraControle.h
	src/CameraControle.cpp
	
	src/chess/ChessBoard.h 
	src/chess/ChessBoard.cpp 
	src/chess/ChessPiece.h 
	src/chess/ChessPiece.cpp 
	src/chess/Player.h 
	src/chess/Player.cpp 
	src/chess/ChessPieceType.h
	src/chess/ChessPieceType.cpp
	src/chess/Moves/MoveSet.h
	src/chess/Moves/MoveSet.cpp
	src/chess/Moves/StandardMove.h
	src/chess/Moves/StandardMove.cpp
	src/chess/Moves/InitialPawnMove.h
	src/chess/Moves/InitialPawnMove.cpp
	
	src/GameLogic/GameState.h
	src/GameLogic/GameState.cpp
	src/GameLogic/SelectChessBoardState.h
	src/GameLogic/SelectChessBoardState.cpp
	src/GameLogic/SelectChessPieceState.h
	src/GameLogic/SelectChessPieceState.cpp
	src/GameLogic/SelectMoveState.h
	src/GameLogic/SelectMoveState.cpp
	src/GameLogic/PerformMoveState.h
	src/GameLogic/PerformMoveState.cpp
	src/GameLogic/StateMashine.h
	src/GameLogic/StateMashine.cpp
)

foreach( File ${ProjectSources} )
	string( REGEX REPLACE "(.*)/(.*)\\.(.*)" "\\1" subdir ${File} )
	string(REPLACE "/" "\\" subdir  ${subdir})
	source_group( ${subdir} FILES ${File} )
endforeach()

add_executable( ChessApp ${ProjectSources} )
target_link_libraries( ChessApp Bembel ${EXTERNAL_LIBRARIES})
set_target_properties( ChessApp PROPERTIES FOLDER "Examples" )
bembel_configure_application( ChessApp "Examples/Chess")

