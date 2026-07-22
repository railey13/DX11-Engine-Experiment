#include "Pot.h"
#include "iostream"

Pot::Pot() {
	RenderSystem* render = GraphicsEngine::get()->getRenderSystem();

	setTexture(GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/CartethyiaPuppet.gif"));
	mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets/Meshes/teapot.obj");

	constant cc;
	cc.m_time = 0;
	m_cb = render->createConstantBuffer(&cc, sizeof(constant));

	m_name = "Pot";
}

Pot::~Pot() {

}

void Pot::update(f32 deltaTime) {

}

void Pot::draw(Matrix4x4 view, Matrix4x4 proj) {
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

	context->setVertexBuffer(mesh->getVertexBuffer());
	context->setIndexBuffer(mesh->getIndexBuffer());

	context->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
}
