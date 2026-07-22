#include "AppWindow.h"
#include "EngineTime.h"

int main() {
	try {
		GraphicsEngine::initialize();
		InputSystem::initialize();
	}
	catch (...) { return -1; }

	{
		try {
			AppWindow::initialize();
			EngineTime::initialize();

			AppWindow* app = AppWindow::get();

			while (app->isRun()) {
				app->broadcast();
			};
		}
		catch (...) {
			GraphicsEngine::destroy();
			InputSystem::destroy();

			return -1;
		}
	}

	GraphicsEngine::destroy();
	InputSystem::destroy();

	return 0;
}