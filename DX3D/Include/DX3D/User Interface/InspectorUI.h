#pragma once
#include <DX3D/User Interface/UI.h>

class InspectorUI : public UI{
public:
	InspectorUI(UIHandler* handler);
	~InspectorUI();

	// Inherited via UI
	void draw() override;
private:
	void setButton(const char* label, const wchar_t* path, GameObject* obj, float width);
private:
	char m_nameBuffer[128] = "";
	f32 m_transform_speed = 0.05f;
	
};

