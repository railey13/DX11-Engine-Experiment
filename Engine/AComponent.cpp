#include "AComponent.h"
#include "AGameObject.h"

AComponent::AComponent() {

}

AComponent::~AComponent() {

}

void AComponent::release() {
	m_gameobject->removeComponent(m_typeId);
}
