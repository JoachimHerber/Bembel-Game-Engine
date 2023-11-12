module;
#include <coroutine>
#include <filesystem>
export module bembel.examples.chess:Logic;

import bembel;
import :Board;
import :Camera;
import :SelectionHighlight;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;
using namespace graphics;
using namespace gui;

export struct GameLogicCoroutine {
    struct promise_type {
        GameLogicCoroutine get_return_object() {
            return {.hndl = std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never  initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void                return_void() {}
        void                unhandled_exception() {}
    };

    std::coroutine_handle<promise_type> hndl;
};

export GameLogicCoroutine runGameLogic(
    ChessBoard*  board,
    Camera*      camera,
    LabelWidget* lable,
    Signal<>&    button_press,
    Signal<>&    frame_sync
);

} // namespace bembel::examples::chess