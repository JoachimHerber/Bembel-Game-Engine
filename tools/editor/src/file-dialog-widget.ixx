module;
#include <string_view>
#include <filesystem>
export module bembel.tools.editor:FileDialogWidget;

import bembel;

namespace bembel::tools {
using namespace bembel::base;
using namespace bembel::gui;

export class FileDialogWidget : public Widget {
  public:
    FileDialogWidget(Widget& parent, std::u8string_view text = {u8""});

  private:
};

} // namespace bembel::tools