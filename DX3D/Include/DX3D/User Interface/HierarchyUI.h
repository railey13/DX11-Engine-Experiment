#pragma once
#include <DX3D/User Interface/UI.h>

class HierarchyUI : public UI{
public:
	HierarchyUI(UIHandler* handler);
	~HierarchyUI();

	// Inherited via UI
	void draw() override;
private:
	bool m_isSelected = false;
};

