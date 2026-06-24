#include "AppWindow.h"
#include "Vector3D.h"
#include "InputSystem.h"
#include "iostream"
#include "../Settings.h"

AppWindow* AppWindow::sharedInstance = NULL;

AppWindow* AppWindow::get() {
	return sharedInstance;
}

void AppWindow::initialize() {
	sharedInstance = new AppWindow();
	sharedInstance->init();
}

void AppWindow::createGraphicsWindow() {
	GraphicsEngine* graphEngine = GraphicsEngine::get();

	m_swap_chain = graphEngine->getRenderSystem()->createSwapChain(this->m_hwnd, Settings::WindowWidth, Settings::WindowHeight);

	RECT rc = this->getClientWindowRect();
}

AppWindow::AppWindow() {
	
}

void AppWindow::update() {
	GraphicsEngine* graphEngine = GraphicsEngine::get();
	for (auto obj : objects) {
		obj->update(graphEngine, this->getClientWindowRect());
	}
}

AppWindow::~AppWindow() {

}

void AppWindow::onCreate() {
	/*Window::onCreate();*/
	InputSystem::initialize();
	InputSystem* inputSystem = InputSystem::get();

	inputSystem->addListener(this);
}

void AppWindow::onUpdate() {
	/*Window::onUpdate();*/
	//screen
	InputSystem* inputSystem = InputSystem::get();
	inputSystem->update();

	GraphicsEngine* graphEngine = GraphicsEngine::get();
	
	graphEngine->getRenderSystem()->getImmediateDeviceContext()->ClearRenderTargetColor(this->m_swap_chain, 0, 0, 0, 1);

	RECT rc = this->getClientWindowRect();

	graphEngine->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	this->update();

	for (auto obj : objects) {
		obj->render(graphEngine);
	}

	m_swap_chain->present(false);
}

void AppWindow::onDestroy() {
	Window::onDestroy();

	objects.clear();

	GraphicsEngine::get()->destroy();
}

void AppWindow::onFocus() {
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus() {	
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key) {
	switch (key) {
	case 'W': if (objects.empty()) return;
		//objects.front()->RotateX(1);
		objects.front()->TranslateForward(1);
		break; 
	case 'S': if (objects.empty()) return; 
		//objects.front()->RotateX(-1);
		objects.front()->TranslateForward(-1);
		break;
	case 'A': if (objects.empty()) return; 
		//objects.front()->RotateY(1);
		objects.front()->TranslateSideward(-1);
		break;
	case 'D': if (objects.empty()) return;
		//objects.front()->RotateY(-1);
		objects.front()->TranslateSideward(1);
		break;
	default: break;
	}
}

void AppWindow::onKeyUp(int key) {
	std::cout << key << std::endl;
	if (!objects.empty()) {
		objects.front()->TranslateForward(0);
		objects.front()->TranslateSideward(0);
	}
	switch (key) {
	case VK_SPACE: this->SpawnObject();
		std::cout << "SPACE" << std::endl;
		break;
	case VK_BACK: this->DestroyObject();;
		std::cout << "BACK" << std::endl;
		break;
	case VK_DELETE: this->DestroyAllObjects();
		std::cout << "DELETE" << std::endl;
		break;
	case VK_ESCAPE: m_is_run = false;
		std::cout << "ESCAPE" << std::endl;
		break;
	case 'L': InputSystem::get()->showCursor(!InputSystem::get()->isCursorVisible());
	default: break;
	}
}

void AppWindow::onMouseMove(const Point& mouse_pos) {
	if (objects.empty()) return;

	objects.front()->RotateX((mouse_pos.m_y - (Settings::WindowHeight/ 2.0f)) * 0.1);
	objects.front()->RotateY((mouse_pos.m_x - (Settings::WindowWidth / 2.0f)) * 0.1);

	InputSystem::get()->setCursorPosition(Point(Settings::WindowWidth/2.0f, Settings::WindowHeight/2.0f));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos) {
	if (objects.empty()) return;
	//objects.front()->Scale(0.5f);
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos) {
	if (objects.empty()) return;
	//objects.front()->Scale(1.f);
}

void AppWindow::onRightMouseDown(const Point& mouse_pos) {
	if (objects.empty()) return;
	//objects.front()->Scale(2.0f);
}

void AppWindow::onRightMouseUp(const Point& mouse_pos) {
	if (objects.empty()) return;
	//objects.front()->Scale(1.0f);
}

void AppWindow::SpawnObject() {
	int colorID = (objects.size() % 7) + 1;  // cycles color (look in testobject source file for color reference)
	auto object = new TestObject(colorID);
	objects.push_back(object);
}

void AppWindow::DestroyObject() {
	if (objects.empty()) return;
	objects.back()->release();
	objects.pop_back();
}

void AppWindow::DestroyAllObjects() {
	if (objects.empty()) return;
	while (!objects.empty()) {
		objects.back()->release();
		objects.pop_back();
	}
}

