#include <DX3D/GameObject/LightComponent.h>
#include <DX3D/GameObject/GameObject.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Game/World.h>
#include <DX3D/Graphics/GraphicsEngine.h>

LightComponent::LightComponent() {

}

LightComponent::~LightComponent() {
	m_gameobject->getWorld()->getGame()->getGraphicsEngine()->removeComponent(this);
}

void LightComponent::setLightType(LightType type) {
	m_light_type = type;
}

void LightComponent::setLightType(std::string type) {
	if (type == "DirectionLight") {
		m_light_type = LightType::DirectionLight;
	}
	else if (type == "PointLight") {
		m_light_type = LightType::PointLight;
	}
}

void LightComponent::setColor(Vector4D color) {
	m_color = color;
}

void LightComponent::setRadius(f32 radius) {
	m_radius = radius;
}

void LightComponent::setIntensity(f32 intensity) {
	m_intensity = intensity;
}

std::string LightComponent::getLightTypeString() const {
	switch (m_light_type) {
		case LightType::DirectionLight: return "DirectionLight";
		case LightType::PointLight: return "PointLight";
	}
}

void LightComponent::onCreateInternal() {
	m_gameobject->getWorld()->getGame()->getGraphicsEngine()->addComponent(this);
}

void LightComponent::onActivate() {
	m_active = true;
}

void LightComponent::onDeactivate() {
	m_active = false;
}