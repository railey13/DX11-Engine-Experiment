#pragma once
#include "AUIScreen.h"

class AboutScreen : public AUIScreen {
public:
	AboutScreen();
	~AboutScreen();

	// Inherited via AUIScreen
	void draw() override; 

};

