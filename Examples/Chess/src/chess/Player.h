#ifndef BEMBEL_PLAYER_H
#define BEMBEL_PLAYER_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

class ChessGame;
class ChessPiece;

class Player {
 public:
  Player(ChessGame*, const std::string& name);

  ChessGame* getChessGame() const;
  const std::string& getName() const;

  const std::vector<ChessPiece*>& getChessPieces() const;

  void clearChessPieces();
  void addChessPiece(ChessPiece*);
  void removeChessPiece(ChessPiece*);

  void clearCaptureChessPieces();
  void captureChessPiece(ChessPiece*);

 private:
  ChessGame* game;
  std::string name;

  std::vector<ChessPiece*> chess_pices;
  std::vector<ChessPiece*> captured_pices;

  glm::vec3 capture_area_pos;
  glm::vec3 capture_area_row_offset;
  glm::vec3 capture_area_collum_offset;
  unsigned capture_area_pices_per_row;
};
#endif // include guards
