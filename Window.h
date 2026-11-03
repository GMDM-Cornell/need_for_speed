#pragma once

#include "IWindow.h"
#include <string>

class Window : public IWindow
{
protected:
    std::string title;
    bool open;
    bool closable;

public:
    Window(const std::string& title, bool closable = true);
    virtual ~Window() = default;

    void Render() override;

    virtual void DrawContent() = 0;

    void Open() override;
    void Close() override;

    bool IsOpen() const override;
};