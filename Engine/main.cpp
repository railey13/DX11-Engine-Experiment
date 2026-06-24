#include "AppWindow.h"
#include "EngineTime.h"

int main() {
	try {
		GraphicsEngine::initialize();
		InputSystem::initialize();
	}
	catch (...) { return -1; }

	{
		AppWindow::initialize();
		EngineTime::initialize();
		AppWindow* runningApp = AppWindow::get();

		runningApp->createGraphicsWindow();
		runningApp->onCreate();

		while (runningApp->isRun()) {
			runningApp->broadcast();
		}
	}

	GraphicsEngine::destroy();
	InputSystem::destroy();

	return 0;
}