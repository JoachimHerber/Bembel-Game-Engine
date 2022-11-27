module;
#include "bembel/pch.h"
module bembel.examples.chess.logic;

import bembel;
import bembel.examples.chess.board;
import bembel.examples.chess.camera;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

GameLogic::GameLogic(
    ChessBoard*             board,
    SelectionPointer*       selection_ptr,
    CameraControle*         camera,
    GraphicalUserInterface* gui
)
  : m_board(board), m_selection_ptr(selection_ptr), m_camera(camera), m_gui(gui) {
    setState(std::make_unique<SelectChessPice>(this, ChessPlayer::WHITE));

    selection_ptr->getSelectSignal().bind(this, &GameLogic::onClick);
}

void GameLogic::update(double time) {
    if(m_state) m_state->update(time);
}

void GameLogic::onClick() {
    if(m_state) m_state->onClick();
}

void GameLogic::setState(std::unique_ptr<GameState> state) {
    if(m_state) m_state->onExit();
    m_last_state = std::move(m_state);
    m_state      = std::move(state);
    if(m_state) m_state->onEnter();
}

std::vector<GameLogic::Move> GameLogic::getPossibleMoves(ChessPiece pice) {
    std::vector<Move> moves;

    if(!pice) return moves;

    switch(pice.getType()) {
        case ChessPieceType::PAWN: getPossiblePawnMoves(pice, moves); break;
        case ChessPieceType::ROOK: getPossibleRookMoves(pice, moves); break;
        case ChessPieceType::KNIGHT: getPossibleKnightMoves(pice, moves); break;
        case ChessPieceType::BISHOP: getPossibleBishopMoves(pice, moves); break;
        case ChessPieceType::QUEEN: {
            getPossibleRookMoves(pice, moves);
            getPossibleBishopMoves(pice, moves);
            break;
        }
        case ChessPieceType::KING: getPossibleKingMoves(pice, moves); break;
    }
    return moves;
}

void GameLogic::getPossiblePawnMoves(ChessPiece pice, std::vector<Move>& moves) {
    ivec2 pos = pice.getBoardPosition();
    int   dir = (pice.getOwner() == ChessPlayer::WHITE) ? 1 : -1;

    if(addMove(pice, moves, pos + ivec2(dir, 0), false)) {
        if(!pice.hasMoved()) { addMove(pice, moves, pos + ivec2(2 * dir, 0), false); }
    }
    if(auto other = m_board->getChessPieceAt(pos + ivec2(dir, -1))) {
        if(other.getOwner() != pice.getOwner())
            moves.emplace_back(Move::CAPUTE, pos + ivec2(dir, -1));
    }
    if(m_board->canCaptureEnPassant(pos + ivec2(dir, -1))) {
        moves.emplace_back(Move::CAPUTE_EN_PASSANT, pos + ivec2(dir, -1));
    }
    if(auto other = m_board->getChessPieceAt(pos + ivec2(dir, +1))) {
        if(other.getOwner() != pice.getOwner())
            moves.emplace_back(Move::CAPUTE, pos + ivec2(dir, +1));
    }
    if(m_board->canCaptureEnPassant(pos + ivec2(dir, +1))) {
        moves.emplace_back(Move::CAPUTE_EN_PASSANT, pos + ivec2(dir, +1));
    }
}

void GameLogic::getPossibleKnightMoves(ChessPiece pice, std::vector<Move>& moves) {
    ivec2 pos = pice.getBoardPosition();

    addMove(pice, moves, pos + ivec2(+1, +2), true);
    addMove(pice, moves, pos + ivec2(+2, +1), true);
    addMove(pice, moves, pos + ivec2(+2, -1), true);
    addMove(pice, moves, pos + ivec2(+1, -2), true);
    addMove(pice, moves, pos + ivec2(-1, -2), true);
    addMove(pice, moves, pos + ivec2(-2, -1), true);
    addMove(pice, moves, pos + ivec2(-2, +1), true);
    addMove(pice, moves, pos + ivec2(-1, +2), true);
}

void GameLogic::getPossibleRookMoves(ChessPiece pice, std::vector<Move>& moves) {
    ivec2 pos = pice.getBoardPosition();

    for(int i = 1; addMove(pice, moves, pos + ivec2(+i, 0), true); ++i) {}
    for(int i = 1; addMove(pice, moves, pos + ivec2(-i, 0), true); ++i) {}
    for(int i = 1; addMove(pice, moves, pos + ivec2(0, +i), true); ++i) {}
    for(int i = 1; addMove(pice, moves, pos + ivec2(0, -i), true); ++i) {}
}

void GameLogic::getPossibleBishopMoves(ChessPiece pice, std::vector<Move>& moves) {
    ivec2 pos = pice.getBoardPosition();

    for(int i = 1; addMove(pice, moves, pos + ivec2(-i, -i), true); ++i) {}
    for(int i = 1; addMove(pice, moves, pos + ivec2(-i, +i), true); ++i) {}
    for(int i = 1; addMove(pice, moves, pos + ivec2(+i, -i), true); ++i) {}
    for(int i = 1; addMove(pice, moves, pos + ivec2(+i, +i), true); ++i) {}
}

void GameLogic::getPossibleKingMoves(ChessPiece pice, std::vector<Move>& moves) {
    ivec2 pos = pice.getBoardPosition();

    addMove(pice, moves, pos + ivec2(+1, +0), true);
    addMove(pice, moves, pos + ivec2(+1, +1), true);
    addMove(pice, moves, pos + ivec2(+0, +1), true);
    addMove(pice, moves, pos + ivec2(-1, +1), true);
    addMove(pice, moves, pos + ivec2(-1, +0), true);
    addMove(pice, moves, pos + ivec2(-1, -1), true);
    addMove(pice, moves, pos + ivec2(+0, -1), true);
    addMove(pice, moves, pos + ivec2(+1, -1), true);
}

