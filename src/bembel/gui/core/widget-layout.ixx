module;
#include "bembel/pch.h"
export module bembel.gui.core:WidgetLayout;

import bembel.base;
import bembel.kernel;
import :Widget;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class WidgetLayout {
  public:
    virtual bool  configure(In<xml::Element const*> properties)            = 0;
    virtual void  calculateLayout(In<ivec2>, In<std::span<Widget* const>>) = 0;
    virtual ivec2 calculateMinSize(In<std::span<Widget const* const>>)     = 0;

    static Factory<WidgetLayout>& GetLayouterFactory();
};

export class LinearWidgetLayout : public WidgetLayout {
  public:
    enum class Direction { VERTICAL, HORIZONTAL };
    enum class PrimaryAxis {
        SCALE_TO_FIT  = -1,
        ALIGN_BUTTON  = 0,
        ALIGN_LEFT    = 0,
        ALIGN_CENTER  = 1,
        ALIGN_TOP     = 2,
        ALIGN_RIGHT   = 2,
        SPACE_EQUALLY = 3,

    };
    enum class SecondaryAxis {
        SCALE_TO_FIT = -1,
        ALIGN_BUTTON = 0,
        ALIGN_LEFT   = 0,
        ALIGN_CENTER = 1,
        ALIGN_TOP    = 2,
        ALIGN_RIGHT  = 2,
    };

    LinearWidgetLayout() = default;
    LinearWidgetLayout(
        In<Direction> dir, In<PrimaryAxis> primary_axis, In<SecondaryAxis> secondary_axis
    )
      : m_direction{dir}
      , m_primary_axis{primary_axis}
      , m_secondary_axis{secondary_axis} {}

    ~LinearWidgetLayout();

    virtual bool configure(In<xml::Element const*> properties) override;
    virtual void calculateLayout(In<ivec2> area_size, In<std::span<Widget* const>> widgets)
        override;
    virtual ivec2 calculateMinSize(In<std::span<Widget const* const>> widgets) override;

  private:
    void calculateHorizontalLayout(In<ivec2> area_size, In<std::span<Widget* const>> widgets);
    void calculateVerticalLayout(In<ivec2> area_size, In<std::span<Widget* const>> widgets);

  private:
    Direction     m_direction      = Direction::VERTICAL;
    PrimaryAxis   m_primary_axis   = PrimaryAxis::SCALE_TO_FIT;
    SecondaryAxis m_secondary_axis = SecondaryAxis::SCALE_TO_FIT;

    static bool registerd;
};

export class RelativeWidgetLayout : public WidgetLayout {
  public:
    RelativeWidgetLayout(){};
    ~RelativeWidgetLayout(){};

    virtual bool configure(In<xml::Element const*> properties) override;
    virtual void calculateLayout(In<ivec2> area_size, In<std::span<Widget* const>> widgets)
        override;
    virtual ivec2 calculateMinSize(In<std::span<Widget const* const>> widgets) override;

  private:
    static bool registerd;
};

} // namespace bembel::gui