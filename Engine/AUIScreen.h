#pragma once
#include <string>
#include "../IMGUI/imgui.h"
#include "../IMGUI/backends/imgui_impl_dx11.h"
#include "../IMGUI/backends/imgui_impl_win32.h"

class UIManager;

class AUIScreen {
protected:
	AUIScreen(std::string name);
	~AUIScreen();

	std::string getName();
	virtual void draw() = 0;

	void setActive(bool flag);

protected:
	std::string m_name;
	bool m_isActive = false;

	friend class UIManager;
};

