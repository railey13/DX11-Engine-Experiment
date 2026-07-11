#pragma once
#include "AUIScreen.h"

class HierarchyScreen : public AUIScreen{
public:
	HierarchyScreen();
	~HierarchyScreen();

	// Inherited via AUIScreen
	void draw() override;
private:
	bool m_isSelected = false;
};

