#include "AGameObject.h"

AGameObject::AGameObject() : m_name("GameObject") {
	m_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/white.png");
	m_transform = createComponent<TransformComponent>();
}

AGameObject::~AGameObject() {

}

TransformComponent* AGameObject::getTransform() {
	return m_transform;
}

void AGameObject::createComponentInternal(AComponent* component, size_t id) {
	auto compPtr = std::unique_ptr<AComponent>(component);
	m_components.emplace(id, std::move(compPtr));
	component->m_typeId = id;
	component->m_gameobject = this;
}

AComponent* AGameObject::getComponentInternal(size_t id) {

	auto it = m_components.find(id);

	if (it == m_components.end()) return nullptr;

	return it->second.get();
}

void AGameObject::removeComponent(size_t id) {
	m_components.erase(id);
}

void AGameObject::setTexture(TexturePtr tex) {
	m_tex = tex;
}

