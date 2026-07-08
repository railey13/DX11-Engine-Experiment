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

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"Engine/VertexShader.hlsl", "vsmain", &vs_byte_code, &vs_size);
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(vs_byte_code, vs_size);

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"Engine/PixelShader.hlsl", "psmain", &ps_byte_code, &ps_size);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(ps_byte_code, ps_size);
}

AppWindow::AppWindow() {
	
}

AppWindow::~AppWindow() {
	sharedInstance = nullptr;
}

void AppWindow::onCreate() {
	/*Window::onCreate();*/
	InputSystem::get()->addListener(this);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); 
	(void)io;	

	ImGui::StyleColorsDark();
	
	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX11_Init(GraphicsEngine::get()->getRenderSystem()->getD11Device(), 
						GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->getContext());
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

	DrawCredits();

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

	m_window_width = width;
	m_window_height = height;

	if (m_swap_chain) {
		m_swap_chain->resize(m_window_width, m_window_height);
	}
}

void AppWindow::onKeyDown(i32 key) {

}

void AppWindow::onKeyUp(i32 key) {

}

void AppWindow::onMouseMove(const Point& mouse_pos) {

}

void AppWindow::onLeftMouseDown(const Point& mouse_pos) {

}

void AppWindow::onLeftMouseUp(const Point& mouse_pos) {

}

void AppWindow::onRightMouseDown(const Point& mouse_pos) {

}

void AppWindow::onRightMouseUp(const Point& mouse_pos) {

}

void AppWindow::SpawnObject(AGameObject* object) {
	f32 spawnDistance = 1.0f; 
	Vector3D spawnPos = m_sceneCamera.getPosition() + m_sceneCamera.getForwardDirection() * spawnDistance;

	object->setPosition(spawnPos);
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

void AppWindow::DrawCredits() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("GameObject")) {
			if (ImGui::MenuItem("Cube")) {
				auto obj = new Cube(vs_byte_code, vs_size);
				SpawnObject(obj);
			}
			if (ImGui::MenuItem("Sphere")) {
				auto obj = new Sphere(vs_byte_code, vs_size);
				SpawnObject(obj);
			}
			if (ImGui::MenuItem("Plane")) {
				auto obj = new Plane(vs_byte_code, vs_size);
				SpawnObject(obj);
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools")) {
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	
	if (m_tool_active) {
		if (ImGui::Begin("Credits", &m_tool_active, ImGuiWindowFlags_NoCollapse)) {
			ImGui::Text("About");
			ImGui::Text("Scene Editor v0.1");
			ImGui::Text("Developed By: Nikos Bumanglag");
			ImGui::Text("Acknowledgements");

			static char ack_text[2048] =
				"Engine Structure and pipeline is based on PardCode's C++ 3D Game Tutorial Series:\n"
				"https://github.com/PardCode/CPP-3D-Game-Tutorial-Series\n"
				"Scene Editor UI built using Dear ImGui by Omar Cornut and contributors:\n"
				"https://github.com/ocornut/imgui\n";

			ImGui::InputTextMultiline(
				"acknowledgements",
				ack_text,
				IM_ARRAYSIZE(ack_text),
				ImVec2(-FLT_MIN, 180),
				ImGuiInputTextFlags_ReadOnly
			);

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Close").x) * 0.5f);
			if (ImGui::Button("Close")) {
				std::cout << "Close" << std::endl;
				m_tool_active = false;
				std::cout << m_tool_active << std::endl;

			}
		}

		ImGui::End();

	}
	
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

