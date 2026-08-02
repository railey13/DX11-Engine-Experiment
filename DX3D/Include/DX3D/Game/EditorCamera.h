#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/All.h>

class EditorCamera : public GameObject {
public:
	EditorCamera();
	~EditorCamera();
protected:
	void onCreate();
	void update(f32 deltaTime);
private:
	Game* m_game = nullptr;
	CameraComponent* m_camera;

	f32 m_forward = 0.0f;
	f32 m_strafe = 0.0f;
private:
	friend class Game;
};


