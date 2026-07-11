#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "Window.h"
#include "DeviceContext.h"
#include "AUIScreen.h"

#include "../IMGUI/imgui.h"
#include "../IMGUI/backends/imgui_impl_dx11.h"
#include "../IMGUI/backends/imgui_impl_win32.h"

namespace UINames {
	const std::string MAIN_MENU_BAR = "MAIN_MENU_BAR";
	const std::string HIERARCHY_SCREEN = "HIERARCHY_SCREEN";
	const std::string ABOUT_SCREEN = "ABOUT_SCREEN";
}

class UIManager {
public:
	static UIManager* get();

	static void initialize(HWND hwnd);
	static void destroy();

	void drawAllUI();
	
	AUIScreen* getUIScreen(std::string name);

	void setUIScreenActive(std::string name, bool flag);

	bool isUIScreenActive(std::string name);
private:
	UIManager(HWND hwnd);

	UIManager(UIManager const&) {};
	UIManager& operator=(UIManager const&) {};
	static UIManager* sharedInstance;

	~UIManager();
private:
	std::vector<AUIScreen*> m_ui_list;
	std::unordered_map<std::string, AUIScreen*> m_ui_table;
};

