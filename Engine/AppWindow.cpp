#include "AppWindow.h"
#include "Vector3D.h"
#include "iostream"
#include "../Settings.h"

AppWindow* AppWindow::sharedInstance = NULL;

AppWindow* AppWindow::get() {
	return sharedInstance;
}

void AppWindow::initialize() {
	if (sharedInstance != NULL) throw std::exception("App Window already exists");

	sharedInstance = new AppWindow();
}

void AppWindow::destroy() {
	if (sharedInstance == NULL) return;

	delete sharedInstance;
}

void AppWindow::createGraphicsWindow() {
	m_sceneCamera = Camera();

	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, Settings::WindowWidth, Settings::WindowHeight);

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"Engine\\VertexShader.hlsl", "vsmain", &vs_byte_code, &vs_size);
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(vs_byte_code, vs_size);

	//GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"Engine\\PixelShader.hlsl", "psmain", &ps_byte_code, &ps_size);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(ps_byte_code, ps_size);

	//GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	AGameObject* obj = new Plane(vs_byte_code, vs_size);
	m_objects.push_back(obj);

	obj = new Cube(vs_byte_code, vs_size);
	m_objects.push_back(obj);

	obj = new Sphere(vs_byte_code, vs_size);
	obj->setPosition(Vector3D(1,1,0));
	m_objects.push_back(obj);


}

AppWindow::AppWindow() {
	
}

AppWindow::~AppWindow() {
	sharedInstance = nullptr;
}

void AppWindow::onCreate() {
	/*Window::onCreate();*/
	InputSystem::get()->addListener(this);
}

void AppWindow::onUpdate() {
	/*Window::onUpdate();*/
	//screen
	InputSystem::get()->update();
	GraphicsEngine* graphEngine = GraphicsEngine::get();

	f32 deltaTime = EngineTime::getDeltaTime();

	m_sceneCamera.update(deltaTime);

	graphEngine->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	graphEngine->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);
	
	graphEngine->getRenderSystem()->getImmediateDeviceContext()->ClearRenderTargetColor(this->m_swap_chain, 0, 0, 0, 1);

	graphEngine->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(Settings::WindowWidth, Settings::WindowHeight);

	for (auto obj : m_objects) {
		obj->update(deltaTime);
		obj->draw(m_vs, m_ps, m_sceneCamera.getViewMatrix(), m_sceneCamera.getProjectionMatrix());
	}

	m_swap_chain->present(false);
}

void AppWindow::onDestroy() {
	Window::onDestroy();

	InputSystem::get()->removeListener(this);
	m_objects.clear();

	GraphicsEngine::get()->destroy();
}

void AppWindow::onFocus() {
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus() {	
	InputSystem::get()->removeListener(this);
}

void AppWindow::onResize(ui32 width, ui32 height) {
	if (width == 0 || height == 0) return;

	std::cout << width << ", " << height << std::endl;

	Settings::WindowWidth = width;
	Settings::WindowHeight = height;

	if (m_swap_chain) {
		m_swap_chain->resize(width, height);
	}
}

void AppWindow::onKeyDown(i32 key) {

}

void AppWindow::onKeyUp(i32 key) {
	/*switch (key) {
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
	}*/
}

void AppWindow::onMouseMove(const Point& mouse_pos) {
	//objects.front()->RotateX((mouse_pos.m_y - (Settings::WindowHeight/ 2.0f)) * 0.1);
	//objects.front()->RotateY((mouse_pos.m_x - (Settings::WindowWidth / 2.0f)) * 0.1);
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos) {
	if (m_objects.empty()) return;
	//objects.front()->Scale(0.5f);
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos) {
	if (m_objects.empty()) return;
	//objects.front()->Scale(1.f);
}

void AppWindow::onRightMouseDown(const Point& mouse_pos) {
	if (m_objects.empty()) return;
	//objects.front()->Scale(2.0f);
}

void AppWindow::onRightMouseUp(const Point& mouse_pos) {
	if (m_objects.empty()) return;
	//objects.front()->Scale(1.0f);
}

void AppWindow::SpawnObject() {
	//i32 colorID = (objects.size() % 7) + 1;  // cycles color (look in testobject source file for color reference)

	auto object = new Cube(vs_byte_code, vs_size);
	m_objects.push_back(object);
}

void AppWindow::DestroyObject() {
	if (m_objects.empty()) return;
	delete m_objects.back();
	m_objects.pop_back();
}

void AppWindow::DestroyAllObjects() {
	if (m_objects.empty()) return;
	while (!m_objects.empty()) {
		delete m_objects.back();
		m_objects.pop_back();
	}
}

