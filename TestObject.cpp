#include "TestObject.h"
#include "EngineTime.h"
#include <iostream>
#include <random>

TestObject::TestObject() : TestObject(0) {
	
}

TestObject::TestObject(int colorID) {
	initialize(colorID);
}

TestObject::~TestObject() {

}

void TestObject::release() {
	m_vb->release();
	m_ib->release();
	m_cb->release();
	m_vs->release();
	m_ps->release();
}

void TestObject::Translate(Vector3D pos) {
	m_position = pos;
}

void TestObject::update(GraphicsEngine* graphEngine, RECT rc){
	float deltaTime = EngineTime::getDeltaTime();
	constant cc;

	//cc.m_time = (float)EngineTime::getTotalTime();

	//float tSpeed = 0.3f;
	//float cSpeed = 10.0f;
	//float rSpeed = 0.5f;

	//float t = fmod(EngineTime::getTotalTime() * tSpeed * 0.5f, 1.0f);
	//float s = sin(EngineTime::getTotalTime() * cSpeed) * 0.5f + 0.5f;
	//float r = 0;

	//r += EngineTime::getTotalTime() * speed / 0.5f;

	Matrix4x4 temp;

	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5f, 0.5f, 0), Vector3D(2, 2, 0), s));

	//temp.setTranslation(Vector3D::lerp(Vector3D(0, -2, 0), Vector3D(0, 2, 0), t));

	//cc.m_world *= temp;

	m_position.m_x += m_velocity.m_x * deltaTime;
	m_position.m_y += m_velocity.m_y * deltaTime;

	float halfWidth = (rc.right - rc.left) / 800.0f;
	float halfHeight = (rc.bottom - rc.top) / 800.0f;

	// offset value of the vertices from the origin (when setting up the vertices of this cube)
	const float halfCube = 0.2f;

	float maxX = halfWidth - halfCube;
	float minX = -halfWidth + halfCube;
	float maxY = halfHeight - halfCube;
	float minY = -halfHeight + halfCube;


	if (m_position.m_x < minX || m_position.m_x > maxX) {
		m_velocity.m_x *= -1.0f; // inverse the direction
		m_position.m_x = (m_position.m_x < minX) ? minX : maxX; // clamp the pos to prevent getting stuck
	}

	if (m_position.m_y < minY || m_position.m_y > maxY) {
		m_velocity.m_y *= -1.0f;
		m_position.m_y = (m_position.m_y < minY) ? minY : maxY;
	}

	cc.m_world.setScale(Vector3D(scale, scale, scale));

	temp.setIdentity();
	temp.setRotationZ(0);

	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(rotateY);

	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(rotateX);

	cc.m_world *= temp;

	temp.setIdentity();
	temp.setTranslation(m_position);

	cc.m_world *= temp;

	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH(
		(rc.right - rc.left) / 400.f,
		(rc.bottom - rc.top) / 400.f,
		-4.0f,
		4.0f
	);



	m_cb->update(graphEngine->getImmediateDeviceContext(), &cc);
}

void TestObject::render(GraphicsEngine* graphEngine) {
	graphEngine->getImmediateDeviceContext()->setCosntantBuffer(m_vs, m_cb);
	graphEngine->getImmediateDeviceContext()->setCosntantBuffer(m_ps, m_cb);

	graphEngine->getImmediateDeviceContext()->setVertexShader(m_vs);
	graphEngine->getImmediateDeviceContext()->setPixelShader(m_ps);

	graphEngine->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	graphEngine->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	//graphEngine->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
	graphEngine->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
}

void TestObject::RotateX(float x) {
	rotateX += x * EngineTime::getDeltaTime();
}

void TestObject::RotateY(float y) {
	rotateY += y * EngineTime::getDeltaTime();
}

void TestObject::Scale(float scale) {
	this->scale = scale;
}

void TestObject::initialize(int colorID) {
	Vector3D color = GetColor(colorID);
	vertex list[] = {
		// POS - COLOR - COLOR 1
		// X - Y - Z
		// FRONT FACE
		{Vector3D(-0.2f, -0.2f, -0.2f), color, color}, // POS1
		{Vector3D(-0.2f, 0.2f, -0.2f),	color, color}, // POS2
		{Vector3D(0.2f, 0.2f, -0.2f),	color, color}, // POS3 
		{Vector3D(0.2f, -0.2f, -0.2f),	color, color}, // POS4

		// BACK FACE
		{Vector3D(0.2f, -0.2f, 0.2f),	color, color}, // POS1
		{Vector3D(0.2f, 0.2f, 0.2f),	color, color}, // POS2
		{Vector3D(-0.2f, 0.2f, 0.2f),	color, color}, // POS3 
		{Vector3D(-0.2f, -0.2f, 0.2f),	color, color}, // POS4
	};

	UINT size_list = ARRAYSIZE(list);

	unsigned int index_list[] = {
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

	UINT size_index_list = ARRAYSIZE(index_list);

	GraphicsEngine* graphEngine = GraphicsEngine::get();

	m_vb = graphEngine->createVertexBuffer();
	m_ib = graphEngine->createIndexBuffer();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	graphEngine->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = graphEngine->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	m_ib->load(index_list, size_index_list);

	graphEngine->releaseCompiledShader();

	graphEngine->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = graphEngine->createPixelShader(shader_byte_code, size_shader);

	constant cc;
	cc.m_time = 0;

	m_cb = graphEngine->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));

	graphEngine->releaseCompiledShader();

	m_velocity = GenerateRandomVelocity();
}

Vector3D TestObject::GenerateRandomVelocity()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

	Vector3D vel(dist(gen), dist(gen), 0.0f);

	vel.normalize();

	return vel * 0.5f;
}

Vector3D TestObject::GetColor(int colorID) {
	switch (colorID) {
	case 1: return Vector3D(1.0f, 0.0f, 0.0f);   // Red
	case 2: return Vector3D(1.0f, 0.5f, 0.0f);   // Orange
	case 3: return Vector3D(1.0f, 1.0f, 0.0f);   // Yellow
	case 4: return Vector3D(0.0f, 1.0f, 0.0f);   // Green
	case 5: return Vector3D(0.0f, 0.0f, 1.0f);   // Blue
	case 6: return Vector3D(0.29f, 0.0f, 0.51f);  // Indigo
	case 7: return Vector3D(0.54f, 0.17f, 0.89f); // Violet
	default: return Vector3D(1.0f, 1.0f, 1.0f);  // White 
	}
}

