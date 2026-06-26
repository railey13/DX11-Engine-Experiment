#include "Plane.h"
#include "iostream"

Plane::Plane(void* shader_byte_code, size_t size_shader) {
	vertex list[] = {
		// POS - COLOR - COLOR 1
		// X - Y - Z
		// FRONT FACE
		{Vector3D(-1, 0, -1),	Vector3D(1.0f, 1.0f, 1.0f), Vector3D(1.0f, 1.0f, 1.0f)}, // POS1
		{Vector3D(-1, 0, 1),	Vector3D(1.0f, 1.0f, 1.0f), Vector3D(1.0f, 1.0f, 1.0f)}, // POS2
		{Vector3D(1, 0, 1),		Vector3D(1.0f, 1.0f, 1.0f), Vector3D(1.0f, 1.0f, 1.0f)}, // POS3 
		{Vector3D(1, 0, -1),	Vector3D(1.0f, 1.0f, 1.0f), Vector3D(1.0f, 1.0f, 1.0f)}, // POS4
	};

	ui32 size_list = ARRAYSIZE(list);

	ui32 index_list[] = {
		// FRONT SIDE
		0,1,2, // FIRST TRIANGLE
		2,3,0, // SECOND TRIANGLE
		// BACK SIDE
		4,5,6,
		6,7,4,
		// TOP SIDE
		1,6,5,
		5,2,1,
		// BOTTOM SIDE
		7,0,3,
		3,4,7,
		// RIGHT SIDE
		3,2,5,
		5,4,3,
		// LEFT SIDE
		7,6,1,
		1,0,7
	};

	ui32 size_index_list = ARRAYSIZE(index_list);

	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);

	constant cc;
	cc.m_time = 0;
	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

}

Plane::~Plane() {

}

void Plane::update(f32 deltaTime) {

}

void Plane::draw(VertexShaderPtr vs, PixelShaderPtr ps, Matrix4x4 view, Matrix4x4 proj) {
	constant cc;
	Matrix4x4 temp;

	cc.m_time = 0;

	cc.m_world.setIdentity();

	temp.setIdentity();
	temp.setScale(Vector3D(m_scale.m_x, 1.0f, m_scale.m_z));

	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationZ(m_rotation.m_z);

	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rotation.m_y);

	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_rotation.m_x);

	cc.m_world *= temp;

	temp.setTranslation(m_position);

	cc.m_world *= temp;

	cc.m_view = view;
	cc.m_proj = proj;

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(vs, m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(ps, m_cb);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
}
