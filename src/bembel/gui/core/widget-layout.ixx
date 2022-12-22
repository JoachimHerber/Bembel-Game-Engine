module;
#include <string_view>
export module bembel.gui.core:WidgetLayout;

import bembel.base;
import bembel.kernel;
import :Widget;
import :GroupWidget;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class LinearWidgetLayout : public Widget::Layout {
  public:
    enum class Mode {
        SCALE_TO_FIT  = -1,
        ALIGN_BOTTON  = 0,
        ALIGN_LEFT    = 0,
        ALIGN_CENTER  = 1,
        ALIGN_RIGHT   = 2,
        ALIGN_TOP     = 2,
        SPACE_EQUALLY = 3,
    };

    LinearWidgetLayout(GroupWidget* group, In<Mode> mode = Mode::ALIGN_TOP)
      : m_group{group}, m_mode{mode} {}

    ~LinearWidgetLayout() = default;

    virtual bool configure(In<xml::Element const*> properties) override;

    virtual uint getMinWidth() const override;
    virtual uint getMinHeight() const override;

    virtual void updateLayout() override;
    virtual void updateLayout(In<vec2> size) override;

    void setMargin(uint left, uint right) {
        m_margin_left  = left;
        m_margin_right = right;
    }

    struct RowParams {
        Mode                mode = Mode::SCALE_TO_FIT;
        std::optional<uint> height;
        uint                min_height = 0;
        float               rel_height = 0.f;
    };

    LinearWidgetLayout& addRow(In<RowParams> params = {});

    LinearWidgetLayout& addSpacing(In<uint> width);
    LinearWidgetLayout& addWidget(
        In<Widget*> widget, In<float> rel_width = 1.0f, In<uint> min_width = 0
    );

  private:
    struct Row {
        RowParams params;

        struct Element {
            Widget* widget;
            float   rel_width    = 1.0;
            uint    min_width    = 0;
            uint    margin_right = 0;

            mutable uint width = 0;
        };

        std::vector<Element> elements;

        uint margin_left = 0;

        mutable uint min_width  = 0;
        mutable uint min_height = 0;
    };

    void scaleElements(std::span<Row::Element>, uint width);

  private:
    GroupWidget* m_group;
    Mode         m_mode;

    uint m_margin_left  = 0;
    uint m_margin_right = 0;

    std::vector<Row> m_rows;
};

export class RelativeWidgetLayout : public Widget::Layout {
  public:
    RelativeWidgetLayout(GroupWidget* group) : m_group{group} {}
    ~RelativeWidgetLayout() = default;

    virtual bool configure(In<xml::Element const*> properties) override;

    virtual uint getMinWidth() const override;
    virtual uint getMinHeight() const override;

    virtual void updateLayout() override;
    virtual void updateLayout(In<vec2> size) override;

    RelativeWidgetLayout& addWidget(
        In<Widget*> widget,
        In<vec2>    position,
        In<vec2>    size,
        In<vec2>    position_offset = {0.f, 0.f},
        In<vec2>    size_offset     = {0.f, 0.f}
    );

  private:
    struct Element {
        Widget* widget;

        vec2 rel_pos;
        vec2 rel_size;

        vec2 pos_offset;
        vec2 size_offset;
    };
    GroupWidget* m_group;

    std::vector<Element> m_elements;
};

} // namespace bembel::gui