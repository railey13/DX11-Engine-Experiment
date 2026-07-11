#include "UIManager.h"
#include "GraphicsEngine.h"

#include "MainMenuScreen.h"
#include "AboutScreen.h"

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

void UIManager::drawAllUI() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (auto* screen : m_ui_list) {
		screen->draw();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

AUIScreen* UIManager::getUIScreen(std::string name) {
	auto it = m_ui_table.find(name);
	if (it != m_ui_table.end()) {
		return it->second;
	}
	return nullptr;
}

void UIManager::setUIScreenActive(std::string name, bool flag) {
	auto it = m_ui_table.find(name);
	if (it != m_ui_table.end() && it->second) {
		it->second->setActive(flag);
	}
}

bool UIManager::isUIScreenActive(std::string name) {
	auto it = m_ui_table.find(name);
	if (it != m_ui_table.end()) {
		return it->second->m_isActive;
	}

	return false;
}

UIManager::UIManager(HWND hwnd) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(GraphicsEngine::get()->getRenderSystem()->getD11Device(),
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->getContext());

	MainMenuScreen* mainmenuScreen = new MainMenuScreen();
	m_ui_table[UINames::MAIN_MENU_BAR] = mainmenuScreen;
	m_ui_list.push_back(mainmenuScreen);

	AboutScreen* aboutScreen = new AboutScreen();
	m_ui_table[UINames::ABOUT_SCREEN] = aboutScreen;
	m_ui_list.push_back(aboutScreen);
}

UIManager::~UIManager() {

}
