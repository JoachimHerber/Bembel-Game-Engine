module;
#include <string_view>
export module bembel.tools.editor:ModelMgr;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;

export class ModelManager {
  public:
    ModelManager(In<not_null_ptr<TabGroupWidget::Tab>> tab) : m_tab{tab} {
        using LinearWidgetLayout::Mode::SCALE_TO_FIT;

        m_preview = tab->getContentArea().createChildWidget<GroupWidget>();
        m_buttons = tab->getContentArea().createChildWidget<GroupWidget>();

        auto style = m_preview->getStyle();

        m_tab->getContentArea().background_color = style->getColor(Style::Colors::WINDOW_BORDER);

        m_preview->background_color = style->getColor(Style::Colors::WINDOW_BACKGROUND);
        m_buttons->background_color = style->getColor(Style::Colors::WINDOW_BACKGROUND);

        auto root_layout = tab->getContentArea().setLayout<LinearWidgetLayout>(SCALE_TO_FIT);
        root_layout->setMargin(4, 4);
        root_layout->addRow({.height = 4});
        root_layout->addRow({.rel_height = 1.0f})
            .addWidget(m_preview, 1.0f)
            .addSpacing(4)
            .addWidget(m_buttons, 0.0f, 100);
        root_layout->addRow({.height = 4});

        m_import_models = m_buttons->createChildWidget<ButtonWidget>(u8"Import Models");

        auto button_layout = m_buttons->setLayout<LinearWidgetLayout>(SCALE_TO_FIT);
        button_layout->setMargin(8, 8);
        button_layout->addRow({.min_height = 8, .rel_height = 0.0});
        button_layout->addRow().addWidget(m_import_models);
        button_layout->addRow({.min_height = 0, .rel_height = 1.0});

        tab->getContentArea().updateLayout();
    }

  private:
    not_null_ptr<TabGroupWidget::Tab> m_tab;

    not_null_ptr<GroupWidget> m_preview;
    not_null_ptr<GroupWidget> m_buttons;

    not_null_ptr<ButtonWidget> m_import_models;
};

} // namespace bembel::tools