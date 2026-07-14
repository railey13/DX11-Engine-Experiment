#include "Plane.h"
#include "iostream"

Plane::Plane() {
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	ShaderLibrary::get()->requestVertexShaderData(ShaderNames::BASE_VERTEX_SHADER_NAME, &shader_byte_code, &size_shader);

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

	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);

	constant cc;
	cc.m_time = 0;
	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

	m_name = "Plane";
}

Plane::~Plane() {

}

void Plane::update(f32 deltaTime) {

}

void Plane::draw(Matrix4x4 view, Matrix4x4 proj) {
	DeviceContextPtr context = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();

	context->setVertexShader(ShaderLibrary::get()->getVertexShader(ShaderNames::BASE_VERTEX_SHADER_NAME));
	context->setPixelShader(ShaderLibrary::get()->getPixelShader(ShaderNames::BASE_PIXEL_SHADER_NAME));

	constant cc;
	Matrix4x4 temp;

	cc.m_time = 0;

	getTransform()->getWorldMatrix(cc.m_world);

	cc.m_view = view;
	cc.m_proj = proj;

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);

	context->setConstantBuffer(m_cb);

	context->setTexutre(ShaderLibrary::get()->getPixelShader(ShaderNames::BASE_PIXEL_SHADER_NAME), m_tex);

	context->setVertexBuffer(m_vb);
	context->setIndexBuffer(m_ib);

	context->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
}
