module;
#include <string>
export module bembel.gui.widgets:SplitGroup;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

export class SplitGroupWidget : public Widget {
  public:
    static constexpr std::string_view WIDGET_TYPE_NAME = "ResizableTable";

  public:
    SplitGroupWidget(Widget& parent, bool horizontal = true);
    ~SplitGroupWidget();

    virtual bool configure(xml::Element const* properties) override;

    virtual uint getMinWidth(In<std::optional<uint>>) const override;
    virtual uint getMinHeight(In<std::optional<uint>>) const override;

    virtual std::string_view getWidgetTypeName() const override { return WIDGET_TYPE_NAME; }

    GroupWidget& getFirstChildGroup() { return m_first_group; }
    GroupWidget& getSecondChildGroup() { return m_second_group; }

  protected:
    void onSizeChanged(In<ivec2>, In<ivec2>);

    void onMoveSeperator(In<ivec2> cursor, InOut<ivec2> movement);

    void updateLayout();

  private:
    GroupWidget m_first_group{*this};
    GroupWidget m_second_group{*this};

    InteractionHandle m_move_seperator;

    bool m_horizontal;
    int  m_seperator_pos = 0;
};

} // namespace bembel::gui
