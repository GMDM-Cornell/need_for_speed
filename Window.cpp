#include "Window.h"
#include "imgui.h"

Window::Window(const std::string& title, bool closable)
    : title(title), open(false), closable(closable)
{
}

void Window::Render()
{
    if (!open)
    {
        return;
    }

    ImGui::Begin(title.c_str(), closable ? &open : nullptr);

    DrawContent();

    ImGui::End();
}

void Window::Open()
{
    open = true;
}

void Window::Close()
{
    open = false;
}

bool Window::IsOpen() const
{
    return open;
}