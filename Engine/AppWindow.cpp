#include "AppWindow.h"
#include "Vector3D.h"
#include "iostream"
#include "../Settings.h"

#include "UIManager.h"
#include "GameObjectManager.h"
#include "ShaderLibrary.h"

#include "SpawnObjectCommand.h"
#include "DeleteObjectCommand.h"
#include "CloseWindowCommand.h"

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
	m_sceneCamera = new Camera();

	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, Settings::WindowWidth, Settings::WindowHeight);

	ShaderLibrary::initialize();
	GameObjectManager::initialize();

	UIManager::initialize(m_hwnd);
}

AppWindow::AppWindow() {
	
}

AppWindow::~AppWindow() {
	sharedInstance = nullptr;
}

void AppWindow::onCreate() {
	/*Window::onCreate();*/
	InputSystem::get()->addListener(this);

	m_invoker.bindCommand((int)Action::SpawnCube, [this]() { return new SpawnObjectCommand(this, GameObjectManager::CUBE); });
	m_invoker.bindCommand((int)Action::SpawnSphere, [this]() { return new SpawnObjectCommand(this, GameObjectManager::SPHERE); });
	m_invoker.bindCommand((int)Action::SpawnPlane, [this]() { return new SpawnObjectCommand(this, GameObjectManager::PLANE); });

	m_invoker.bindCommand((int)Action::DeleteSelectedObject, [this]() {
		return new DeleteObjectCommand(this, GameObjectManager::get()->getSelectedObject());
	});

	m_invoker.bindCommand((int)Action::CloseWindow, [this]() { return new CloseWindowCommand(this); });
}

void AppWindow::onUpdate() {
	/*Window::onUpdate();*/
	//screen
	InputSystem::get()->update();
	DeviceContextPtr context = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();

	m_sceneCamera->update(EngineTime::getDeltaTime());

	context->ClearRenderTargetColor(this->m_swap_chain, 0.55f, 0.68f, 0.76f, 1);
	context->setViewportSize(m_window_width, m_window_height);

	GameObjectManager::get()->updateObjects();
	GameObjectManager::get()->renderObjects(m_sceneCamera->getViewMatrix(), m_sceneCamera->getProjectionMatrix());

	UIManager::get()->drawAllUI();

	m_swap_chain->present(false);
}

void AppWindow::onDestroy() {
	Window::onDestroy();

	InputSystem::get()->removeListener(this);
	ShaderLibrary::get()->destroy();
	GameObjectManager::get()->destroy();
	GraphicsEngine::get()->destroy();
}

void AppWindow::onFocus() {
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus() {	
	InputSystem::get()->removeListener(this);
}

void AppWindow::onResize() {
	RECT rc = this->getClientWindowRect();

	m_window_width = rc.right - rc.left;
	m_window_height = rc.bottom - rc.top;

	if (m_window_width == 0 || m_window_height == 0) return;
	
	if (m_swap_chain) {
		m_swap_chain->resize(rc.right, rc.bottom);
	}

	m_sceneCamera->setAspect((f32)m_window_width, (f32)m_window_height);
	onUpdate();
}

void AppWindow::onKeyDown(i32 key) {

}

void AppWindow::onKeyUp(i32 key) {
	// return if any input field is highlighted
	if (ImGui::GetIO().WantCaptureKeyboard) return;
	// temporary inputs to test textures
	AGameObject* obj = GameObjectManager::get()->getSelectedObject();
	TextureManager* texture = GraphicsEngine::get()->getTextureManager();
	switch (key) {
		case '0': 
			if (obj)
				obj->setTexture(texture->createTextureFromFile(L"Assets/Textures/white.png"));
			break;
		case '1':
			if (obj)
				obj->setTexture(texture->createTextureFromFile(L"Assets/Textures/furina.png"));
			break;
		case '2':
			if (obj)
				obj->setTexture(texture->createTextureFromFile(L"Assets/Textures/CartethyiaPuppet.gif"));
			break;
		case '3':
			if (obj)
				obj->setTexture(texture->createTextureFromFile(L"Assets/Textures/MornyeThinking.gif"));
			break;
		case '4':
			if(obj)
				obj->setTexture(texture->createTextureFromFile(L"Assets/Textures/AemeathGame.gif"));
			break;
		case 90: m_invoker.undo();
			break;
		case 89: m_invoker.redo();
			break;
		case VK_DELETE: 
			if (obj)
				m_invoker.executeCommand((int)Action::DeleteSelectedObject);
	}
}

void AppWindow::onMouseMove(const Point& mouse_pos) {

}

void AppWindow::onLeftMouseDown(const Point& mouse_pos) {
	if (ImGui::GetIO().WantCaptureMouse) return;

	GameObjectManager::get()->setSelectedObject(nullptr);
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos) {

}

void AppWindow::onRightMouseDown(const Point& mouse_pos) {
	GameObjectManager::get()->setSelectedObject(nullptr);
}

void AppWindow::onRightMouseUp(const Point& mouse_pos) {

}