bool GameLogic::addMove(ChessPiece pice, std::vector<Move>& moves, ivec2 pos, bool can_capture) {
    if(pos.x < 0 || 8 <= pos.x) return false;
    if(pos.y < 0 || 8 <= pos.y) return false;

    auto other = m_board->getChessPieceAt(pos);
    if(!other) {
        moves.emplace_back(Move::MOVE, pos);
        return true;
    }
    if(can_capture && other.getOwner() != pice.getOwner()) moves.emplace_back(Move::CAPUTE, pos);
    return false;
}

GameLogic::SelectChessPice::SelectChessPice(GameLogic* logic, ChessPlayer player)
  : m_logic(logic), m_player(player) {}

void GameLogic::SelectChessPice::update(double) {
    ivec2 tile = m_logic->getSelectionPointer()->getSelectedTile();

    ChessPiece selection = m_logic->getChessBoard()->getChessPieceAt(tile);

    if(selection == m_selection) return;

    for(uint x = 0; x < 8; ++x) {
        for(uint y = 0; y < 8; ++y) {
            m_logic->getChessBoard()
                ->getTileAt({x, y})
                .getComponent<SelectionHighlightComponent>()
                ->highlight = SelectionHighlight::NO_HIGHLIGHT;
        }
    }

    if(m_selection) { m_selection.setHighlight(SelectionHighlight::NO_HIGHLIGHT); }

    if(!selection || selection.getOwner() != m_player) {
        m_selection = ChessPiece{};
        return;
    }

    m_selection = selection;

    auto moves = m_logic->getPossibleMoves(m_selection);

    m_selection.setHighlight(
        moves.empty() ? SelectionHighlight::SELECTABLE : SelectionHighlight::FOCUSED
    );

    for(auto move : moves) {
        m_logic->getChessBoard()
            ->getTileAt(move.to)
            .getComponent<SelectionHighlightComponent>()
            ->highlight = SelectionHighlight::SELECTABLE;
    }
}

void GameLogic::SelectChessPice::onClick() {
    if(!m_selection) return;

    if(m_logic->getPossibleMoves(m_selection).empty()) return;

    m_selection.setHighlight(SelectionHighlight::SELECTED);

    m_logic->setState(std::make_unique<SelectMove>(m_logic, m_selection));
}

void GameLogic::SelectChessPice::onEnter() {
    auto lable = m_logic->getGui()->getWidget<LabelWidget>("Label");
    if(!lable) return;

    lable->setText(
        m_player == ChessPlayer::WHITE ? u8"White's Turn: select chess pice"
                                       : u8"Black's Turn: select chess pice"
    );
}

GameLogic::SelectMove::SelectMove(GameLogic* logic, ChessPiece pice)
  : m_logic(logic), m_pice(pice), m_moves(m_logic->getPossibleMoves(pice)) {}

void GameLogic::SelectMove::update(double) {
    ivec2 tile = m_logic->getSelectionPointer()->getSelectedTile();

    size_t selection = 0;
    for(; selection < m_moves.size(); ++selection) {
        if(m_moves[selection].to == tile) break;
    }

    if(m_selection == selection) return;

    if(auto highlight = getHighlightComponentOfSelectedTile()) {
        if(highlight->highlight == SelectionHighlight::FOCUSED)
            highlight->highlight = SelectionHighlight::SELECTABLE;
    }
    m_selection = selection;
    if(auto highlight = getHighlightComponentOfSelectedTile()) {
        if(highlight->highlight == SelectionHighlight::SELECTABLE)
            highlight->highlight = SelectionHighlight::FOCUSED;
    }
}

void GameLogic::SelectMove::onClick() {
    auto highlight = getHighlightComponentOfSelectedTile();
    if(!highlight) return;

    m_pice.setHighlight(SelectionHighlight::NO_HIGHLIGHT);

    for(auto& move : m_moves) {
        m_logic->getChessBoard()
            ->getTileAt(move.to)
            .getComponent<SelectionHighlightComponent>()
            ->highlight = SelectionHighlight::NO_HIGHLIGHT;
    }
    m_logic->setState(std::make_unique<PerformMove>(m_logic, m_pice, m_moves[m_selection]));
}

void GameLogic::SelectMove::onEnter() {
    auto lable = m_logic->getGui()->getWidget<LabelWidget>("Label");
    if(!lable) return;

    lable->setText(
        m_pice.getOwner() == ChessPlayer::WHITE ? u8"White's Turn: select move"
                                                : u8"Black's Turn: select move"
    );
}

GameLogic::PerformMove::PerformMove(GameLogic* logic, ChessPiece pice, Move move)
  : m_logic(logic), m_pice(pice), m_move(move) {
    m_animation = std::make_unique<StandartMoveAnimation>(pice, move.to);
}

void GameLogic::PerformMove::update(double time) {
    bool animation_finished = m_animation->update(time);

    if(!animation_finished) return;

    m_pice.setBoardPosition(m_move.to);

    m_logic->setState(std::make_unique<SelectChessPice>(
        m_logic, m_pice.getOwner() == ChessPlayer::WHITE ? ChessPlayer::BLACK : ChessPlayer::WHITE
    ));
}

void GameLogic::PerformMove::onEnter() {
    auto lable = m_logic->getGui()->getWidget<LabelWidget>("Label");
    if(!lable) return;

    lable->setText(u8"");
}

} // namespace bembel::examples::chess
