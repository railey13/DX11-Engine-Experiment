#include <DX3D/GameObject/GameObject.h>
#include <DX3D/GameObject/TransformComponent.h>
#include <DX3D/GameObject/MeshComponent.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Game/World.h>
#include <DX3D/Game/Game.h>

GameObject::GameObject() : m_name("GameObject") {
	m_transform = createComponent<TransformComponent>();
}

GameObject::~GameObject() {
	std::cout << "DELETED: " << m_name.c_str() << std::endl;
	if (m_parent) {
		m_parent->removeChild(this);
		m_parent = nullptr;
	}
	
	while (!m_children.empty()) {
		auto c = m_children.back();

		c->setParent(nullptr);
	}

	m_components.clear();
}

void GameObject::release() {
	m_world->removeGameObject(this);
}

void GameObject::createComponentInternal(Component* component, size_t id) {
	auto compPtr = std::unique_ptr<Component>(component);
	m_components.emplace(id, std::move(compPtr));
	component->m_typeId = id;
	component->m_gameobject = this;
	component->onCreateInternal();
}

Component* GameObject::getComponentInternal(size_t id) {

	auto it = m_components.find(id);

	if (it == m_components.end()) return nullptr;

	return it->second.get();
}

void GameObject::removeComponent(size_t id) {
	m_components.erase(id);
}

void GameObject::toggleComponentsActive(bool flag) {
	for (auto&& [typeID, component] : m_components) {
		component->setActive(flag);
	}
}

void GameObject::toggleChildrenActive(bool flag) {
	for (auto obj : m_children) {
		obj->setActive(flag);
	}
}

void GameObject::setName(const std::string& name) {
	m_name = m_world->generateUniqueName(name);
}

void GameObject::setActive(bool active) {
	m_active = active;
	toggleComponentsActive(active);
	toggleChildrenActive(active);
}

void GameObject::setParent(GameObject* parent) {
	if (m_parent == parent || parent == this) return;

	if (m_parent) {
		m_parent->removeChild(this);
	}

	m_parent = parent;

	if (m_parent) {
		m_parent->addChild(this);
	}

	m_transform->updateWorldMatrix();
}

InputSystem* GameObject::getInputSystem() {
	return m_world->getGame()->getInputSystem();
}

void GameObject::setMeshData(const MeshPtr& mesh, const MaterialPtr& material) {
	if (!mesh) return; 

	auto meshComponent = createComponent<MeshComponent>();
	meshComponent->setMesh(mesh);

	auto mat = material;
	if (!mat) {
		mat = m_world->getGame()->getResourceManager()->getDefaultMaterial();
	}
	meshComponent->addMaterial(mat);
}

void GameObject::addChild(GameObject* child) {
	m_children.push_back(child);
}

void GameObject::removeChild(GameObject* child) {
	m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
}
