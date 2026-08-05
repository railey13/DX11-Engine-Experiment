#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/GameObject/Component.h>
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/Matrix4x4.h>
#include <DX3D/Math/Quaternion.h>

//#include <reactphysics3d/mathematics/Quaternion.h>

class TransformComponent : public Component  {
public:
	TransformComponent();
	~TransformComponent();
public:
	void setPosition(const Vector3D& position);
	void setRotation(const Quaternion& rotation);
	void setRotationEuler(const Vector3D& rotation);
	void setScale(const Vector3D& scale);

	Vector3D getPosition() const { return m_position; }
	Quaternion getRotation() const { return m_rotation; }
	Vector3D getRotationEuler() const { return m_rotationEuler; }
	Vector3D getScale() const { return m_scale; }

	void getLocalMatrix(Matrix4x4& mat) const {return  mat.setMatrix(m_localMatrix); }
	void getWorldMatrix(Matrix4x4& mat) const { return mat.setMatrix(m_worldMatrix); }

	Vector3D getForwardDirection();
protected:
	void updateWorldMatrix();
protected:
	Vector3D m_position = Vector3D(0, 0, 0);
	Quaternion m_rotation = Quaternion::identity();
	Vector3D m_rotationEuler = Vector3D(0, 0, 0);
	Vector3D m_scale = Vector3D(1,1,1);

	Matrix4x4 m_localMatrix;
	Matrix4x4 m_worldMatrix;

	friend class GameObject;
};

