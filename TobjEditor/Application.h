#pragma once
#include "Window.h"
#include "Tobj.h"

class Application {
public:
    Application();
    int Run();
private:
    std::unique_ptr<Window> wnd;
};