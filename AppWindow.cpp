#include "AppWindow.h"
#include "Vector3D.h"
#include "InputSystem.h"
#include "iostream"

AppWindow* AppWindow::sharedInstance = NULL;

AppWindow* AppWindow::get() {
	return sharedInstance;
}

void AppWindow::initialize() {
	sharedInstance = new AppWindow();
	sharedInstance->init();
}

void AppWindow::createGraphicsWindow() {
	GraphicsEngine::initialize();
	GraphicsEngine* graphEngine = GraphicsEngine::get();

	m_swap_chain = graphEngine->createSwapChain();

	RECT rc = this->getClientWindowRect();

	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	//auto obj = new TestObject(graphEngine);
	//obj->Translate(Vector3D(-0.35f, 0, 0));
	//obj->speed = 0.0;
	//objects.push_back(obj);

	//obj = new TestObject(graphEngine);
	//obj->Translate(Vector3D(0.35f, 0, 0));
	//obj->speed = 0.7;
	//objects.push_back(obj);

	//obj = new TestObject(graphEngine);
	//obj->Translate(Vector3D(0, 0.5f, 0));
	//obj->speed = 0.6;
	//objects.push_back(obj);

	//obj = new TestObject(graphEngine);
	//obj->Translate(Vector3D(0, -0.5f, 0));
	//obj->speed = 0.5;
	//objects.push_back(obj);

	//{
	//	vertex list[] = {
	//		// X - Y - Z
	//		// FRONT FACE
	//		{Vector3D(-0.2f, -0.2f, -0.2f),	Vector3D(0.0f, 0.0f, 0.0f),		Vector3D(0.0f, 1.0f, 0.0f)}, // POS1
	//		{Vector3D(-0.2f, 0.2f, -0.2f),	Vector3D(1.0f, 1.0f, 0.0f),		Vector3D(1.0f, 0.0f, 0.0f)}, // POS2
	//		{Vector3D(0.2f, 0.2f, -0.2f),	Vector3D(0.0f, 0.0f, 1.0f),		Vector3D(1.0f, 0.0f, 0.0f)}, // POS3 
	//		{Vector3D(0.2f, -0.2f, -0.2f),	Vector3D(1.0f, 1.0f, 1.0f),		Vector3D(1.0f, 0.0f, 0.0f)}, // POS4

	//		// BACK FACE
	//		{Vector3D(0.2f, -0.2f, 0.2f),	Vector3D(0.0f, 0.0f, 0.0f),		Vector3D(0.0f, 1.0f, 0.0f)}, // POS1
	//		{Vector3D(0.2f, 0.2f, 0.2f),	Vector3D(1.0f, 1.0f, 0.0f),		Vector3D(1.0f, 0.0f, 0.0f)}, // POS2
	//		{Vector3D(-0.2f, 0.2f, 0.2f),	Vector3D(0.0f, 0.0f, 1.0f),		Vector3D(1.0f, 0.0f, 0.0f)}, // POS3 
	//		{Vector3D(-0.2f, -0.2f, 0.2f),	Vector3D(1.0f, 1.0f, 1.0f),		Vector3D(1.0f, 0.0f, 0.0f)}, // POS4
	//	};

	//	UINT size_list = ARRAYSIZE(list);

	//	unsigned int index_list[] = {
	//		// FRONT SIDE
	//		0,1,2, // FIRST TRIANGLE
	//		2,3,0, // SECOND TRIANGLE
	//		// BACK SIDE
	//		4,5,6,
	//		6,7,4,
	//		// TOP SIDE
	//		1,6,5,
	//		5,2,1,
	//		// BOTTOM SIDE
	//		7,0,3,
	//		3,4,7,
	//		// RIGHT SIDE
	//		3,2,5,
	//		5,4,3,
	//		// LEFT SIDE
	//		7,6,1,
	//		1,0,7
	//	};

	//	UINT size_index_list = ARRAYSIZE(index_list);

	//	auto obj = new TestObject(list, index_list, size_list, size_index_list, graphEngine);
	//	obj->Translate(Vector3D(0.35,0,0));
	//	obj->speed = 0.5f;
	//	objects.push_back(obj);
	//}

	
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
	//graphEngine->getImmediateDeviceContext()->ClearRenderTargetColor(this->m_swap_chain, 0.388f, 0.525f, 0.804f, 1);
	graphEngine->getImmediateDeviceContext()->ClearRenderTargetColor(this->m_swap_chain,0, 0, 0, 1);

	RECT rc = this->getClientWindowRect();

	graphEngine->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	this->update();

	for (auto obj : objects) {
		obj->render(graphEngine);
	}

	m_swap_chain->present(false);
}

void AppWindow::onDestroy() {
	Window::onDestroy();

	m_swap_chain->release();

	for (auto obj : objects) {
		obj->release();
	}

	objects.clear();

	GraphicsEngine::destroy();
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
		break; 
	case 'S': if (objects.empty()) return; 
		//objects.front()->RotateX(-1);
		break;
	case 'A': if (objects.empty()) return; 
		//objects.front()->RotateY(1);
		break;
	case 'D': if (objects.empty()) return;
		//objects.front()->RotateY(-1);
		break;
	default: break;
	}
}

void AppWindow::onKeyUp(int key) {
	std::cout << key << std::endl;
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
	default: break;
	}
}

void AppWindow::onMouseMove(const Point& delta_mouse_pos) {
	if (objects.empty()) return;
	//objects.front()->RotateX(-delta_mouse_pos.m_y);
	//objects.front()->RotateY(delta_mouse_pos.m_x);
}

void AppWindow::onLeftMouseDown(const Point& delta_mouse_pos) {
	if (objects.empty()) return;
	//objects.front()->Scale(0.5f);
}

void AppWindow::onLeftMouseUp(const Point& delta_mouse_pos) {
	if (objects.empty()) return;
	//objects.front()->Scale(1.f);
}

void AppWindow::onRightMouseDown(const Point& delta_mouse_pos) {
	if (objects.empty()) return;
	//objects.front()->Scale(2.0f);
}

void AppWindow::onRightMouseUp(const Point& delta_mouse_pos) {
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

