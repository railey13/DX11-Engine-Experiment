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
			AppWindow* runningApp = AppWindow::get();

			runningApp->createGraphicsWindow();
			runningApp->onCreate();

			while (runningApp->isRun());
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