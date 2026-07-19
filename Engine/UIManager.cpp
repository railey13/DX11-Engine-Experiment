#include "UIManager.h"
#include "GraphicsEngine.h"
#include "AppWindow.h"

#include "MainBarUI.h"
#include "AboutUI.h"
#include "HierarchyUI.h"
#include "InspectorUI.h"
#include "MainBarUI.h"

UIManager* UIManager::sharedInstance = NULL;

UIManager* UIManager::get() {
	return sharedInstance;
}

void UIManager::initialize(HWND hwnd) {
	sharedInstance = new UIManager(hwnd);
}

void UIManager::destroy() {
	if (sharedInstance == NULL) return;

	delete sharedInstance;
}

UIManager::UIManager(HWND hwnd) {
	IMGUISetUp(hwnd);
	registerUI<HierarchyUI>();
	registerUI<InspectorUI>();
	registerUI<MainBarUI>();
	registerUI<ViewportUI>();
}

UIManager::~UIManager() {
	m_ui_table.clear();
}

void UIManager::IMGUISetUp(HWND hwnd) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsDark();

	GraphicsEngine* graphEngine = GraphicsEngine::get();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(graphEngine->getRenderSystem()->getD11Device(),
		graphEngine->getRenderSystem()->getImmediateDeviceContext()->getContext());
}

void UIManager::registerUIInternal(UI* ui, size_t id) {
	auto compPtr = std::unique_ptr<UI>(ui);
	m_ui_table.emplace(id, std::move(compPtr));
	ui->m_typeId = id;
}

UI* UIManager::getUIInternal(size_t id) const {
	auto it = m_ui_table.find(id);

	if (it == m_ui_table.end()) return nullptr;

	return it->second.get();
}

void UIManager::draw() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (auto& [type, ui] : m_ui_table) {
		ui->draw();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
