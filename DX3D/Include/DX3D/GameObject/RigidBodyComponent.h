#pragma once
#include <DX3D/GameObject/Component.h>
#include <DX3D/Prerequisites.h>
#include <ReactPhysics3D/reactphysics3d.h>
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/Quaternion.h>

class RigidBodyComponent : public Component{
public:
	RigidBodyComponent();
	virtual ~RigidBodyComponent();
protected:
	void onCreateInternal();
	void onActivate() override;
	void onDeactivate() override;
public:
	void setBodyType(RBType type);
	void setMass(f32 mass);

	void enableGravity(bool flag);

	void applyLocalForceAtCenterOfMass(const Vector3D& force);
	void applyWorldForceAtCenterOfMass(const Vector3D& force);

	void syncPhysicsToTransform();
	void syncTransformToPhysics(const Vector3D& position, const Quaternion& rotation);

	void updateTransform(const Vector3D& position, const Quaternion& rotation);

	f32 getMass() const { return m_rigidBody->getMass(); }
	RBType getBodyType() const { return m_type; }
	rp3d::RigidBody* getRawRigidBody() { return m_rigidBody; }

	void setFreezeY(bool flag);
	bool isYFreeze() const { return m_freezeY; }

	void setActive(bool flag) override;
private:
	rp3d::RigidBody* m_rigidBody = nullptr;
	PhysicsEngine* m_physEngine = nullptr;
	RBType m_type = RBType::Static;

	bool m_freezeY = false;
};

