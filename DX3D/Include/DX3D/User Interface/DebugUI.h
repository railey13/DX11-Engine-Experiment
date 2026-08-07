#pragma once
#include <DX3D/User Interface/UI.h>

class DebugUI : public UI {
public:
	DebugUI(UIHandler* handler);
	~DebugUI();

	// Inherited via UI
	void draw() override;
};