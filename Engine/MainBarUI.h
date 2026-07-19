#pragma once
#include "UI.h"

class MainBarUI  : public UI{
public:
	MainBarUI();
	~MainBarUI();
	
	// Inherited via UI
	void draw() override;
private:
	friend class SpawnObjectCommand;
};

