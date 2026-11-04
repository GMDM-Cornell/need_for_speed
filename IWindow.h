#pragma once

class IWindow
{
public:
    virtual ~IWindow() = default;

    virtual void Render() = 0; //Virtual makes sure it can be overwriten and =0 makes it a pure virtual class
    virtual void Open() = 0;
    virtual void Close() = 0;
    virtual bool IsOpen() const = 0;
};