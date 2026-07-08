#include "Camera.h"

Camera::Camera() {
	InputSystem::get()->addListener(this);
	m_position = Vector3D(-0.3f, 0.5f, -1);
}

void Camera::update(f32 deltaTime) {
	Matrix4x4 world_cam;
	world_cam.setIdentity();

	Matrix4x4 temp;

	temp.setRotationX(m_pitch);
	world_cam *= temp;

	temp.setRotationY(m_yaw);
	world_cam *= temp;

	Vector3D new_pos = m_position
		+ world_cam.getZDirection() * (m_forward * deltaTime)
		+ world_cam.getXDirection() * (m_strafe * deltaTime);

	world_cam.setTranslation(new_pos);
	m_position = new_pos;

	world_cam.inverse();

	m_view = world_cam;
}

Camera::~Camera() {
	InputSystem::get()->removeListener(this);
}

void Camera::onKeyDown(i32 key) {
	if (moveLock) return;
	switch (key) {
		case 'W': m_forward = 1;
			break;
		case 'A': m_strafe = -1;
			break;
		case 'S': m_forward = -1;
			break;
		case 'D': m_strafe = 1;
			break;
	}
}

void Camera::onKeyUp(i32 key) {
	switch (key) {
		case 'W': m_forward = 0;
			break;
		case 'A': m_strafe = 0;
			break;
		case 'S': m_forward = 0;
			break;
		case 'D': m_strafe = 0;
			break;
	}
}

void Camera::onMouseMove(const Point& mouse_pos) {
	if (moveLock) return;
	
	float dx = (mouse_pos.m_x - lastMousePos.m_x);
	float dy = (mouse_pos.m_y - lastMousePos.m_y);

	m_yaw += dx * sensitivity;
	m_pitch += dy * sensitivity;

	// clamp pitch
	const float maxPitch = 1.49f, minPitch = -1.49f;
	m_pitch = std::max(minPitch, std::min(maxPitch, m_pitch));
	lastMousePos = mouse_pos;
}

void Camera::onLeftMouseDown(const Point& mouse_pos) {

}

void Camera::onLeftMouseUp(const Point& mouse_pos) {

}

void Camera::onRightMouseDown(const Point& mouse_pos) {
	//InputSystem::get()->setCursorPosition(Point(Settings::WindowWidth / 2.0f, Settings::WindowHeight / 2.0f));
	InputSystem::get()->showCursor(!InputSystem::get()->isCursorVisible());

	savedMousePos = mouse_pos;

	lastMousePos = mouse_pos;
	moveLock = false;
}

void Camera::onRightMouseUp(const Point& mouse_pos) {
	InputSystem::get()->showCursor(!InputSystem::get()->isCursorVisible());
	moveLock = true;

	InputSystem::get()->setCursorPosition(savedMousePos);
	lastMousePos = savedMousePos;
}

Matrix4x4 Camera::getViewMatrix() const {
	return m_view;
}

Matrix4x4 Camera::getProjectionMatrix() const { 
	Matrix4x4 proj;
	proj.setPerspectiveFovLH(m_fov, ((f32)Settings::WindowWidth / (f32)Settings::WindowHeight), m_znear, m_zfar);

	return proj;
}

Vector3D Camera::getForwardDirection() const {
	Matrix4x4 world_cam;
	world_cam.setIdentity();

	Matrix4x4 temp;
	temp.setRotationX(m_pitch);
	world_cam *= temp;

	temp.setRotationY(m_yaw);
	world_cam *= temp;

	return world_cam.getZDirection();
}

Vector3D Camera::getPosition() const {

	return m_position;
}
