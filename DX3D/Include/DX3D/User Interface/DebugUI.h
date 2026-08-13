#pragma once
#include <DX3D/User Interface/UI.h>

class DebugUI : public UI {
public:
	DebugUI(UIHandler* handler);
	~DebugUI();

	// Inherited via UI
	void draw() override;
private:
	f32 m_fpsUpdateTimer = 0.0f;
	f32 m_displayedFps = 0.0f;
	i32 m_frameCount = 0;
};