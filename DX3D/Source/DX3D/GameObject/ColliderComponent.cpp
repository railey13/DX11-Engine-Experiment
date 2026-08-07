#include <DX3D/GameObject/ColliderComponent.h>
#include <DX3D/GameObject/GameObject.h>
#include <DX3D/GameObject/RigidBodyComponent.h>
#include <DX3D/Game/World.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Physics/PhysicsEngine.h>	

ColliderComponent::ColliderComponent() {

}

ColliderComponent::~ColliderComponent() {
	PhysicsEngine* p = getGameObject()->getWorld()->getGame()->getPhysicsEngne();
	auto rb = getGameObject()->getComponent<RigidBodyComponent>();

	if(rb && rb->getRawRigidBody() && m_collider){
		rb->getRawRigidBody()->removeCollider(m_collider);
	}

	if (m_shape) {
		switch (m_type) {
			case ColliderType::Box: {
				p->getPhysicsCommon().destroyBoxShape(static_cast<rp3d::BoxShape*>(m_shape));
				break;
			}
			case ColliderType::Sphere: {
				p->getPhysicsCommon().destroySphereShape(static_cast<rp3d::SphereShape*>(m_shape));
				break;
			}
			case ColliderType::Capsule: {
				p->getPhysicsCommon().destroyCapsuleShape(static_cast<rp3d::CapsuleShape*>(m_shape));
				break;
			}
		}
	}

}

void ColliderComponent::setAsBox(const Vector3D& halfExtents) {
	PhysicsEngine* p = getGameObject()->getWorld()->getGame()->getPhysicsEngne();
	m_type = ColliderType::Box;

	Vector3D safeExtents = halfExtents;
	if (safeExtents.m_x <= 0.0f) safeExtents.m_x = 0.5f;
	if (safeExtents.m_y <= 0.0f) safeExtents.m_y = 0.01f;
	if (safeExtents.m_z <= 0.0f) safeExtents.m_z = 0.5f;
	
	rp3d::Vector3 temp = rp3d::Vector3(safeExtents.m_x, safeExtents.m_y, safeExtents.m_z);

	m_shape = p->getPhysicsCommon().createBoxShape(temp);

	attachShape(m_shape);

	std::cout << "Name: " << getGameObject()->getName() << std::endl;
	std::cout << "Corrected Mesh HalfExtents -> X: " << safeExtents.m_x << ", Y: " << safeExtents.m_y << ", Z: " << safeExtents.m_z << std::endl;
}

void ColliderComponent::setAsSphere(f32 radius) {
	PhysicsEngine* p = getGameObject()->getWorld()->getGame()->getPhysicsEngne();
	m_type = ColliderType::Sphere;
	m_shape = p->getPhysicsCommon().createSphereShape(radius);
	attachShape(m_shape);
}

void ColliderComponent::setAsCapsule(f32 radius, f32 height) {
	PhysicsEngine* p = getGameObject()->getWorld()->getGame()->getPhysicsEngne();
	m_type = ColliderType::Capsule;
	m_shape = p->getPhysicsCommon().createCapsuleShape(radius, height);
	attachShape(m_shape);
}

void ColliderComponent::onCreateInternal() {
	//setAsBox(Vector3D(0.2f, 0.2f, 0.2f)); // default collider shape for now
}

void ColliderComponent::onActivate() {

}

void ColliderComponent::onDeactivate() {

}

void ColliderComponent::attachShape(rp3d::CollisionShape* shape) {
	RigidBodyComponent* rb = getGameObject()->getComponent<RigidBodyComponent>();

	if (rb == nullptr) {
		rb = getGameObject()->createComponent<RigidBodyComponent>();
	}

	if (rb->getRawRigidBody()) {
		m_collider = rb->getRawRigidBody()->addCollider(shape, rp3d::Transform::identity());
	}

	m_collider->setUserData(this);
}
