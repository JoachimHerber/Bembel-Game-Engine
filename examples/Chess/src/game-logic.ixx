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

export coro::Task<> runGameLogic(
    ChessBoard* board, Camera* camera, LabelWidget* lable, Signal<>& button_press
);

} // namespace bembel::examples::chess