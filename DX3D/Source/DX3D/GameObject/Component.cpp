#include <DX3D/GameObject/Component.h>
#include <DX3D/GameObject/GameObject.h>

Component::Component() {

}

Component::~Component() {
	
}

void Component::release() {
	m_gameobject->removeComponent(m_typeId);
}

void Component::setActive(bool active) {
	m_active = active;
}

void Component::onCreateInternal() {

}

void Component::onActivate() {

}

void Component::onDeactivate() {

}
