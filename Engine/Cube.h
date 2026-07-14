#pragma once
#include "AGameObject.h"

class Cube : public AGameObject {
public:
	Cube();
	~Cube();
public:
	// Inherited via AGameObject
	void update(f32 deltaTime) override;
	void draw(Matrix4x4 view, Matrix4x4 proj) override;
private:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;
};

