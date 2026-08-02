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
protected:
	void onCreateInternal();
private:
	MeshPtr m_mesh;
	std::vector<MaterialPtr> m_materials;

};

