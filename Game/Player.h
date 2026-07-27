#pragma once
#include <DX3D/All.h>
class Player : public GameObject {
public:
	Player();
	virtual ~Player();
protected:
	virtual void onCreate();
	virtual void update(f32 deltaTime);
private:
	GameObject* m_gameObject = nullptr;
	f32 m_elapsedSeconds = 0.0f;

	CameraComponent* m_camera;

	f32 m_forward = 0.0f;
	f32 m_strafe = 0.0f;
};

