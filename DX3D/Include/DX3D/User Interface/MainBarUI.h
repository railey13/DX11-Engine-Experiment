#pragma once
#include <DX3D/User Interface/UI.h>

class MainBarUI  : public UI{
public:
	MainBarUI(UIHandler* handler);
	~MainBarUI();
	
	// Inherited via UI
	void draw() override;
private:
	ResourceManager* resource = nullptr;
private:
	friend class SpawnObjectCommand;
};

