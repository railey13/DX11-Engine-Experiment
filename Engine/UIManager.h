#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "Window.h"

#include "../IMGUI/imgui.h"
#include "../IMGUI/backends/imgui_impl_dx11.h"
#include "../IMGUI/backends/imgui_impl_win32.h"



class UIManager {
public:
	static UIManager* get();

	static void initialize(HWND hwnd);
	static void destroy();
private:
	UIManager(HWND hwnd);

	UIManager(UIManager const&) {};
	UIManager& operator=(UIManager const&) {};
	static UIManager* sharedInstance;

	~UIManager();
private:
	std::vector<std::string> m_ui_list;
	std::unordered_map<std::string, std::string> m_ui_table;
};

