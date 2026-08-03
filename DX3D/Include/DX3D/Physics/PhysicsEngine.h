#pragma once
#include <DX3D/Prerequisites.h>
#include <set>

class PhysicsEngine {
public:
	PhysicsEngine(Game* game);
	virtual ~PhysicsEngine();

	void update();
public:
	void addComponent(Component* component);
	void removeComponent(Component* component);
private:
	Game* m_game = nullptr;
	std::set<Component*> m_components;
};

