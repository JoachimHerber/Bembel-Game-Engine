#ifndef BEMBEL_GRAPHICS_VIEWPORT_HPP
#define BEMBEL_GRAPHICS_VIEWPORT_HPP

#include <bembel/library.hpp>

namespace bembel::kernel {
class Window;

class BEMBEL_API Viewport final {
  public:
    class BEMBEL_API View {
      public:
        View() {}
        virtual ~View() {
            if(this->viewport) this->viewport->removeView(this);
        }

        virtual void onCurserMove(const glm::vec2& pos){};
        virtual void onResize(const glm::uvec2&){};

        virtual void draw(const glm::ivec2& viewport_position, const glm::uvec2& viewport_size) = 0;

        Viewport* getViewport() { return viewport; }

      protected:
        friend class Viewport;
        Viewport* viewport = nullptr;
    };

    Viewport(Window* window);
    Viewport(
        Window*          window,
        const glm::vec2& relativ_position,
        const glm::vec2& relativ_size,
        const glm::vec2& position_offset,
        const glm::vec2& size_offset);
    ~Viewport();

    const glm::ivec2& getPosition() const;
    const glm::uvec2& getSize() const;

    Window* getWindow();

    bool addView(View*);
    bool removeView(View*);

    void updatePosition(const glm::vec2& frame_buffer_size);
    void updateSize(const glm::vec2& frame_buffer_size);

    void enable();
    void disable();
    bool isEnabled();

    void draw() const;

    void onCurserMove(const glm::vec2& pos) const;

  private:
    Window* window;

    std::vector<View*> views;

    glm::vec2 relativ_position;
    glm::vec2 relativ_size;

    glm::vec2 position_offset;
    glm::vec2 size_offset;

    glm::ivec2 position;
    glm::uvec2 size;

    bool enabled = true;
};

} // namespace bembel::kernel
#endif // include guards
