#pragma once
#include "AGameObject.h"

class Cube : public AGameObject {
public:
	Cube(void* shader_byte_code, size_t size_shader);
	~Cube();
public:
	// Inherited via AGameObject
	void update(f32 deltaTime) override;
	void draw(VertexShaderPtr vs, PixelShaderPtr ps, Matrix4x4 view, Matrix4x4 proj) override;
private:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;
};

