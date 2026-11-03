#pragma once

class IWindow
{
public:
    virtual ~IWindow() = default;

    virtual void Render() = 0;
    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual bool IsOpen() const = 0;
};