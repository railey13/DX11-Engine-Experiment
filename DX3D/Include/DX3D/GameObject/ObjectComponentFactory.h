#pragma once
#include <DX3D/Prerequisites.h>

class ObjectComponentFactory {
public:
	static void CreateDirLight(GameObject* obj) {
		auto dir = obj->createComponent<LightComponent>();
		dir->setColor(Vector4D(1, 1, 1, 1));
		dir->setLightType(LightType::DirectionLight);
		dir->setRadius(1.0f);
		obj->setName(dir->getLightTypeString());
	}

	static void CreatePointLight(GameObject* obj) {
		auto point = obj->createComponent<LightComponent>();
		point->setColor(Vector4D(1, 1, 1, 1));
		point->setLightType(LightType::PointLight);
		point->setRadius(1.0f);
		obj->setName(point->getLightTypeString());
	}
};