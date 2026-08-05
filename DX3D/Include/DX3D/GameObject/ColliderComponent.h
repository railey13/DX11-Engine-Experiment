#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/GameObject/Component.h>
#include <ReactPhysics3D/reactphysics3d.h>

class ColliderComponent : public Component {
public:
	ColliderComponent();
	virtual ~ColliderComponent();
public:
	void setAsBox(const Vector3D& halfExtents);
	void setAsSphere(f32 radius);
	void setAsCapsule(f32 radius, f32 height);
protected:
	void onCreateInternal();
	void onActivate();
	void onDeactivate();
private:
	void attachShape(rp3d::CollisionShape* shape);
private:
	rp3d::CollisionShape* m_shape = nullptr;
	rp3d::Collider* m_collider = nullptr;
	ColliderType m_type = ColliderType::Box;
};

