export module bembel.examples.chess:Logic;

import std;
import bembel;
import :Board;
import :Camera;
import :SelectionHighlight;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;


export coro::Task<void> runGameLogic(
    ChessBoard*  board,
    Camera*      camera,
    LabelWidget* lable,
    Signal<>&    button_press,
    Signal<>&    frame_sync
);

} // namespace bembel::examples::chess