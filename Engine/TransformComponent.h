#pragma once
#include "Prerequisites.h"
#include "AComponent.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
	
class TransformComponent : public AComponent  {
public:
	TransformComponent();
	~TransformComponent();
public:
	void setPosition(const Vector3D& position);
	Vector3D getPosition();

	void setRotation(const Vector3D& rotation);
	Vector3D getRotation();

	void setScale(const Vector3D& scale);
	Vector3D getScale();

	void getWorldMatrix(Matrix4x4& mat);
protected:
	void updateWorldMatrix();
protected:
	Vector3D m_position;
	Vector3D m_rotation;
	Vector3D m_scale = Vector3D(1,1,1);

	Matrix4x4 m_worldMatrix;
};

