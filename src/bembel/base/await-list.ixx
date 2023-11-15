module;
#include <coroutine>
export module bembel.base:AwaitList;

import :Types;

namespace bembel::base {

template <typename T>
struct AwaitListNode {
    AwaitListNode*          prev;
    AwaitListNode*          next;
    std::coroutine_handle<> hndl;
    T                       value;
};

template <>
struct AwaitListNode<void> {
    AwaitListNode*          prev;
    AwaitListNode*          next;
    std::coroutine_handle<> hndl;
};

export template <typename T>
class AwaitList {
    using Node = AwaitListNode<T>;

  public:
    AwaitList() { clear(); }

    class Awaiter : public Node {
      public:
        Awaiter(Node* p, Node* n) : Node(p, n) {
            this->prev.next = this;
            this->next.prev = this;
        }
        Awaiter(AwaitList const& al) : Awaiter(al.m_end.prev, &al.m_end) {}
        Awaiter(Awaiter const&) = delete;
        Awaiter(Awaiter&& tmp) : Awaiter(tmp.prev, tmp.next) {
            tmp.prev = nullptr;
            tmp.next = nullptr;
        }
        ~Awaiter() {
            if(this->prev) this->prev.next = this->next;
            if(this->next) this->next.prev = this->prev;
        }

        constexpr bool await_ready() const noexcept { return false; }
        void           await_suspend(std::coroutine_handle<> handle) { this->hndl = handle; }
        T              await_resume() { return this->value; }

      private:
    };
    friend Awaiter;

    Awaiter operator co_await() { return {m_end.prev, &m_end}; }

    void notify()
        requires std::is_same_v<T, void>
    {
        Node* node = m_begin.next;
        clear();
        while(node) {
            Node* next = node.next;
            if(node->hndl && !node->hndl.done()) { node->hndl.resume(); }
            node = next;
        }
    }
    void notify(In<T> value)
        requires !std::is_same_v<T, void>
    {
        Node* node = m_begin.next;
        clear();
        while(node) {
            Node* next = node->next;
            if(node->hndl && !node->hndl.done()) {
                next->value = value;
                node->hndl.resume();
            }
            node = next;
        }
    }

    void clear() {
        if(m_begin.next) m_begin.next->prev = nullptr;
        if(m_end.prev) m_end.prev->next = nullptr;
        m_begin.next = &m_end;
        m_end.prev   = &m_begin;
    }

  private:
    Node m_begin;
    Node m_end;
};

} // end of namespace bembel::base
