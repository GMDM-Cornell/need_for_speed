#pragma once
#include <string>

class Window {
protected:
    std::string title;
    bool open = true;
    bool closable = false;

    virtual void DrawContent() = 0;

public:
    virtual void Reload() {}
    Window(const std::string& windowTitle);
    virtual ~Window() = default;

    void Render();
};
