#ifndef BEMBEL_GUI_GUISYSTEM_HPP
#define BEMBEL_GUI_GUISYSTEM_HPP

#include <bembel/base/utils/factory.hpp>
#include <bembel/kernel/core/system.hpp>

namespace bembel::gui {
class GraphicalUserInterface;

class BEMBEL_API GUISystem : public kernel::System {
  public:
    GUISystem(kernel::Kernel&);
    GUISystem(const GUISystem&) = delete;
    GUISystem& operator=(const GUISystem&) = delete;
    ~GUISystem();

    virtual bool configure(const base::xml::Element*) override;

    virtual bool init() override;
    virtual void shutdown() override;
    virtual void update(double time_since_last_update) override;

    GraphicalUserInterface* createGUI(const std::string& name);

    GraphicalUserInterface* getGUI(const std::string& name);

  private:
    std::vector<std::unique_ptr<GraphicalUserInterface>> guis;

    std::unordered_map<std::string, GraphicalUserInterface*> named_guis;
};

} // namespace bembel::gui
#endif // include guards
