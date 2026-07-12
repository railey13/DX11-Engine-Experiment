#pragma once
#include "AUIScreen.h"
#include "Prerequisites.h"

class InspectorScreen : public AUIScreen{
public:
	InspectorScreen();
	~InspectorScreen();

	// Inherited via AUIScreen
	void draw() override;
private:
	char m_nameBuffer[128] = "";
	f32 m_transform_speed = 0.05f;
	
};

