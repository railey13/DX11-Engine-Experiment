#include "AppWindow.h"
#include "EngineTime.h"

int main() {
	AppWindow::initialize();
	EngineTime::initialize();

	AppWindow* runningApp = AppWindow::get();

	runningApp->createGraphicsWindow();
	runningApp->onCreate();

	while (runningApp->isRun()) {
		runningApp->broadcast();
	}
	return 0;
}