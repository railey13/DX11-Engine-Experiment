#pragma once
#include "UI.h"

class HierarchyUI : public UI{
public:
	HierarchyUI();
	~HierarchyUI();

	// Inherited via UI
	void draw() override;
private:
	bool m_isSelected = false;
};

