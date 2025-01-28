module bembel.examples.chess;

import std;
import bembel;
import :Board;
import :Selector;
import :Moves;
import :MoveAnimation;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;
using namespace kernel::i18n::literals;
using namespace base::coro;

Task<Entity> selectChessPiece(
    ChessBoard* board, ChessPlayer cur_player, Camera* camera, Signal<>& button_press
) {
    ChessPieceSelector selection{board, cur_player, camera};
    Entity             chess_piece;
    while(!chess_piece) {
        co_await button_press;
        chess_piece = selection.getSelectedChessPiece();
    }
    co_return chess_piece;
}

Task<Move> selectMove(ChessBoard* board, Entity pice, Camera* camera, Signal<>& button_press) {
    std::optional<Move> move;
    {
        MoveSelector selection{board, pice, camera};
        while(!move) {
            co_await button_press;
            move = selection.getSelectedMove();
        }
    }
    co_return move.value();
}

void resetHighlights(ChessBoard* board) {
    for(uint x = 0; x < 8; ++x) {
        for(uint y = 0; y < 8; ++y) {
            board->getTileAt({x, y}).assign<SelectionHighlight>(SelectionHighlight::NO_HIGHLIGHT);
            if(auto chess_piece = board->getChessPieceAt({x, y}))
                chess_piece.setHighlight(SelectionHighlight::NO_HIGHLIGHT);
        }
    }
}

Task<void> runGameLogic(
    ChessBoard*  board,
    Camera*      camera,
    LabelWidget* lable,
    Signal<>&    button_press,
    Signal<>&    frame_sync
) {
    ChessPlayer cur_player   = ChessPlayer::WHITE;
    auto        isWhitesTurn = [&]() { return cur_player == ChessPlayer::WHITE; };
    while(true) {
        lable->setText(
            isWhitesTurn() ? "examples.chess.select_chess_pice.white"_i18n()
                           : "examples.chess.select_chess_pice.black"_i18n()
        );

        auto chess_piece_entity =
            co_await selectChessPiece(board, cur_player, camera, button_press);
        ChessPiece chess_piece(board, chess_piece_entity);

        lable->setText(
            isWhitesTurn() ? "examples.chess.select_move.white"_i18n()
                           : "examples.chess.select_move.black"_i18n()
        );
        chess_piece.setHighlight(SelectionHighlight::SELECTED);

        auto move = co_await selectMove(board, chess_piece_entity, camera, button_press);

        resetHighlights(board);

        chess_piece.makeRigidBodyKinematic();
        auto captured_chess_piece = board->getChessPieceAt(move.to);
        if(move.type == Move::CAPUTE_EN_PASSANT) {
            captured_chess_piece =
                board->getChessPieceAt(ivec2(chess_piece.getBoardPosition().x, move.to.y));
        }
        if(captured_chess_piece) { captured_chess_piece.makeRigidBodyDynamic(); }

        lable->setText(u8"");

        co_await playMoveAnimation(chess_piece, move.to, frame_sync);

        if(captured_chess_piece) {
            // let the captured piece 'ragdoll' for a bit
            using namespace std::chrono_literals;
            auto start_time = std::chrono::steady_clock::now();
            while(std::chrono::steady_clock::now() - start_time < 2s) {
                co_await frame_sync;
                vec3 v = captured_chess_piece.getRigidBodyLinearVelocity();
                if(glm::dot(v, v) < 0.01f) break; // the captured piece has stoped moving

                vec3 pos = captured_chess_piece.getPosition();
                if(pos.y < -1.f) break; // the captured piece has rolled of the board
            }
            // @ToDo add particle effect
            Asset<ParticleEffect> effect    = {"capture_pawn"};
            auto                  particles = board->getScene()->getDataContainer<ParticleData>();
            particles->spawnParticleEffect(
                *effect.get(),
                captured_chess_piece.getPosition(),
                captured_chess_piece.getRotation(),
                1.0f
            );
        }
        chess_piece.setBoardPosition(move.to);

        cur_player = isWhitesTurn() ? ChessPlayer::BLACK : ChessPlayer::WHITE;
    }
    co_return;
}
} // namespace bembel::examples::chess
