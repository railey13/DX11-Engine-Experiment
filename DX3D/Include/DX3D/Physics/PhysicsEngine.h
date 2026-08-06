#pragma once
#include <DX3D/Prerequisites.h>
#include <reactphysics3d/reactphysics3d.h>
#include <set>

class PhysicsEngine {
public:
	PhysicsEngine(Game* game);
	~PhysicsEngine();

	void update(f32 deltaTime);
	void step();
public:
	void addComponent(Component* component);
	void removeComponent(Component* component);
		
	rp3d::PhysicsCommon& getPhysicsCommon() { return m_physicsCommon; }
	rp3d::PhysicsWorld* getPhysicsWorld() { return m_physicsWorld; }

	f32 getFixedTimeStep() const { return fixedTime; }
private:
	f32 fixedTime = 1 / 60.0f;
	f32 m_physicsAccumulator = 0.0f;
	Game* m_game = nullptr;
	rp3d::PhysicsCommon m_physicsCommon;
	rp3d::PhysicsWorld* m_physicsWorld = nullptr;

	std::set<RigidBodyComponent*> m_rigidBodies;
};

