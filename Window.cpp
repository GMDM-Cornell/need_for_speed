#include "Window.h"
#include "imgui.h"

Window::Window(const std::string& windowTitle) : title(windowTitle) {}

void Window::Render() {
    if (!open) return;

    if (ImGui::Begin(title.c_str(), closable ? &open : nullptr, ImGuiWindowFlags_MenuBar)) {
        DrawContent();
    }
    ImGui::End();
};