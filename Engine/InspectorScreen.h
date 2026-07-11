#pragma once
#include "AUIScreen.h"

class InspectorScreen : public AUIScreen{
public:
	InspectorScreen();
	~InspectorScreen();

	// Inherited via AUIScreen
	void draw() override;
private:
	char m_nameBuffer[128] = "";
	//AGameObject* m_lastSelected = nullptr;
};

