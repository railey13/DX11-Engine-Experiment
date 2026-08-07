#include <DX3D/Physics/PhysicsEngine.h>
#include <DX3D/GameObject/RigidBodyComponent.h>
#include <DX3D/GameObject/GameObject.h>

PhysicsEngine::PhysicsEngine(Game* game) : m_game(game){
	rp3d::PhysicsWorld::WorldSettings settings;
	m_physicsWorld = m_physicsCommon.createPhysicsWorld(settings);

	m_physicsWorld->setEventListener(this);
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
		step();
		m_physicsAccumulator -= fixedTime;
	}

}

void PhysicsEngine::step() {
	m_physicsWorld->update(fixedTime);

	for (auto rb : m_rigidBodies) {
		if (rb->isActive()) {
			rb->syncPhysicsToTransform();
		}
	}
}

void PhysicsEngine::onContact(const rp3d::CollisionCallback::CallbackData& callBackData) {

}

void PhysicsEngine::onTrigger(const rp3d::OverlapCallback::CallbackData& callBackData) {

}

void PhysicsEngine::addComponent(Component* component) {
	if (auto rb = dynamic_cast<RigidBodyComponent*>(component)) 
		m_rigidBodies.emplace(rb);
}

void PhysicsEngine::removeComponent(Component* component) {
	if (auto c = dynamic_cast<RigidBodyComponent*>(component))
		m_rigidBodies.erase(c);
}
