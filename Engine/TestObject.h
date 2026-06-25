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
	f32 m_time;
	f32 padding[3];
};

class TestObject {
public:
	TestObject();
	TestObject(i32 colorID);
	~TestObject();

	void release();
public:
	void Translate(Vector3D pos);
public:
	void update(GraphicsEngine* graphEngine, RECT rc);
	void render(GraphicsEngine* graphEngine);
public:
	f32 speed;
private:
	f32 rotateX = 0.0f;
	f32 rotateY = 0.0f;
	f32 scale = 10.f;

	f32 m_forward = 0.0f;
	f32 m_strafe = 0.0f;
public:
	void TranslateForward(f32 dir);
	void TranslateSideward(f32 dir);
	void RotateX(f32 x);
	void RotateY(f32 y);
	void Scale(f32 scale);
private:
	void initialize(i32 colorID);

	Vector3D GenerateRandomVelocity();
	Vector3D GetColor(i32 colorID);
private:
	Vector3D m_velocity;
	Vector3D m_position;

	Matrix4x4 m_world_cam;
private:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	ConstantBufferPtr m_cb;
};

