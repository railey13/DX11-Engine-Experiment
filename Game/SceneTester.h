#pragma once
#include <DX3D/All.h>

class SceneTester : public Game {
public:
	SceneTester();
	~SceneTester();
public:
	virtual void onCreate();
	virtual void onUpdate(f32 deltaTime);
};

