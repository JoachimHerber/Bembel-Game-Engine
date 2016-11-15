include_directories( "${EXTERNAL_INCLUDE_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}" )

set( ProjectSources 
	src/main.cpp 
	src/ChessApplication.h 
	src/ChessApplication.cpp 
	src/CameraControle.h
	src/CameraControle.
	
	src/chess/ChessBoard.h 
	src/chess/ChessBoard.cpp 
	src/chess/ChessPiece.h 
	src/chess/ChessPiece.cpp 
	src/chess/Player.h 
	src/chess/Player.cpp 
	src/chess/ChessPieceType.h
	src/chess/ChessPieceType.cpp
)
source_group( "src" FILES ${ProjectSources} )

add_executable( ChessApp ${ProjectSources} )
target_link_libraries( ChessApp Bembel ${EXTERNAL_LIBRARIES})
set_target_properties( ChessApp PROPERTIES FOLDER "Examples" )
bembel_configure_application( ChessApp "Examples/Chess")

