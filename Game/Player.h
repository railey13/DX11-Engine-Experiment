#pragma once
#include <DX3D/All.h>

class Player : public GameObject {
public:
	Player();
	virtual ~Player();
protected:
	virtual void onCreate();
	virtual void update(f32 deltaTime);

	void onCollisionEnter(GameObject* obj) override;
	void onCollisionStay(GameObject* obj) override;
	void onCollisionExit(GameObject* obj) override;
private:
	GameObject* m_camera = nullptr;

	f32 m_forward = 0.0f;
	f32 m_strafe = 0.0f;
	f32 m_pitch = 0.0f;
	f32 m_yaw = 0.0f;

	f32 jumpForce = 2.0f;
	f32 verticalVelocity = 0.0f;
	f32 speed = 3.0f;
};

