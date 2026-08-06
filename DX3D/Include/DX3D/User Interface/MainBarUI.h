#pragma once
#include <DX3D/User Interface/UI.h>

class MainBarUI  : public UI{
public:
	MainBarUI(UIHandler* handler);
	~MainBarUI();
	
	// Inherited via UI
	void draw() override;
private:
	ResourceManager* m_resource = nullptr;
	bool m_edit = true;
	bool m_pause = false;
};

