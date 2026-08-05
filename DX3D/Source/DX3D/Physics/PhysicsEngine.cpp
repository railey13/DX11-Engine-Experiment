#include <DX3D/Physics/PhysicsEngine.h>
#include <DX3D/GameObject/RigidBodyComponent.h>

PhysicsEngine::PhysicsEngine(Game* game) : m_game(game){
	m_physicsWorld = m_physicsCommon.createPhysicsWorld();
}

PhysicsEngine::~PhysicsEngine() {
	if (m_physicsWorld) {
		m_physicsCommon.destroyPhysicsWorld(m_physicsWorld);
	}
}

void PhysicsEngine::update(f32 deltaTime) {
	constexpr f32 maxFrameTime = 0.25f; // for clamping

	m_physicsAccumulator += std::min(deltaTime, maxFrameTime);

	while (m_physicsAccumulator >= fixedTime) {
		m_physicsWorld->update(deltaTime);

		for (auto rb : m_rigidBodies) {
			if (rb->isActive()) {
				rb->syncPhysicsToTransform();
			}
		}

		m_physicsAccumulator -= fixedTime;
	}

}

void PhysicsEngine::addComponent(Component* component) {
	if (auto rb = dynamic_cast<RigidBodyComponent*>(component)) 
		m_rigidBodies.emplace(rb);
}

void PhysicsEngine::removeComponent(Component* component) {
	if (auto c = dynamic_cast<RigidBodyComponent*>(component))
		m_rigidBodies.erase(c);
}
