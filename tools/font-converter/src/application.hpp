#ifndef BEMBEL_TOOLS_FONTCONVERTER_APPLICATION_HPP
#define BEMBEL_TOOLS_FONTCONVERTER_APPLICATION_HPP

#include <bembel/bembel.hpp>

#include "./font-converter.hpp"
#include "./font-view.hpp"
#include "./preview-renderer.hpp"

class Application : public bembel::Application {
  public:
    Application();
    ~Application();

    virtual bool init() override;
    virtual void cleanup() override;

    virtual void update(double time) override;

    void handleEvent(const bembel::WindowShouldCloseEvent&);
    void handleEvent(const bembel::WindowResizeEvent&);
    void handleEvent(const bembel::FileDropEvent&);

  private:
    bembel::GUISystem* gui_system;

    std::unique_ptr<FontConverter>   converter;
    std::unique_ptr<PreviewRenderer> preview;
    std::unique_ptr<FontView>        font_view;
};

#endif // include guards
