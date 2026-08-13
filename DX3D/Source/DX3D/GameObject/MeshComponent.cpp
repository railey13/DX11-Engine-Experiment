#include <DX3D/GameObject/MeshComponent.h>
#include <DX3D/GameObject/GameObject.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Game/World.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Resource/Mesh.h>

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

void MeshComponent::onActivate() {
	m_active = true;
}

void MeshComponent::onDeactivate() {
	m_active = false;
}

void MeshComponent::setPrimitiveType(std::string type) {
	if (type == "Cube") m_primitiveType = PrimitiveType::Cube;
	else if (type == "Sphere") m_primitiveType = PrimitiveType::Sphere;
	else if (type == "Plane") m_primitiveType = PrimitiveType::Plane;
	else if (type == "Capsule") m_primitiveType = PrimitiveType::Capsule;
}

void MeshComponent::setPrimitiveType(PrimitiveType type) {
	m_primitiveType = type;
}

std::string MeshComponent::getPrimitiveTypeString() const {
	switch (m_primitiveType) {
		case PrimitiveType::Cube: return "Cube"; 
		case PrimitiveType::Sphere: return "Sphere";
		case PrimitiveType::Plane: return "Plane";
		case PrimitiveType::Capsule: return "Capsule";
	}
}

std::wstring MeshComponent::getFilePath() const {
	return m_mesh->m_full_path;
}

void MeshComponent::onCreateInternal() {
	m_gameobject->getWorld()->getGame()->getGraphicsEngine()->addComponent(this);
}

