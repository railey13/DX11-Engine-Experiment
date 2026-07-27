#include <DX3D/Game/EditorCamera.h>
#include <DX3D/Input/InputSystem.h>

EditorCamera::EditorCamera() {
	m_name = "EditorCamera";
}

EditorCamera::~EditorCamera() {

}

void EditorCamera::onCreate() {
	createComponent<TransformComponent>();
	createComponent<CameraComponent>();

	m_camera = getComponent<CameraComponent>();

	m_camera->setSensitivity(0.005f);
	m_camera->setFarPlane(1000.f);

	getTransform()->setPosition(Vector3D(0, 0, -5));
}

void EditorCamera::update(f32 deltaTime) {	

	m_forward = 0.0f;
	m_strafe = 0.0f;

	if (!getInputSystem()->isKeyDown(Key::RightMouseButton)) {
		return;
	}

	if (getInputSystem()->isKeyDown(Key::W)) {
		m_forward = 1;
	}
	if (getInputSystem()->isKeyDown(Key::A)) {
		m_strafe = -1;
	}
	if (getInputSystem()->isKeyDown(Key::S)) {
		m_forward = -1;
	}
	if (getInputSystem()->isKeyDown(Key::D)) {
		m_strafe = 1;
	}

	Matrix4x4 world;
	getTransform()->getWorldMatrix(world);

	auto pos = getTransform()->getPosition();
	pos += world.getZDirection() * m_forward * 3.0f * deltaTime;
	pos += world.getXDirection() * m_strafe * 3.0f * deltaTime;

	auto deltaPos = getInputSystem()->getDeltaMousePosition();
	auto rot = getTransform()->getRotation();
	rot += Vector3D(deltaPos.m_y * m_camera->getSensitivity(), deltaPos.m_x * m_camera->getSensitivity(), 0);

	getTransform()->setPosition(pos);
	getTransform()->setRotation(rot);
}
