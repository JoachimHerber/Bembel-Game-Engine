module;
#include <cassert>
#include <filesystem>
#include <glm/glm.hpp>
module bembel.gui.widgets:Tabs;

import bembel.base;
import bembel.kernel;
import bembel.gui.core;

namespace bembel::gui {
using namespace bembel::base;
using namespace bembel::kernel;

TabGroupWidget::Tab::Tab(TabGroupWidget* group, usize index, std::u8string_view label)
  : m_group{group}, m_index{index}, m_label{*group, label}, m_content{*group} {
    m_handle.action_signal.bind(this, &Tab::onAction);
    m_handle.cursor = Asset<CursorIcon>("Hand");

    m_label.setName("Label");
    m_label.setAlignment(LabelWidget::Alignment::Center);
}

TabGroupWidget::Tab::~Tab() {}

void TabGroupWidget::Tab::onAction(InteractionHandle::Action action, ivec2) {
    if(action != InteractionHandle::Action::INTERACT) return;

    m_group->setSelectedTab(m_index);
}

TabGroupWidget::TabGroupWidget(Widget& parent) : Widget{parent} {
    size.change_signal.bind(this, &TabGroupWidget::onSizeChanged);

    m_view = std::make_unique<SimpleTabGroupWidgetView>(*this);
}

TabGroupWidget::~TabGroupWidget() {}

bool TabGroupWidget::configure(xml::Element const* properties) {
    // for(auto* tab_config: xml::i)
    // if(!m_title_bar.configure(properties->FirstChildElement("TileBar"))) return false;
    // if(!m_window_area.configure(properties->FirstChildElement("WindowArea"))) return false;

    Widget::configure(properties);

    return true;
}

uint TabGroupWidget::getMinWidth() const {
    // auto style = getStyle();
    // assert(style && "GUI::Style is undefined");
    //
    // float border_width = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);

    return 0;
}

uint TabGroupWidget::getMinHeight() const {
    // auto style = getStyle();
    // assert(style && "GUI::Style is undefined");
    //
    // float border_width     = style->getValue(Style::Values::WINDOW_BORDER_WIDTH);
    // float title_bar_height = style->getValue(Style::Values::WINDOW_TITLE_BAR_HEIGHT);
    return 0;
}

TabGroupWidget::Tab* TabGroupWidget::addTab(std::u8string_view lable) {
    m_tabs.push_back(std::make_unique<Tab>(this, m_tabs.size(), lable));

    auto tab = m_tabs.back().get();

    if(m_child_widgets.empty()) { m_child_widgets.push_back(&tab->m_content); }
    m_interaction_handles.push_back(&tab->m_handle);
    m_child_widgets.push_back(&tab->m_label);

    updateLayout();

    return tab;
}

void TabGroupWidget::setSelectedTab(usize index) {
    if(index >= m_tabs.size()) return;
    if(m_selected_tab == index) return;

    m_selected_tab     = index;
    m_child_widgets[0] = &m_tabs[m_selected_tab]->m_content;
}

void TabGroupWidget::onSizeChanged(In<ivec2>, In<ivec2>) {
    updateLayout();
}

void TabGroupWidget::updateLayout() {
    auto style = getStyle();
    assert(style && "GUI::Style is undefined");

    if(m_tabs.empty()) return;

    int bar_height   = int(style->getValue(Style::Values::TAB_BAR_HEIGHT));
    int border_width = int(style->getValue(Style::Values::INPUT_BORDER_WIDTH));
    int text_margin  = int(style->getValue(Style::Values::BUTTON_TEXT_MARGIN));
    int tab_margin   = int(style->getValue(Style::Values::TAB_MARGIN));

    ivec2 content_size = this->size.get();
    content_size.y -= bar_height;

    int tab_label_width = 64;
    for(auto& tab : m_tabs) {
        tab_label_width =
            std::max(tab_label_width, int(tab->m_label.getMinWidth()) + 2 * text_margin);
    }

    tab_label_width = std::min(tab_label_width, content_size.x / int(m_tabs.size()));

    int x = tab_margin;
    for(auto& tab : m_tabs) {
        tab->m_label.position  = ivec2(x, content_size.y) + ivec2(text_margin);
        tab->m_handle.position = ivec2(x, content_size.y);
        tab->m_label.size      = ivec2(tab_label_width, bar_height) - ivec2(2 * text_margin);
        tab->m_handle.size     = ivec2(tab_label_width, bar_height);
        tab->m_content.size    = content_size;
        x += tab_label_width + tab_margin;
    }
}

void SimpleTabGroupWidgetView::draw(RenderBatchInterface& batch) {
    auto style = m_widget.getStyle();
    assert(style && "GUI::Style is undefined");

    int bar_height   = int(style->getValue(Style::Values::TAB_BAR_HEIGHT));
    int border_width = int(style->getValue(Style::Values::INPUT_BORDER_WIDTH));
    int tab_margin   = int(style->getValue(Style::Values::TAB_MARGIN));

    auto min = m_widget.position.get();
    auto max = min + m_widget.size.get();
    min.y    = max.y - bar_height;

    auto tc_bar = style->getTextureCoords("tab_bar");

    auto tc_tab_inactive = style->getTextureCoords("tab_inactive");
    auto tc_tab_active = style->getTextureCoords("tab_active");

    if(!tc_bar || !tc_tab_inactive || !tc_tab_active) return;

    batch.setSecondaryColor(style->getColor(Style::Colors::BORDER));
    batch.setAlpha(0xff);

    batch.drawIcon(min, max, tc_bar->min, tc_bar->max);

    auto drawTab = [&](TabGroupWidget::Tab* tab) {
        auto pos  = tab->getPosition();
        auto size = tab->getSize();

        float x0 = pos.x - tab_margin;
        float x1 = pos.x;
        float x2 = pos.x + size.x;
        float x3 = pos.x + size.x + tab_margin;

        auto tc = tab->isSelected() ? tc_tab_active : tc_tab_inactive;

        float const u0 = tc->min.x;
        float const u1 = (tc->min.x + tc->max.x) * 0.5f;
        float const u2 = tc->max.x;

        float const v0 = tc->min.y;
        float const v1 = tc->max.y;

        if(tab->isSelected()) {
            batch.setPrimaryColor(style->getColor(Style::Colors::BUTTON_ACTIVE));
        } else if(tab->isHovered()) {
            batch.setPrimaryColor(style->getColor(Style::Colors::BUTTON_HOVERED));
        } else {
            batch.setPrimaryColor(style->getColor(Style::Colors::BUTTON));
        }

        batch.drawIcon({x0, min.y}, {x1, max.y}, {u0, v0}, {u1, v1});
        batch.drawIcon({x1, min.y}, {x2, max.y}, {u1, v0}, {u1, v1});
        batch.drawIcon({x2, min.y}, {x3, max.y}, {u1, v0}, {u2, v1});
    };

    for(usize i = 0; i < m_widget.getNumTabs(); ++i) {
        auto tab = m_widget.getTab(i);
        if(tab->isSelected()) continue;

        drawTab(tab);
    }
    drawTab(m_widget.getTab(m_widget.getSelectedTab()));
} // namespace bembel::gui

} // namespace bembel::gui
