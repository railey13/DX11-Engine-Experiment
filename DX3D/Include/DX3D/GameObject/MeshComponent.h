#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/GameObject/Component.h>

#include <vector>

class MeshComponent : public Component {
public:
	MeshComponent();
	virtual ~MeshComponent();

	void setMesh(const MeshPtr& mesh);
	const MeshPtr& getMesh() { return m_mesh; }

	void addMaterial(const MaterialPtr& material);
	void removeMaterial(ui32 index);
	const std::vector<MaterialPtr>& getMaterials();

	void onActivate() override;
	void onDeactivate() override;

	MeshSource getMeshSource() const { return m_meshSource; }
	PrimitiveType getPrimitiveType() const { return m_primitiveType; }
	std::string getPrimitiveTypeString() const;
	std::wstring getFilePath() const;
protected:
	void onCreateInternal();
private:
	void setPrimitiveType(std::string type);
	void setPrimitiveType(PrimitiveType type);
private:
	MeshPtr m_mesh;
	std::vector<MaterialPtr> m_materials;

	PrimitiveType m_primitiveType = PrimitiveType::Cube;
	MeshSource m_meshSource = MeshSource::File;
private:
	friend class GameObject;
	friend class SceneHandler;
};

