module;
#include <coroutine>
export module bembel.examples.chess:MoveAnimation;

import bembel;
import :Board;
import :Moves;

namespace bembel::examples::chess {
using namespace base;
using namespace kernel;

struct MoveAnimation {
    struct Promise {
        using Handle = std::coroutine_handle<Promise>;

        MoveAnimation get_return_object() { return MoveAnimation(Handle::from_promise(*this)); }
        std::suspend_never  initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void                return_void() noexcept {
            if(parent && !parent.done()) parent.resume();
        }
        void unhandled_exception() { logError("Unhandled exception in coroutine\n"); }

        std::coroutine_handle<> parent;
    };
    struct Awaiter : std::suspend_always {
        Awaiter(std::coroutine_handle<Promise> handle) : m_handle{handle} {}
        void await_suspend(std::coroutine_handle<> handle) { m_handle.promise().parent = handle; }

      private:
        std::coroutine_handle<Promise> m_handle;
    };

    using promise_type = Promise;
    using Handle       = std::coroutine_handle<promise_type>;

    MoveAnimation(Handle hndl) : m_hndl(hndl) {}
    ~MoveAnimation() { m_hndl.destroy(); }

    Awaiter operator co_await() { return {m_hndl}; }

  private:
    Handle m_hndl;
};

export MoveAnimation playMoveAnimation(ChessPiece chess_piece, ivec2 to, Signal<>& frame_sync);

// export class StandartMoveAnimation : public MoveAnimation {
//   public:
//     StandartMoveAnimation(ChessPiece, ivec2 to);
//
//     bool update(double time) override;
//
//   private:
//     ChessPiece m_chess_piece;
//
//     vec3  m_start;
//     vec3  m_dir;
//     float m_dist;
//     float m_hop_dist;
//     float m_hop_height;
//
//     float m_progress = 0;
// };

} // namespace bembel::examples::chess