#include "./group-widget.hpp"

#include "../rendering/render-batch.hpp"
#include "./widget-type-utils.hpp"

namespace bembel::gui {

const std::string GroupWidget::WIDGET_TYPE_NAME = "Group";

GroupWidget::GroupWidget(GraphicalUserInterface* _gui, Widget* _parent)
: Widget{_gui, _parent} {
    size.change_signal.addSlot(this, &GroupWidget::onSizeChanged);
}

GroupWidget::~GroupWidget() {
    size.change_signal.removeSlot(this, &GroupWidget::onSizeChanged);
}

bool GroupWidget::init() {
    return false;
}

inline void stringToLower(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
}

bool GroupWidget::init(const base::xml::Element* properties) {
    if(!properties) return true;

    std::string layout;
    if(base::xml::getAttribute(properties, "layout", layout)) {
        stringToLower(layout);
        if(layout == "linear") {
            std::string direction = "vertical", alignment = "center";
            base::xml::getAttribute(properties, "layout_direction", direction);
            base::xml::getAttribute(properties, "layout_alignment", alignment);
            stringToLower(direction);
            stringToLower(alignment);
            this->setLayouter(std::make_shared<LinearWidgetLayouter>(direction, alignment));
        }
    }

    for(auto it : base::xml::IterateChildElements(properties)) {
        auto widget = this->createChildWidget(it->Value());
        if(!widget || !widget->init(it)) return false;
    }
    return true;
}

Widget* GroupWidget::createChildWidget(const std::string& widget_type_name) {
    auto& factory = WidgetTypeUtils::getWidgetFactory();
    this->widgets.push_back(factory.createObject(widget_type_name, this));
    this->child_widgets.push_back(this->widgets.back().get());
    return this->widgets.back().get();
}

const std::string& GroupWidget::getWidgetTypeName() const {
    return WIDGET_TYPE_NAME;
}

void GroupWidget::setLayouter(std::shared_ptr<WidgetLayouter> layouter) {
    this->layouter = layouter;
}

glm::ivec2 GroupWidget::getMinSize() const {
    if(this->layouter) {
        return this->layouter->calculateMinSize(this->child_widgets.begin(), this->child_widgets.end());
    } else {
        return glm::ivec2(0, 0);
    }
}

void GroupWidget::onSizeChanged(const glm::ivec2& old_size, const glm::ivec2& new_size) {
    if(this->layouter) {
        this->layouter->calculateLayout(new_size, this->child_widgets.begin(), this->child_widgets.end());
    }
}

LinearWidgetLayouter::LinearWidgetLayouter(const std::string& direction, const std::string& alignment) {
    const static std::map<std::string, Direction> string_to_direction_enum = {
        {"vertical", Direction::VERTICAL}, {"horizontal", Direction::HORIZONTAL}};
    const static std::map<std::string, Alignment> string_to_allingment_enum = {
        {"left", Alignment::LEFT},
        {"button", Alignment::BUTTON},
        {"center", Alignment::CENTER},
        {"right", Alignment::RIGHT},
        {"top", Alignment::TOP}};

    auto direction_it = string_to_direction_enum.find(direction);
    if(direction_it != string_to_direction_enum.end()) this->direction = direction_it->second;

    auto alignment_it = string_to_allingment_enum.find(alignment);
    if(alignment_it != string_to_allingment_enum.end()) this->alignment = alignment_it->second;
}

LinearWidgetLayouter::LinearWidgetLayouter(Direction direction, Alignment alignment)
: direction{direction}
, alignment{alignment} {
}

LinearWidgetLayouter::~LinearWidgetLayouter() {
}

void LinearWidgetLayouter::calculateLayout(
    const glm::ivec2& size, WidgetLayouter::WidgetIterator begin, WidgetLayouter::WidgetIterator end) {
    int sum_widget_heights = 0;
    int num_child_widgets  = 0;
    for(auto it = begin; it != end; ++it) {
        sum_widget_heights += (*it)->size.get().y;
        ++num_child_widgets;
    }

    float vertical_space = (size.y - sum_widget_heights) / (num_child_widgets + 1.f);
    float offset         = size.y - vertical_space;

    for(auto it = begin; it != end; ++it) {
        offset -= (*it)->size.get().y;
        int x = 0;
        if(this->alignment == Alignment::CENTER) x = (size.x - (*it)->size.get().x) / 2;
        if(this->alignment == Alignment::RIGHT) x = (size.x - (*it)->size.get().x);

        (*it)->position.set(glm::ivec2{x, int(offset)});

        offset -= vertical_space;
    }
}

glm::ivec2 LinearWidgetLayouter::calculateMinSize(
    WidgetLayouter::ConstWidgetIterator begin, WidgetLayouter::ConstWidgetIterator end) {
    int sum = 0;
    int max = 0;

    bool vertical  = this->direction == Direction::VERTICAL;
    bool no_resize = this->resize_mode == ResizeMode::NO_RESIZE;

    for(auto it = begin; it != end; ++it) {
        auto& size = no_resize ? (*it)->size.get() : (*it)->getMinSize();
        sum += vertical ? size.y : size.x;
        max = std::max(max, vertical ? size.x : size.y);
    }
    return vertical ? glm::ivec2{max, sum} : glm::ivec2{sum, max};
}

} // namespace bembel::gui
