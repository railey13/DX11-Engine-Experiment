#pragma once
#include "GameObject.h"
#include "DX3D/Resource/Mesh.h"
class Bunny : public GameObject {
public:
	Bunny();
	~Bunny();
public:
	// Inherited via GameObject
	void update(f32 deltaTime) override;
	void draw(Matrix4x4 view, Matrix4x4 proj) override;
private:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	MeshPtr mesh;
};

