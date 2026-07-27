#include <DX3D/GameObject/MeshComponent.h>
#include <DX3D/GameObject/GameObject.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Game/World.h>
#include <DX3D/Graphics/GraphicsEngine.h>

MeshComponent::MeshComponent() {

}

MeshComponent::~MeshComponent() {
	m_gameobject->getWorld()->getGame()->getGraphicsEngine()->removeComponent(this);
}

void MeshComponent::setMesh(const MeshPtr& mesh) {
	m_mesh = mesh;
}

void MeshComponent::addMaterial(const MaterialPtr& material) {
	m_materials.push_back(material);
}

void MeshComponent::removeMaterial(ui32 index) {
	if (index >= m_materials.size()) return;

	m_materials.erase(m_materials.begin() + index);
}

const std::vector<MaterialPtr>& MeshComponent::getMaterials() {
	return m_materials;
}

void MeshComponent::onCreateInternal() {
	m_gameobject->getWorld()->getGame()->getGraphicsEngine()->addComponent(this);
}
