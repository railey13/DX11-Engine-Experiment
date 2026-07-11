#pragma once
#include "AUIScreen.h"

class MainMenuScreen  : public AUIScreen{
public:
	MainMenuScreen();
	~MainMenuScreen();
	
	// Inherited via AUIScreen
	void draw() override;
private:
	friend class SpawnObjectCommand;
};

