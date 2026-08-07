#include <DX3D/GameObject/RigidBodyComponent.h>
#include <DX3D/GameObject/GameObject.h>
#include <DX3D/GameObject/TransformComponent.h>
#include <DX3D/Game/World.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Physics/PhysicsEngine.h>	

RigidBodyComponent::RigidBodyComponent() {

}

RigidBodyComponent::~RigidBodyComponent() {
	PhysicsEngine* p = m_gameobject->getWorld()->getGame()->getPhysicsEngne();
	p->removeComponent(this);
	if (m_rigidBody) {
		p->getPhysicsWorld()->destroyRigidBody(m_rigidBody);
		m_rigidBody = nullptr;
	}
}

void RigidBodyComponent::onCreateInternal() {
	auto physics = m_gameobject->getWorld()->getGame()->getPhysicsEngne();
	physics->addComponent(this);

	auto transform = m_gameobject->getTransform();

	Vector3D pos = transform->getPosition();
	rp3d::Vector3 p(pos.m_x, pos.m_y, pos.m_z);

	Quaternion rot = transform->getRotation();
	rp3d::Quaternion q(rot.m_x, rot.m_y, rot.m_z, rot.m_w);

	rp3d::Transform physicsTransform(p, q);

	m_rigidBody = physics->getPhysicsWorld()->createRigidBody(physicsTransform);
	m_rigidBody->setType(rp3d::BodyType::DYNAMIC);
}

void RigidBodyComponent::onActivate() {
	m_active = true;
	m_rigidBody->setIsActive(true);
}

void RigidBodyComponent::onDeactivate() {
	m_active = false;
	m_rigidBody->setIsActive(false);
}

void RigidBodyComponent::setBodyType(RBType type) {
	switch (type) {
		case RBType::Static: {
			m_rigidBody->setType(rp3d::BodyType::STATIC);
			break;
		}
		case RBType::Dynamic: {
			m_rigidBody->setType(rp3d::BodyType::DYNAMIC);
			break;
		}
		case RBType::Kinematic: {
			m_rigidBody->setType(rp3d::BodyType::KINEMATIC);
			break;
		}
	}

	m_type = type;
	m_rigidBody->setUserData(this);
}

void RigidBodyComponent::setMass(f32 mass) {
	m_rigidBody->setMass(mass);
}

void RigidBodyComponent::enableGravity(bool flag) {
	m_rigidBody->enableGravity(flag);
}

void RigidBodyComponent::applyLocalForceAtCenterOfMass(const Vector3D& force) {
	m_rigidBody->applyLocalForceAtCenterOfMass(rp3d::Vector3(force.m_x, force.m_y, force.m_z));
}

void RigidBodyComponent::applyWorldForceAtCenterOfMass(const Vector3D& force) {
	m_rigidBody->applyWorldForceAtCenterOfMass(rp3d::Vector3(force.m_x,force.m_y, force.m_z));
}

void RigidBodyComponent::syncPhysicsToTransform() {
	const rp3d::Transform& physicsTransform = m_rigidBody->getTransform();

	rp3d::Vector3 pos = physicsTransform.getPosition();
	rp3d::Quaternion rot = physicsTransform.getOrientation();

	auto transform = m_gameobject->getTransform();
	transform->setPosition(Vector3D(pos.x, pos.y, pos.z));

	transform->setRotation(Quaternion(rot.x, rot.y, rot.z, rot.w));
}

void RigidBodyComponent::syncTransformToPhysics(const Vector3D& position, const Quaternion& rotation) {
	if (!m_rigidBody || m_rigidBody->getType() == rp3d::BodyType::STATIC) return;

	const rp3d::Transform& physicsTransform = m_rigidBody->getTransform();
	rp3d::Vector3 pos = physicsTransform.getPosition();
	rp3d::Quaternion rot = physicsTransform.getOrientation();

	auto transform = m_gameobject->getTransform();
	transform->setPosition(Vector3D(pos.x, pos.y, pos.z));
	transform->setRotation(Quaternion(rot.x, rot.y, rot.z, rot.w));
}

void RigidBodyComponent::updateTransform(const Vector3D& position, const Quaternion& rotation) {
	if (!m_rigidBody) return;

	rp3d::Vector3 p(position.m_x, position.m_y, position.m_z);
	rp3d::Quaternion q(rotation.m_x, rotation.m_y, rotation.m_z, rotation.m_w);

	m_rigidBody->setTransform(rp3d::Transform(p, q));
}

void RigidBodyComponent::setFreezeY(bool flag) {
	if (flag) {
		m_rigidBody->setAngularLockAxisFactor(rp3d::Vector3(0, 1, 0));
	}
	else {
		m_rigidBody->setAngularLockAxisFactor(rp3d::Vector3(1, 1, 1));
	}
	
	m_freezeY = flag;
}

void RigidBodyComponent::setActive(bool flag) {
	m_active = flag;
	m_rigidBody->setIsActive(flag);
}
