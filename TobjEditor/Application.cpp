#include <memory>
#include "Application.h"

#define	 WIDTH  450
#define	 HEIGHT 120
#define  NAME   "Tobj Editor"

Application::Application() 
	:
	wnd(std::make_unique<Window>(WIDTH, HEIGHT, NAME))
	{}

int Application::Run() {
	while (true) {
		if (const auto ecode = wnd->ProcessMessages()) {
			return *ecode;
		}
	}
}
