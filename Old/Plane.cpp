#include "Plane.h"
#include "iostream"

Plane::Plane() {
	Vector3D position_list[] = {
		{Vector3D(-1, 0, -1), }, // POS1
		{Vector3D(-1, 0,  1), }, // POS2
		{Vector3D(1,  0,  1), }, // POS3 
		{Vector3D(1,  0, -1)  }, // POS4
	};

	Vector2D texcoord_list[] = {
		{Vector2D(0.0f, 0.0f)},
		{Vector2D(0.0f, 1.0f)},
		{Vector2D(1.0f, 0.0f)},
		{Vector2D(1.0f, 1.0f)}
	};


	vertex list[] = {
		// FRONT SIDE
		{position_list[0], texcoord_list[1]},
		{position_list[1], texcoord_list[0]},
		{position_list[2], texcoord_list[2]},
		{position_list[3], texcoord_list[3]},
	};

	ui32 size_list = ARRAYSIZE(list);

	ui32 index_list[] = {
		// FRONT SIDE
		0,1,2, // FIRST TRIANGLE
		2,3,0, // SECOND TRIANGLE
	};

	ui32 size_index_list = ARRAYSIZE(index_list);

	RenderSystem* render = GraphicsEngine::get()->getRenderSystem();
	ShaderManager* shader = GraphicsEngine::get()->getShaderManager();

	m_vb = render->createVertexBuffer(list, sizeof(vertex), size_list, shader->getVSByteCode(), shader->getVSSize());
	m_ib = render->createIndexBuffer(index_list, size_index_list);

	constant cc;
	cc.m_time = 0;
	m_cb = render->createConstantBuffer(&cc, sizeof(constant));

	m_name = "Plane";
}

Plane::~Plane() {

}

void Plane::update(f32 deltaTime) {

}

void Plane::draw(Matrix4x4 view, Matrix4x4 proj) {
	constant cc;
	Matrix4x4 temp;

	cc.m_time = 0;

	getTransform()->getWorldMatrix(cc.m_world);

	cc.m_view = view;
	cc.m_proj = proj;

	RenderSystem* render = GraphicsEngine::get()->getRenderSystem();

	m_cb->update(render->getImmediateDeviceContext(), &cc);

	DeviceContextPtr context = render->getImmediateDeviceContext();

	context->setConstantBuffer(m_cb);

	context->setTexutre(GraphicsEngine::get()->getShaderManager()->getPS(), m_tex);

	context->setVertexBuffer(m_vb);
	context->setIndexBuffer(m_ib);

	context->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
}
