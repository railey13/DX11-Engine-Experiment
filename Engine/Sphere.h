#pragma once
#include "GameObject.h"

class Sphere : public GameObject {
public:
	Sphere();
	~Sphere();
public:
	// Inherited via GameObject
	void update(f32 deltaTime) override;
	void draw(Matrix4x4 view, Matrix4x4 proj) override;
private:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	ui32 m_stackCount = 16;
	ui32 m_sliceCount = 24;
	f32 radius = 0.3f;
};