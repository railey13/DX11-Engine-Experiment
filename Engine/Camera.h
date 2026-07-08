#pragma once
#include "InputListener.h"
#include "InputSystem.h"
#include "Prerequisites.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "Settings.h"

class Camera : public InputListener {
public:
	Camera();

	void update(f32 deltaTime);

	~Camera();
public:
	// Inherited via InputListener
	void onKeyDown(i32 key) override;
	void onKeyUp(i32 key) override;
	// Inherited via InputListener
	void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;
public:
	Matrix4x4 getViewMatrix() const;
	Matrix4x4 getProjectionMatrix() const;
	Vector3D getForwardDirection() const;
	Vector3D getPosition() const;
private:
	Vector3D m_position;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	f32 m_pitch = 0.0f;
	f32 m_yaw = 0.0f;
	f32 m_forward = 0.0f;
	f32 m_strafe = 0.0f;
	
	f32 m_fov = 1.57f;
	f32 m_znear = 0.1f;
	f32 m_zfar = 100.0f;

	f32 sensitivity = 0.005f;

	Point lastMousePos;
	Point savedMousePos;

	bool ignoreNextMouseMove = false;

	bool moveLock = true;
};

