#include <DX3D/Physics/PhysicsEngine.h>
#include <DX3D/GameObject/RigidBodyComponent.h>
#include <DX3D/GameObject/ColliderComponent.h>
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
	// For each contact pair
	for (ui32 p = 0; p < callBackData.getNbContactPairs(); p++) {

		// Get the contact pair
		CollisionCallback::ContactPair contactPair = callBackData.getContactPair(p);
		
		ColliderComponent* col1 = static_cast<ColliderComponent*>(contactPair.getBody1()->getUserData());
		ColliderComponent* col2 = static_cast<ColliderComponent*>(contactPair.getBody2()->getUserData());
		if (!col1 || !col2) continue;

		GameObject* obj1 = col1->getGameObject();
		GameObject* obj2 = col2->getGameObject();
		if (!obj1 || !obj2) continue;

		switch (contactPair.getEventType()) {
			case rp3d::CollisionCallback::ContactPair::EventType::ContactStart: {
				obj1->onCollisionEnter(obj2);
				obj2->onCollisionEnter(obj1);
				break;
			}
			case rp3d::CollisionCallback::ContactPair::EventType::ContactStay: {
				obj1->onCollisionStay(obj2);
				obj2->onCollisionStay(obj1);
				break;
			}
			case rp3d::CollisionCallback::ContactPair::EventType::ContactExit: {
				obj1->onCollisionExit(obj2);
				obj2->onCollisionExit(obj1);
				break;
			}
		}
	}
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
