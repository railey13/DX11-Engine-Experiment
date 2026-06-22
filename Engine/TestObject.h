#pragma once
#include "DeviceContext.h"
#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Vector3D.h"
#include "Matrix4x4.h"

__declspec(align(16))
struct constant {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	float m_time;
	float padding[3];
};

class TestObject {
public:
	TestObject();
	TestObject(int colorID);
	~TestObject();

	void release();
public:
	void Translate(Vector3D pos);
public:
	void update(GraphicsEngine* graphEngine, RECT rc);
	void render(GraphicsEngine* graphEngine);
public:
	float speed;
private:
	float rotateX = 0.0f;
	float rotateY = 0.0f;
	float scale = 10.f;

	float m_forward = 0.0f;
	float m_strafe = 0.0f;
public:
	void TranslateForward(float dir);
	void TranslateSideward(float dir);
	void RotateX(float x);
	void RotateY(float y);
	void Scale(float scale);
private:
	void initialize(int colorID);

	Vector3D GenerateRandomVelocity();
	Vector3D GetColor(int colorID);
private:
	Vector3D m_velocity;
	Vector3D m_position;

	Matrix4x4 m_world_cam;
private:
	VertexBuffer* m_vb;
	IndexBuffer* m_ib;
	VertexShader* m_vs;
	PixelShader* m_ps;
	ConstantBuffer* m_cb;
};

