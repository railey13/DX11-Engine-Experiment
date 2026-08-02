#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/GameObject/Component.h>
#include <DX3D/Math/Rect.h>
#include <DX3D/Math/Matrix4x4.h>

class CameraComponent : public Component {
public:
	CameraComponent();
	virtual ~CameraComponent();
public:
	void getProjectionMatrix(Matrix4x4& proj);
	void getViewMatrix(Matrix4x4& view);

	void setSensitivity(f32 sensitivity);
	void setFarPlane(f32 farPlane);
	void setNearPlane(f32 nearPlane);
	void setFieldOfView(f32 fieldOfView);
	void setType(CameraType type);
	void setScreenArea(const Rect& area);

	f32 getSensitivity(){ return m_sensitivity; }
	f32 getFarPlane() { return m_farPlane; }
	f32 getNearPlane() { return m_nearPlane; }
	f32 getFieldOfView() { return m_fieldOfView; }
	CameraType getType() { return m_type; }
	Rect getScreenArea() { return m_screenArea; }
protected:
	virtual void onCreateInternal();
private:
	void computeProjectionMatrix();
private:
	Matrix4x4 m_projection;

	f32 m_farPlane = 100.0f;
	f32 m_nearPlane = 0.01f;
	f32 m_fieldOfView = 1.3f;
	f32 m_sensitivity = 1.0f;

	CameraType m_type = CameraType::Perspective;
	Rect m_screenArea;
};

