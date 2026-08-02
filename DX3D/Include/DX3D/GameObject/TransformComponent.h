#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/GameObject/Component.h>
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/Matrix4x4.h>
	
class TransformComponent : public Component  {
public:
	TransformComponent();
	~TransformComponent();
public:
	void setPosition(const Vector3D& position);
	void setRotation(const Vector3D& rotation);
	void setScale(const Vector3D& scale);

	Vector3D getPosition() const { return m_position; };
	Vector3D getRotation() const { return m_rotation; };
	Vector3D getScale() const { return m_scale; };
	void getWorldMatrix(Matrix4x4& mat) const { return mat.setMatrix(m_worldMatrix); }
	Vector3D getForwardDirection();
protected:
	void updateWorldMatrix();
protected:
	Vector3D m_position = Vector3D(0, 0, 0);
	Vector3D m_rotation = Vector3D(0,0,0);
	Vector3D m_scale = Vector3D(1,1,1);

	Matrix4x4 m_worldMatrix;
};

