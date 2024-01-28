module;
#include <optional>
#include <vector>
export module bembel.kernel.display:Viewport;

import bembel.base;

namespace bembel::kernel {
using namespace bembel::base;

export class Viewport final {
  public:
    class View {
      public:
        View() = default;
        virtual ~View() {
            if(m_viewport) m_viewport->removeView(this);
        }

        virtual void onCurserMove(In<vec2> pos){};
        virtual void onResize(In<uvec2>){};

        virtual void draw(In<ivec2> viewport_position, In<uvec2> viewport_size) = 0;

        Viewport* getViewport() { return m_viewport; }

      protected:
        friend class Viewport;
        Viewport* m_viewport = nullptr;
    };

    Viewport(
        In<WindowId>            window_id,
        In<std::optional<vec2>> rel_pos     = {},
        In<std::optional<vec2>> rel_size    = {},
        In<std::optional<vec2>> pos_offset  = {},
        In<std::optional<vec2>> size_offset = {}
    )
      : m_window_id{window_id}
      , m_relativ_position{rel_pos}
      , m_relativ_size{rel_size}
      , m_position_offset{pos_offset}
      , m_size_offset{size_offset} {}
    ~Viewport() {
        for(auto it : m_views) it->m_viewport = nullptr;
    }

    void enable() { m_enabled = true; }
    void disable() { m_enabled = false; }
    bool isEnabled() { return m_enabled; }

    const ivec2& getPosition() const { return m_position; }
    const uvec2& getSize() const { return m_size; }

    WindowId getWindowID() { return m_window_id; }

    bool addView(View* view) {
        if(!view || view->m_viewport) return false;

        m_views.push_back(view);
        view->m_viewport = this;
        view->onResize(m_size);
        return true;
    }
    bool removeView(View* view) {
        if(!view || view->m_viewport != this) return false;
        view->m_viewport = nullptr;

        auto it = std::find(m_views.begin(), m_views.end(), view);
        if(it == m_views.end()) return false;

        m_views.erase(it);
        return true;
    }

    void updatePosition(const vec2& frame_buffer_size) {
        if(m_relativ_position) {
            m_position = ivec2(
                frame_buffer_size * *m_relativ_position + m_position_offset.value_or(vec2{0, 0})
            );
        }
    }
    void updateSize(const vec2& frame_buffer_size) {
        if(m_relativ_size) {
            m_size =
                ivec2(frame_buffer_size * *m_relativ_size + m_size_offset.value_or(vec2{0, 0}));
            for(auto view : m_views) { view->onResize(m_size); }
        }
    }

    void draw() const {
        if(!m_enabled) return;

        for(auto view : m_views) view->draw(m_position, m_size);
    }

    void onCurserMove(const vec2& pos) const {
        for(auto view : m_views) view->onCurserMove(pos);
    }

    void setPosition(In<ivec2> pos) { m_position = pos; }
    void setSize(In<uvec2> size) { m_size = size; }

  private:
    WindowId m_window_id;

    std::vector<View*> m_views;

    std::optional<vec2> m_relativ_position;
    std::optional<vec2> m_relativ_size;

    std::optional<vec2> m_position_offset;
    std::optional<vec2> m_size_offset;

    ivec2 m_position;
    uvec2 m_size;

    bool m_enabled = true;
};

} // namespace bembel::kernel
