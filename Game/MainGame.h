#pragma once
#include <DX3D/All.h>

class MainGame : public Game {
public:
	MainGame();
	~MainGame();
public:
	virtual void onCreate();
	virtual void onUpdate(f32 deltaTime);
private:
	GameObject* m_gameObject = nullptr;
	f32 m_elapsedSeconds = 0.0f;
	f32 rotation = 0.0f;
};

