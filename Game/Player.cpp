#include "Player.h"
#include <DX3D/Resource/PrimitiveFactory.h>

Player::Player() {
	m_name = "Player";
}

Player::~Player() {

}

void Player::onCreate() {
	m_camera = m_world->createGameObject<GameObject>();
	m_camera->setName("Camera");
	auto c = m_camera->createComponent<CameraComponent>();

	c->setSensitivity(0.005f);
	c->setFarPlane(1000.f);
	m_camera->setParent(this);
	m_camera->getTransform()->setPosition(Vector3D(0,0.2f,0.1f));
}

void Player::update(f32 deltaTime) {
	auto game = getWorld()->getGame();

	if (game->m_useEditorCamera && game->isPlay()) {
		return;
	}

	if (getInputSystem()->isKeyUp(Key::Escape)) {
		game->getInputSystem()->toggleLockCursor();
		game->getInputSystem()->toggleCursorVisible();
	}

	if (!getInputSystem()->isCursorLocked()) {
		//m_rb->getRawRigidBody()->setLinearVelocity(rp3d::Vector3(0,0,0));
		return;
	}

	m_forward = 0.0f;
	m_strafe = 0.0f;

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

	auto rb = getComponent<RigidBodyComponent>()->getRawRigidBody();

	Matrix4x4 world;
	getTransform()->getWorldMatrix(world);

	Vector3D moveDir = (world.getZDirection() * m_forward) + (world.getXDirection() * m_strafe);

	rp3d::Vector3 currentVel = rb->getLinearVelocity();
	rb->getLinearVelocity();

	rp3d::Vector3 l(moveDir.m_x * speed, currentVel.y, moveDir.m_z * speed);

	rb->setLinearVelocity(l);

	if (getInputSystem()->isKeyDown(Key::Space)) {
		currentVel = rb->getLinearVelocity();

		rp3d::Vector3 j((rp3d::Vector3(currentVel.x, jumpForce, currentVel.z)));
		rb->setLinearVelocity(j);
	}

	auto deltaPos = getInputSystem()->getDeltaMousePosition();

	auto camera = m_camera->getComponent<CameraComponent>();

	m_yaw += deltaPos.m_x * camera->getSensitivity();
	m_pitch += deltaPos.m_y * camera->getSensitivity();

	const float maxPitch = 1.49f, minPitch = -1.49f;
	m_pitch = std::max(minPitch, std::min(maxPitch, m_pitch));

	Quaternion yawQuat = Quaternion::fromAxisAngle(Vector3D(0, 1, 0), m_yaw);
	Quaternion pitchQuat = Quaternion::fromAxisAngle(Vector3D(1, 0, 0), m_pitch);

	getComponent<RigidBodyComponent>()->updateTransform(getTransform()->getPosition(), yawQuat);
	m_camera->getTransform()->setRotation(pitchQuat);
}
