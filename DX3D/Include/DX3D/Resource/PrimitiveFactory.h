#pragma once
#include <DX3D/Prerequisites.h>

class PrimitiveFactory {
public:
	static void createCube(ResourceManager* manager, GameObject* obj);
	static void createPlane(ResourceManager* manager, GameObject* obj);
	static void createSphere(ResourceManager* manager, GameObject* obj);
	static void createCapsule(ResourceManager* manager, GameObject* obj);
};

