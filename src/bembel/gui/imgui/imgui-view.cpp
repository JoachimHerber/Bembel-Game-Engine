module;
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
module bembel.gui.imgui;

import bembel.base;
import bembel.kernel;
import :View;

namespace bembel::gui::imgui {
using namespace bembel::base;
using namespace bembel::kernel;


void View::draw(In<ivec2> viewport_position, In<uvec2> viewport_size) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    drawUI(viewport_position, viewport_size);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace bembel::gui::imgui