#include <DX3D/GameObject/CameraComponent.h>
#include <DX3D/GameObject/GameObject.h>
#include <DX3D/GameObject/TransformComponent.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Game/World.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Debug/Debug.h>

CameraComponent::CameraComponent() {

}

CameraComponent::~CameraComponent() {
	m_gameobject->getWorld()->getGame()->getGraphicsEngine()->removeComponent(this);
}

void CameraComponent::getProjectionMatrix(Matrix4x4& proj) {
	proj = m_projection;
}

void CameraComponent::getViewMatrix(Matrix4x4& view) {
	m_gameobject->getTransform()->getWorldMatrix(view);

	view.inverse();
}

void CameraComponent::setSensitivity(f32 sensitivity) {
	m_sensitivity = sensitivity;
}

void CameraComponent::setFarPlane(f32 farPlane) {
	m_farPlane = farPlane;
	computeProjectionMatrix();
}

void CameraComponent::setNearPlane(f32 nearPlane) {
	m_nearPlane = nearPlane;
	computeProjectionMatrix();
}

void CameraComponent::setFieldOfView(f32 fieldOfView) {
	m_fieldOfView = fieldOfView;
	computeProjectionMatrix();
}

void CameraComponent::setType(CameraType type) {
	m_type = type;
	computeProjectionMatrix();
}

void CameraComponent::setScreenArea(const Rect& area) {
	m_screenArea = area;
	computeProjectionMatrix();
}

void CameraComponent::onCreateInternal() {
	if (m_gameobject->getName() == "EditorCamera") return;

	m_gameobject->getWorld()->getGame()->getGraphicsEngine()->addComponent(this);
}

void CameraComponent::computeProjectionMatrix() {
	if (m_type == CameraType::Perspective)
		m_projection.setPerspectiveFovLH(
			m_fieldOfView,
			(f32)m_screenArea.width / (f32)m_screenArea.height,
			m_nearPlane,
			m_farPlane
		);
	else if (m_type == CameraType::Orthographic)
		m_projection.setOrthoLH(
			(f32)m_screenArea.width,
			(f32)m_screenArea.height,
			m_nearPlane,
			m_farPlane
			);
}

