module;
#include <glbinding/gl/gl.h>

#include <chrono>
#include <optional>
module bembel.examples.chess;

import bembel;
import :Selector;
import :Moves;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;
using namespace ::gl;
using namespace std::chrono;

Selector::Selector(Scene* scene, Camera* camera) : m_scene{scene}, m_camera{camera} {}

Selector::~Selector() {}

void Selector::handleEvent(CursorMovedEvent const& event) {
    ivec2 windowSize = event.window->getWindowSize();
    ivec2 fbSize     = event.window->getFrameBufferSize();

    vec2 cursor_pos = event.position;

    // convert courser position form to window coordinates
    // to viewport coordinates.
    cursor_pos /= vec2(windowSize);
    cursor_pos.y = 1.0 - cursor_pos.y;
    cursor_pos *= fbSize;

    for(auto const& vp : event.window->getViewports()) {
        if(!vp) continue;

        vec2 vp_pos = cursor_pos - vec2(vp->getPosition());
        vp_pos /= vec2(vp->getSize());

        // test if the cursor is within the view port
        if(!(0.0f <= vp_pos.x && vp_pos.x <= 1.0f && 0.0f <= vp_pos.y && vp_pos.y <= 1.0f))
            continue;

        updateSelection(calculateRay(vp_pos));
    }
}

Selector::Ray Selector::calculateRay(In<vec2> cursor_pos) const {
    vec4 rel_pos(2 * cursor_pos.x - 1, 2 * cursor_pos.y - 1, 0, 1);
    rel_pos = m_camera->getInverseProjectionMatrix() * rel_pos;
    rel_pos /= rel_pos.w;
    rel_pos = m_camera->getInverseViewMatrix() * rel_pos;

    Ray ray;
    ray.origin    = m_camera->getPosition();
    ray.direction = glm::vec3(rel_pos) - ray.origin;
    ray.direction = glm::normalize(ray.direction);
    return ray;
}

void Selector::updateSelection(In<Ray> ray) {
    onSelect(m_scene->getDataContainer<World>()->rayTestFirst(
        ray.origin, ray.origin + 50.0f * ray.direction
    ));
}

ChessPieceSelector::~ChessPieceSelector() {}

void ChessPieceSelector::onSelect(EntityID entity) {
    if(m_selection.getId() == entity) return;

    if(m_selection) {
        m_selection.assign<SelectionHighlight>(SelectionHighlight::NO_HIGHLIGHT);

        for(uint x = 0; x < 8; ++x) {
            for(uint y = 0; y < 8; ++y) {
                auto tile = m_board->getTileAt({x, y});
                tile.assign<SelectionHighlight>(SelectionHighlight::NO_HIGHLIGHT);
            }
        }
    }

    m_selection = Entity();

    auto chess_piece = m_scene->getComponent<ChessPieceComponent>(entity);
    if(!chess_piece) return;
    if(chess_piece->owner != m_player) return;

    m_selection = Entity(*m_board->getScene(), entity);
    m_selection.assign<SelectionHighlight>(SelectionHighlight::FOCUSED);

    auto moves = getPossibleMoves(m_board, ChessPiece(m_board, m_selection));
    for(auto const& move : moves) {
        m_board->getTileAt(move.to).assign<SelectionHighlight>(SelectionHighlight::SELECTABLE);
    }
}

MoveSelector::MoveSelector(ChessBoard* board, Entity chess_piece, Camera* camera)
  : Selector{board->getScene(), camera}
  , m_board{board}
  , m_chess_piece{chess_piece}
  , m_possible_moves{getPossibleMoves(board, ChessPiece(board, chess_piece))} {}

void MoveSelector::onSelect(EntityID id) {
    m_selection = {};
    for(auto const& move : m_possible_moves) {
        auto tile = m_board->getTileAt(move.to);
        if(tile.getId() == id) {
            tile.assign<SelectionHighlight>(SelectionHighlight::FOCUSED);
            m_selection = move;
        } else {
            tile.assign<SelectionHighlight>(SelectionHighlight::SELECTABLE);
        }
    }
}

} // namespace bembel::examples::chess
